// board enforces
//   in-state
//      accel set/resume/main on
//   out-state
//      brake rising edge
//      brake > 0mph
const int GM_MAX_STEER = 300;
const int GM_MAX_RT_DELTA = 128;          // max delta torque allowed for real time checks
const uint32_t GM_RT_INTERVAL = 250000;    // 250ms between real time checks
const int GM_MAX_RATE_UP = 8;
const int GM_MAX_RATE_DOWN = 12;
const int GM_DRIVER_TORQUE_ALLOWANCE = 50;
const int GM_DRIVER_TORQUE_FACTOR = 4;
const int GM_MAX_GAS = 3072;
const int GM_MAX_REGEN = 1404;
const int GM_MAX_BRAKE = 350;
const int GM_GAS_INTERCEPTOR_THRESHOLD = 458;  // (610 + 306.25) / 2ratio between offset and gain from dbc file
#define GM_GET_INTERCEPTOR(msg) (((GET_BYTE((msg), 0) << 8) + GET_BYTE((msg), 1) + ((GET_BYTE((msg), 2) << 8) + GET_BYTE((msg), 3)) / 2 ) / 2) // avg between 2 tracks

// Safety-relevant CAN messages for Bolt EV
#define MSG_RX_STEER      0x184   // RX from PSCM, for steering torque and status
#define MSG_RX_WHEEL      0x34A   // RX from EBCM, for wheel speed
#define MSG_RX_BUTTON     0x1E1   // RX from BCM, for Cruise Buttons
#define MSG_RX_BRAKE      0xF1    // RX from EBCM, for Brake Position
#define MSG_RX_GAS        0x1A1   // RX from EBCM, for Pedal Position
#define MSG_RX_PEDAL      0x201   // RX from Pedal Interceptor

#define MSG_TX_LKA        0x180   // TX by OP, for LKA commands
#define MSG_TX_ALIVE      0x409   // TX by OP, for ASCM Alive, To do : We need to check if this message is used for Bolt EV or not.
#define MSG_TX_ASCM       0x40A   // TX by OP, for ASCM, To do : We need to check if this message is used for Bolt EV or not.
#define MSG_TX_ACC        0x370   // TX by OP, for ACC Status, To do : We need to check if this message is used for Bolt EV or not.
#define MSG_TX_PEDAL      0x200   // TX by OP, for Pedal Interceptor
#define MSG_REGEN         0x189   // TX/RX for Regen Paddle


const CanMsg GM_TX_MSGS[] = {{MSG_TX_LKA, 0, 4}, {MSG_TX_ALIVE, 0, 7}, {MSG_TX_ASCM, 0, 7}, {MSG_TX_ACC, 0, 6}, {MSG_TX_PEDAL, 0, 6}, {MSG_REGEN, 0, 7}, // pt bus
                             {0x104c006c, 3, 3}, {0x10400060, 3, 5}};  // gmlan

// TODO: do checksum and counter checks. Add correct timestep, 0.1s for now.
AddrCheckStruct gm_addr_checks[] = {
  {.msg = {{MSG_RX_STEER, 0, 8, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{MSG_RX_WHEEL, 0, 5, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{MSG_RX_BUTTON, 0, 7, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{MSG_RX_BRAKE, 0, 6, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{MSG_RX_GAS, 0, 7, .expected_timestep = 100000U}, { 0 }, { 0 }}},
};
#define GM_RX_CHECK_LEN (sizeof(gm_addr_checks) / sizeof(gm_addr_checks[0]))
addr_checks gm_rx_checks = {gm_addr_checks, GM_RX_CHECK_LEN};

int cam_can_bus = -1;
int bus_camera = -1;
int bus_vehicle = -1;


static int gm_rx_hook(CANPacket_t *to_push) {

 bool valid = addr_safety_check(to_push, &gm_rx_checks, NULL, NULL, NULL);

  int addr = GET_ADDR(to_push);
  int bus = GET_BUS(to_push);

  if (bus == 1 && (addr == MSG_TX_LKA)) {
    cam_can_bus = 1;
  }
  if (bus == 2 && (addr == MSG_TX_LKA)) {
    cam_can_bus = 2;
  }

  if (valid && bus == 0) {
    if (addr == MSG_RX_STEER) {
      int torque_driver_new = ((GET_BYTE(to_push, 6) & 0x7) << 8) | GET_BYTE(to_push, 7);
      torque_driver_new = to_signed(torque_driver_new, 11);
      // update array of samples
      update_sample(&torque_driver, torque_driver_new);
    }

    // sample speed, really only care if car is moving or not
    // rear left wheel speed
    if (addr == MSG_RX_WHEEL) {
      vehicle_moving = GET_BYTE(to_push, 0) | GET_BYTE(to_push, 1);
    }

    // ACC steering wheel buttons
    if (addr == MSG_RX_BUTTON) {
      int button = (GET_BYTE(to_push, 5) & 0x70) >> 4;
      switch (button) {
        case 2:  // resume
        case 3:  // set
        case 5:  // main on
          controls_allowed = 1;
          break;
        case 6:  // cancel
          controls_allowed = 0;
          break;
        default:
          break;  // any other button is irrelevant
      }
    }

    // speed > 0
    if (addr == MSG_RX_BRAKE) {
      // Brake pedal's potentiometer returns near-zero reading
      // even when pedal is not pressed
      brake_pressed = GET_BYTE(to_push, 1) >= 10;
    }

    // Gas Interceptor Check
    if (addr == MSG_RX_PEDAL) {
      gas_interceptor_detected = 1;
      //int gas_interceptor = GM_GET_INTERCEPTOR(to_push);
      //gas_pressed = gas_interceptor > HONDA_GAS_INTERCEPTOR_THRESHOLD;
      //gas_interceptor_prev = gas_interceptor;
    }

    if ((addr == MSG_RX_GAS) && (!gas_interceptor_detected)) {
      gas_pressed = GET_BYTE(to_push, 6) != 0;
    }

    // Check if LKA camera are online
    // on powertrain bus.
    // 384 = ASCMLKASteeringCmd
    generic_rx_checks(addr == MSG_TX_LKA);
  }
  return valid;

    // exit controls on regen paddle
//    if (addr == 189) {
//      bool regen = GET_BYTE(to_push, 0) & 0x20U;
//      if (regen) {
//        controls_allowed = 1;
//      }
//    }

}

// all commands: gas/regen, friction brake and steering
// if controls_allowed and no pedals pressed
//     allow all commands up to limit
// else
//     block all commands that produce actuation

static int gm_tx_hook(CANPacket_t *to_send, bool longitudinal_allowed) {

 bool boolValue =longitudinal_allowed;

  int tx = 1;
  int addr = GET_ADDR(to_send);

  if (!msg_allowed(to_send, GM_TX_MSGS, sizeof(GM_TX_MSGS)/sizeof(GM_TX_MSGS[0]))) {
    tx = 0;
    if (boolValue) {
       tx = 0 ;
    }
  }

  // disallow actuator commands if gas or brake (with vehicle moving) are pressed
  // and the the latching controls_allowed flag is True
  int pedal_pressed = brake_pressed_prev && vehicle_moving;
  bool unsafe_allow_gas = alternative_experience & ALT_EXP_DISABLE_DISENGAGE_ON_GAS;
  if (!unsafe_allow_gas) {
    pedal_pressed = pedal_pressed || gas_pressed_prev;
  }
  bool current_controls_allowed = controls_allowed; // && !pedal_pressed;

  // GAS: safety check (interceptor)
  if (addr == MSG_TX_PEDAL) {
    if (!current_controls_allowed ) {
      if (GET_BYTE(to_send, 0) || GET_BYTE(to_send, 1)) {
        tx = 0;
      }
    }
  }

  // BRAKE: safety check
//  if (addr == 789) {
//    int brake = ((GET_BYTE(to_send, 0) & 0xFU) << 8) + GET_BYTE(to_send, 1);
//    brake = (0x1000 - brake) & 0xFFF;
//    if (!current_controls_allowed) {
//      if (brake != 0) {
//        tx = 0;
//      }
//    }
//    if (brake > GM_MAX_BRAKE) {
//      tx = 0;
//    }
//  }

  // LKA STEER: safety check
  if (addr == MSG_TX_LKA) {
    int desired_torque = ((GET_BYTE(to_send, 0) & 0x7U) << 8) + GET_BYTE(to_send, 1);
    uint32_t ts = microsecond_timer_get();
    bool violation = 0;
    desired_torque = to_signed(desired_torque, 11);

    if (current_controls_allowed ) {

      // *** global torque limit check ***
      violation |= max_limit_check(desired_torque, GM_MAX_STEER, -GM_MAX_STEER);

      // *** torque rate limit check ***
      violation |= driver_limit_check(desired_torque, desired_torque_last, &torque_driver,
        GM_MAX_STEER, GM_MAX_RATE_UP, GM_MAX_RATE_DOWN,
        GM_DRIVER_TORQUE_ALLOWANCE, GM_DRIVER_TORQUE_FACTOR);

      // used next time
      desired_torque_last = desired_torque;

      // *** torque real time rate limit check ***
      violation |= rt_rate_limit_check(desired_torque, rt_torque_last, GM_MAX_RT_DELTA);

      // every RT_INTERVAL set the new limits
      uint32_t ts_elapsed = get_ts_elapsed(ts, ts_last);
      if (ts_elapsed > GM_RT_INTERVAL) {
        rt_torque_last = desired_torque;
        ts_last = ts;
      }
    }

    // no torque if controls is not allowed
    if (!current_controls_allowed && (desired_torque != 0)) {
      violation = 1;
    }

    // reset to 0 if either controls is not allowed or there's a violation
    if (violation || !current_controls_allowed) {
      desired_torque_last = 0;
      rt_torque_last = 0;
      ts_last = ts;
    }

//    if (violation) {
//      tx = 0;
//    }
  }

  // GAS/REGEN: safety check
  if (addr == 715) {
    int gas_regen = ((GET_BYTE(to_send, 2) & 0x7FU) << 5) + ((GET_BYTE(to_send, 3) & 0xF8U) >> 3);
    // Disabled message is !engaged with gas
    // value that corresponds to max regen.
    if (!current_controls_allowed || !longitudinal_allowed) {
      // Stock ECU sends max regen when not enabled
      if (gas_regen != GM_MAX_REGEN) {
        tx = 0;
      }
    }
    // Need to allow apply bit in pre-enabled and overriding states
    if (!controls_allowed) {
      bool apply = GET_BIT(to_send, 0U) != 0U;
      if (apply) {
        tx = 0;
      }
    }
    if (gas_regen > GM_MAX_GAS) {
      tx = 0;
    }
  }

  // 1 allows the message through
  return tx;
}

static int gm_fwd_hook(int bus_num, CANPacket_t *to_fwd) {

//  int bus_fwd = -1;
//
//  if (gm_allow_fwd && !gm_block_fwd) {
//    if (bus_num == 0) {
//      // TODO: Catch message 388 and edit the HandsOffSWlDetectionStatus to 1 (0 is hands off)
//      // Note: Blocking 388 causes error message when pressing LKAS button
//      bus_fwd = gm_camera_bus;
//    }
//    else if (bus_num == gm_camera_bus) {
//      int addr = GET_ADDR(to_fwd);
//      // block stock lkas messages and stock acc messages (if OP is doing ACC)
//      //TODO: Blocking stock camera ACC will need to be an option
//      int is_lkas_msg = (addr == 384);
//      int is_acc_msg = false;
//      //int is_acc_msg = (addr == 0x343);
//      int block_msg = is_lkas_msg || is_acc_msg;
//      if (!block_msg) {
//        bus_fwd = 0;
//      }
//    }
//  }
//  else {
//    // Evaluate traffic to determine if forwarding should be enabled (only camera on bus 2)
//    if (!gm_allow_fwd && !gm_block_fwd && bus_num == gm_camera_bus) {
//      int addr = GET_ADDR(to_fwd);
//      int len = GET_LEN(to_fwd);
//
//      if ((addr == 384 && len != 4) //chassis bus has 384 of different size
//        || (addr == 1120) // F_LRR_Obj_Header from object bus
//        || (addr == 784) // ASCMHeadlight from object bus
//        || (addr == 309) // LHT_CameraObjConfirmation_FO from object bus
//        || (addr == 192) // Unknown id only on chassis bus
//      ) {
//        gm_block_fwd = true;
//      }
//      else if (addr == 384 && len == 4) {
//        gm_good_cam_cnt++;
//      }
//
//      if (gm_good_cam_cnt > 10) {
//        gm_allow_fwd = true;
//      }
//    }
//  }
//
//  return bus_fwd;
  int bus_fwd = -1;
  int addr = GET_ADDR(to_fwd);

  if (cam_can_bus == 1) {
    bus_camera = 1;   //camera PT can bus for grey panda and custom made harness
  } else {
    if (cam_can_bus != -1){
      bus_camera = 2;  //camera PT can bus for comma ai harness
    }
  }

  if (bus_num == bus_vehicle) {
    bus_fwd = bus_camera;       //Forward all messages from vehicle
  }
  if (bus_num == bus_camera) {
    if (addr == MSG_TX_LKA) {
      bus_fwd = -1;    //Block LKA message from camera
    } else {
    bus_fwd = bus_vehicle;       //Forward all messages except LAK message
    }
  }
  return bus_fwd;
}


static const addr_checks* gm_init(int16_t param) {
  UNUSED(param);
  controls_allowed = false;
  relay_malfunction_reset();
  gas_interceptor_detected = 0;
  cam_can_bus = -1;
  bus_camera = -1;
  //bus_radar = 1;  // Radar can bus, Bolt EV doesn't need this can bus
  bus_vehicle = 0; //vehicle PT can bus for comma ai harness
  return &gm_rx_checks;
}


const safety_hooks gm_hooks = {
  .init = gm_init,
  .rx = gm_rx_hook,
  .tx = gm_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = gm_fwd_hook,
//  .addr_check = gm_addr_checks,
//  .addr_check_len = sizeof(gm_addr_checks) / sizeof(gm_addr_checks[0]),
};
