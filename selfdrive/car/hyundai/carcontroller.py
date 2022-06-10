from cereal import car, messaging
from common.realtime import DT_CTRL
from common.numpy_fast import clip, interp
from selfdrive.config import Conversions as CV
from selfdrive.car import apply_std_steer_torque_limits
from selfdrive.car.hyundai.hyundaican import create_lkas11, create_clu11, create_lfahda_mfc, create_hda_mfc, \
                                             create_scc11, create_scc12, create_scc13, create_scc14, \
                                             create_scc42a, create_scc7d0, create_mdps12
from selfdrive.car.hyundai.values import Buttons, CarControllerParams, CAR, FEATURES
from opendbc.can.packer import CANPacker
from selfdrive.controls.lib.longcontrol import LongCtrlState
from selfdrive.car.hyundai.carstate import GearShifter
from selfdrive.controls.lib.lateral_planner import LANE_CHANGE_SPEED_MIN

from selfdrive.car.hyundai.navicontrol  import NaviControl

from common.params import Params
import common.log as trace1
import common.CTime1000 as tm
from random import randint
from decimal import Decimal

VisualAlert = car.CarControl.HUDControl.VisualAlert
LongCtrlState = car.CarControl.Actuators.LongControlState


def process_hud_alert(enabled, fingerprint, visual_alert, left_lane,
                      right_lane, left_lane_depart, right_lane_depart):
  sys_warning = (visual_alert in [VisualAlert.steerRequired, VisualAlert.ldw])

  # initialize to no line visible
  sys_state = 1
  if left_lane and right_lane or sys_warning:  # HUD alert only display when LKAS status is active
    sys_state = 3 if enabled or sys_warning else 4
  elif left_lane:
    sys_state = 5
  elif right_lane:
    sys_state = 6

  # initialize to no warnings
  left_lane_warning = 0
  right_lane_warning = 0
  if left_lane_depart:
    left_lane_warning = 1 if fingerprint in [CAR.GENESIS_G90, CAR.GENESIS_G80] else 2
  if right_lane_depart:
    right_lane_warning = 1 if fingerprint in [CAR.GENESIS_G90, CAR.GENESIS_G80] else 2

  return sys_warning, sys_state, left_lane_warning, right_lane_warning


class CarController():
  def __init__(self, dbc_name, CP, VM):
    self.p = CarControllerParams(CP)
    self.packer = CANPacker(dbc_name)

    self.apply_steer_last = 0
    self.car_fingerprint = CP.carFingerprint
    self.steer_rate_limited = False
    self.counter_init = False
    self.aq_value = 0

    self.resume_cnt = 0
    self.last_lead_distance = 0
    self.resume_wait_timer = 0

    self.last_resume_frame = 0
    self.accel = 0

    self.lanechange_manual_timer = 0
    self.emergency_manual_timer = 0
    self.driver_steering_torque_above = False
    self.driver_steering_torque_above_timer = 100
    
    self.mode_change_timer = 0

    self.acc_standstill_timer = 0
    self.acc_standstill = False

    self.need_brake = False
    self.need_brake_timer = 0

    self.cancel_counter = 0

    self.v_cruise_kph_auto_res = 0

    self.params = Params()
    self.mode_change_switch = int(self.params.get("CruiseStatemodeSelInit", encoding="utf8"))
    self.opkr_variablecruise = self.params.get_bool("OpkrVariableCruise")
    self.opkr_autoresume = self.params.get_bool("OpkrAutoResume")
    self.opkr_cruisegap_auto_adj = self.params.get_bool("CruiseGapAdjust")
    self.opkr_cruise_auto_res = self.params.get_bool("CruiseAutoRes")
    self.opkr_cruise_auto_res_option = int(self.params.get("AutoResOption", encoding="utf8"))
    self.opkr_cruise_auto_res_condition = int(self.params.get("AutoResCondition", encoding="utf8"))

    self.opkr_turnsteeringdisable = self.params.get_bool("OpkrTurnSteeringDisable")
    self.steer_wind_down_enabled = self.params.get_bool("SteerWindDown")
    self.opkr_maxanglelimit = float(int(self.params.get("OpkrMaxAngleLimit", encoding="utf8")))
    self.mad_mode_enabled = self.params.get_bool("MadModeEnabled")
    self.ldws_fix = self.params.get_bool("LdwsCarFix")
    self.radar_helper_option = int(self.params.get("RadarLongHelper", encoding="utf8"))
    self.stopping_dist_adj_enabled = self.params.get_bool("StoppingDistAdj")
    self.standstill_resume_alt = self.params.get_bool("StandstillResumeAlt")
    self.auto_res_delay = int(self.params.get("AutoRESDelay", encoding="utf8")) * 100
    self.auto_res_delay_timer = 0

    self.longcontrol = CP.openpilotLongitudinalControl
    #self.scc_live is true because CP.radarOffCan is False
    self.scc_live = not CP.radarOffCan

    self.timer1 = tm.CTime1000("time")

    self.NC = NaviControl(self.p)

    self.dRel = 0
    self.vRel = 0
    self.yRel = 0

    self.cruise_gap_prev = 0
    self.cruise_gap_set_init = 0
    self.cruise_gap_adjusting = False
    self.standstill_fault_reduce_timer = 0
    self.standstill_res_button = False
    self.standstill_res_count = int(self.params.get("RESCountatStandstill", encoding="utf8"))

    self.standstill_status = 0
    self.standstill_status_timer = 0
    self.switch_timer = 0
    self.auto_res_timer = 0
    self.auto_res_limit_timer = 0
    self.auto_res_limit_sec = int(self.params.get("AutoResLimitTime", encoding="utf8")) * 100
    self.auto_res_starting = False
    self.res_speed = 0
    self.res_speed_timer = 0
    self.autohold_popup_timer = 0
    self.autohold_popup_switch = False

    self.steerMax_base = int(self.params.get("SteerMaxBaseAdj", encoding="utf8"))
    self.steerDeltaUp_base = int(self.params.get("SteerDeltaUpBaseAdj", encoding="utf8"))
    self.steerDeltaDown_base = int(self.params.get("SteerDeltaDownBaseAdj", encoding="utf8"))
    self.model_speed_range = [30, 100, 255]
    self.steerMax_range = [self.p.STEER_MAX, self.steerMax_base, self.steerMax_base]
    self.steerDeltaUp_range = [self.p.STEER_DELTA_UP, self.steerDeltaUp_base, self.steerDeltaUp_base]
    self.steerDeltaDown_range = [self.p.STEER_DELTA_DOWN, self.steerDeltaDown_base, self.steerDeltaDown_base]
    self.steerMax = 0
    self.steerDeltaUp = 0
    self.steerDeltaDown = 0
    self.steer_wind_down = 0

    self.variable_steer_max = self.params.get_bool("OpkrVariableSteerMax")
    self.variable_steer_delta = self.params.get_bool("OpkrVariableSteerDelta")

    self.cc_timer = 0
    self.on_speed_control = False
    self.curv_speed_control = False
    self.vFuture = 0
    self.cruise_init = False

    if CP.lateralTuning.which() == 'pid':
      self.str_log2 = 'T={:0.2f}/{:0.3f}/{:0.2f}/{:0.5f}'.format(CP.lateralTuning.pid.kpV[1], CP.lateralTuning.pid.kiV[1], CP.lateralTuning.pid.kdV[0], CP.lateralTuning.pid.kf)
    elif CP.lateralTuning.which() == 'indi':
      self.str_log2 = 'T={:03.1f}/{:03.1f}/{:03.1f}/{:03.1f}'.format(CP.lateralTuning.indi.innerLoopGainV[0], CP.lateralTuning.indi.outerLoopGainV[0], \
       CP.lateralTuning.indi.timeConstantV[0], CP.lateralTuning.indi.actuatorEffectivenessV[0])
    elif CP.lateralTuning.which() == 'lqr':
      self.str_log2 = 'T={:04.0f}/{:05.3f}/{:07.5f}'.format(CP.lateralTuning.lqr.scale, CP.lateralTuning.lqr.ki, CP.lateralTuning.lqr.dcGain)

    self.sm = messaging.SubMaster(['controlsState', 'radarState'])

  def update(self, enabled, CS, frame, actuators, pcm_cancel_cmd, visual_alert,
             left_lane, right_lane, left_lane_depart, right_lane_depart, set_speed, lead_visible, v_future):

    param = self.p

    self.vFuture = v_future
    path_plan = self.NC.update_lateralPlan()
    if frame % 10 == 0:
      self.model_speed = path_plan.modelSpeed

    self.sm.update(0)
    self.dRel = self.sm['radarState'].leadOne.dRel #EON Lead
    self.vRel = self.sm['radarState'].leadOne.vRel #EON Lead
    self.yRel = self.sm['radarState'].leadOne.yRel #EON Lead

    if CS.out.vEgo > 8:
      if self.variable_steer_max:
        self.steerMax = interp(int(abs(self.model_speed)), self.model_speed_range, self.steerMax_range)
      else:
        self.steerMax = self.steerMax_base
      if self.variable_steer_delta:
        self.steerDeltaUp = interp(int(abs(self.model_speed)), self.model_speed_range, self.steerDeltaUp_range)
        self.steerDeltaDown = interp(int(abs(self.model_speed)), self.model_speed_range, self.steerDeltaDown_range)
      else:
        self.steerDeltaUp = self.steerDeltaUp_base
        self.steerDeltaDown = self.steerDeltaDown_base
    else:
      self.steerMax = self.steerMax_base
      self.steerDeltaUp = self.steerDeltaUp_base
      self.steerDeltaDown = self.steerDeltaDown_base

    self.p.STEER_MAX = min(self.p.STEER_MAX, self.steerMax) # variable steermax
    self.p.STEER_DELTA_UP = min(self.p.STEER_DELTA_UP, self.steerDeltaUp) # variable deltaUp
    self.p.STEER_DELTA_DOWN = min(self.p.STEER_DELTA_DOWN, self.steerDeltaDown) # variable deltaDown

    # Steering Torque
    if 0 <= self.driver_steering_torque_above_timer < 100:
      new_steer = int(round(actuators.steer * self.steerMax * (self.driver_steering_torque_above_timer / 100)))
    else:
      new_steer = int(round(actuators.steer * self.steerMax))
    apply_steer = apply_std_steer_torque_limits(new_steer, self.apply_steer_last, CS.out.steeringTorque, self.p)
    self.steer_rate_limited = new_steer != apply_steer

    # disable when temp fault is active, or below LKA minimum speed
    if self.opkr_maxanglelimit >= 90 and not self.steer_wind_down_enabled:
      lkas_active = enabled and abs(CS.out.steeringAngleDeg) < self.opkr_maxanglelimit and CS.out.gearShifter == GearShifter.drive
    else:
      lkas_active = enabled and not CS.out.steerWarning and CS.out.gearShifter == GearShifter.drive


    if (( CS.out.leftBlinker and not CS.out.rightBlinker) or ( CS.out.rightBlinker and not CS.out.leftBlinker)) and CS.out.vEgo < LANE_CHANGE_SPEED_MIN and self.opkr_turnsteeringdisable:
      self.lanechange_manual_timer = 50
    if CS.out.leftBlinker and CS.out.rightBlinker:
      self.emergency_manual_timer = 50
    if self.lanechange_manual_timer:
      lkas_active = 0
    if self.lanechange_manual_timer > 0:
      self.lanechange_manual_timer -= 1
    if self.emergency_manual_timer > 0:
      self.emergency_manual_timer -= 1

    if abs(CS.out.steeringTorque) > 170 and CS.out.vEgo < LANE_CHANGE_SPEED_MIN:
      self.driver_steering_torque_above = True
    else:
      self.driver_steering_torque_above = False

    if self.driver_steering_torque_above == True:
      self.driver_steering_torque_above_timer -= 1
      if self.driver_steering_torque_above_timer <= 0:
        self.driver_steering_torque_above_timer = 0
    elif self.driver_steering_torque_above == False:
      self.driver_steering_torque_above_timer += 5
      if self.driver_steering_torque_above_timer >= 100:
        self.driver_steering_torque_above_timer = 100

    if not lkas_active:
      apply_steer = 0
      if self.apply_steer_last != 0:
        self.steer_wind_down = 1
    if lkas_active or CS.out.steeringPressed:
      self.steer_wind_down = 0

    self.apply_steer_last = apply_steer

    if CS.cruise_active and CS.lead_distance > 149 and self.dRel < ((CS.out.vEgo * CV.MS_TO_KPH)+5) < 100 and \
     self.vRel*3.6 < -(CS.out.vEgo * CV.MS_TO_KPH * 0.16) and CS.out.vEgo > 7 and abs(CS.out.steeringAngleDeg) < 10 and not self.longcontrol:
      self.need_brake_timer += 1
      if self.need_brake_timer > 50:
        self.need_brake = True
    else:
      self.need_brake = False
      self.need_brake_timer = 0

    sys_warning, sys_state, left_lane_warning, right_lane_warning =\
      process_hud_alert(lkas_active, self.car_fingerprint, visual_alert,
                        left_lane, right_lane, left_lane_depart, right_lane_depart)

    clu11_speed = CS.clu11["CF_Clu_Vanz"]
    enabled_speed = 38 if CS.is_set_speed_in_mph else 60
    if clu11_speed > enabled_speed or not lkas_active or CS.out.gearShifter != GearShifter.drive:
      enabled_speed = clu11_speed

    can_sends = []
    can_sends.append(create_lkas11(self.packer, frame, self.car_fingerprint, apply_steer, lkas_active,
                                   self.steer_wind_down, CS.lkas11, sys_warning, sys_state, enabled, left_lane, right_lane,
                                   left_lane_warning, right_lane_warning, 0, self.ldws_fix, self.steer_wind_down_enabled))

    if CS.CP.mdpsBus: # send lkas11 bus 1 if mdps is bus 1
      can_sends.append(create_lkas11(self.packer, frame, self.car_fingerprint, apply_steer, lkas_active,
                                   self.steer_wind_down, CS.lkas11, sys_warning, sys_state, enabled, left_lane, right_lane,
                                   left_lane_warning, right_lane_warning, 1, self.ldws_fix, self.steer_wind_down_enabled))
      if frame % 2: # send clu11 to mdps if it is not on bus 0
        can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.NONE, enabled_speed, CS.CP.mdpsBus))

    if CS.out.cruiseState.modeSel == 0 and self.mode_change_switch == 5:
      self.mode_change_timer = 50
      self.mode_change_switch = 0
    elif CS.out.cruiseState.modeSel == 1 and self.mode_change_switch == 0:
      self.mode_change_timer = 50
      self.mode_change_switch = 1
    elif CS.out.cruiseState.modeSel == 2 and self.mode_change_switch == 1:
      self.mode_change_timer = 50
      self.mode_change_switch = 2
    elif CS.out.cruiseState.modeSel == 3 and self.mode_change_switch == 2:
      self.mode_change_timer = 50
      self.mode_change_switch = 3
    elif CS.out.cruiseState.modeSel == 4 and self.mode_change_switch == 3:
      self.mode_change_timer = 50
      self.mode_change_switch = 4
    elif CS.out.cruiseState.modeSel == 5 and self.mode_change_switch == 4:
      self.mode_change_timer = 50
      self.mode_change_switch = 5
    if self.mode_change_timer > 0:
      self.mode_change_timer -= 1

    if pcm_cancel_cmd and self.longcontrol:
      can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.CANCEL, clu11_speed, CS.CP.sccBus))

    if CS.out.cruiseState.standstill:
      self.standstill_status = 1
      if self.opkr_autoresume:
        # run only first time when the car stopped
        if self.last_lead_distance == 0:
          # get the lead distance from the Radar
          self.last_lead_distance = CS.lead_distance
          self.resume_cnt = 0
          self.switch_timer = 0
          self.standstill_fault_reduce_timer += 1
        elif self.switch_timer > 0:
          self.switch_timer -= 1
          self.standstill_fault_reduce_timer += 1
        # at least 0.1 sec delay after entering the standstill
        elif 10 < self.standstill_fault_reduce_timer and CS.lead_distance != self.last_lead_distance:
          self.acc_standstill_timer = 0
          self.acc_standstill = False
          if self.standstill_resume_alt: # for D.Fyffe, code from neokii
            self.standstill_res_button = True
            can_sends.append(create_clu11(self.packer, self.resume_cnt, CS.clu11, Buttons.RES_ACCEL, clu11_speed, CS.CP.sccBus))
            self.resume_cnt += 1
            if self.resume_cnt >= randint(6, 8):
              self.resume_cnt = 0
              self.switch_timer = randint(30, 36)
          else:
            if (frame - self.last_resume_frame) * DT_CTRL > 0.1:
              self.standstill_res_button = True
              # send 25 messages at a time to increases the likelihood of resume being accepted, value 25 is not acceptable at some cars.
              can_sends.extend([create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL)] * self.standstill_res_count) if not self.longcontrol \
              else can_sends.extend([create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL, clu11_speed, CS.CP.sccBus)] * self.standstill_res_count)
              self.last_resume_frame = frame
          self.standstill_fault_reduce_timer += 1
        # gap save after 1sec
        elif 100 < self.standstill_fault_reduce_timer and self.cruise_gap_prev == 0 and CS.cruiseGapSet != 1.0 and self.opkr_autoresume and self.opkr_cruisegap_auto_adj: 
          self.cruise_gap_prev = CS.cruiseGapSet
          self.cruise_gap_set_init = 1
        # gap adjust to 1 for fast start
        elif 110 < self.standstill_fault_reduce_timer and CS.cruiseGapSet != 1.0 and self.opkr_autoresume and self.opkr_cruisegap_auto_adj:
          can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.GAP_DIST)) if not self.longcontrol \
            else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.GAP_DIST, clu11_speed, CS.CP.sccBus))
          self.resume_cnt += 1
          if self.resume_cnt > 5:
            self.resume_cnt = 0
            self.switch_timer = randint(10, 15)
          self.cruise_gap_adjusting = True
        elif self.opkr_autoresume:
          self.cruise_gap_adjusting = False
          self.standstill_res_button = False
          self.standstill_fault_reduce_timer += 1
    # reset lead distnce after the car starts moving
    elif self.last_lead_distance != 0:
      self.last_lead_distance = 0
      self.standstill_res_button = False
    elif self.opkr_variablecruise and CS.acc_active and CS.out.cruiseState.modeSel > 0:
      self.on_speed_control = self.NC.onSpeedControl
      self.curv_speed_control = self.NC.curvSpeedControl
      btn_signal = self.NC.update(CS, path_plan)
      if self.opkr_cruisegap_auto_adj:
        # gap restore
        if self.switch_timer > 0:
          self.switch_timer -= 1
        elif self.dRel > 17 and self.vRel*3.6 < 5 and self.cruise_gap_prev != CS.cruiseGapSet and self.cruise_gap_set_init == 1 and self.opkr_autoresume:
          can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.GAP_DIST)) if not self.longcontrol \
            else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.GAP_DIST, clu11_speed, CS.CP.sccBus))
          self.cruise_gap_adjusting = True
          self.resume_cnt += 1
          if self.resume_cnt > 5:
            self.resume_cnt = 0
            self.switch_timer = randint(10, 15)
        elif self.cruise_gap_prev == CS.cruiseGapSet and CS.cruiseGapSet != 1.0 and self.opkr_autoresume:
          self.cruise_gap_set_init = 0
          self.cruise_gap_prev = 0
          self.cruise_gap_adjusting = False
        else:
          self.cruise_gap_adjusting = False
      if not self.cruise_gap_adjusting:
        if btn_signal != None:
          can_sends.append(create_clu11(self.packer, self.resume_cnt, CS.clu11, btn_signal)) if not self.longcontrol \
          else can_sends.append(create_clu11(self.packer, frame, CS.clu11, btn_signal, clu11_speed, CS.CP.sccBus))
          self.resume_cnt += 1
        else:
          self.resume_cnt = 0
    else:
      self.on_speed_control = False
      self.curv_speed_control = False
      self.cruise_gap_adjusting = False
      self.standstill_res_button = False
      self.auto_res_starting = False

    if not enabled:
      self.cruise_init = False
    if CS.cruise_buttons == 4:
      self.cancel_counter += 1
      self.auto_res_starting = False
      self.standstill_res_button = False
    elif CS.cruise_active:
      self.cruise_init = True
      self.cancel_counter = 0
      self.auto_res_limit_timer = 0
      self.auto_res_delay_timer = 0
      if self.res_speed_timer > 0:
        self.res_speed_timer -= 1
        self.auto_res_starting = False
      else:
        self.auto_res_starting = False
        self.v_cruise_kph_auto_res = 0
        self.res_speed = 0
    else:
      if self.auto_res_limit_timer < self.auto_res_limit_sec:
        self.auto_res_limit_timer += 1
      if self.auto_res_delay_timer < self.auto_res_delay:
        self.auto_res_delay_timer += 1
    if CS.brakeHold and not self.autohold_popup_switch:
      self.autohold_popup_timer = 100
      self.autohold_popup_switch = True
    elif CS.brakeHold and self.autohold_popup_switch and self.autohold_popup_timer:
      self.autohold_popup_timer -= 1
    elif not CS.brakeHold and self.autohold_popup_switch:
      self.autohold_popup_switch = False
      self.autohold_popup_timer = 0

    opkr_cruise_auto_res_condition = False
    opkr_cruise_auto_res_condition = not self.opkr_cruise_auto_res_condition or CS.out.gasPressed
    t_speed = 20 if CS.is_set_speed_in_mph else 30
    if self.auto_res_timer > 0:
      self.auto_res_timer -= 1
    elif self.model_speed > 95 and self.cancel_counter == 0 and not CS.cruise_active and not CS.out.brakeLights and int(CS.VSetDis) >= t_speed and \
     (1 < CS.lead_distance < 149 or int(CS.clu_Vanz) > t_speed) and int(CS.clu_Vanz) >= 3 and self.cruise_init and \
     self.opkr_cruise_auto_res and opkr_cruise_auto_res_condition and (self.auto_res_limit_sec == 0 or self.auto_res_limit_timer < self.auto_res_limit_sec) and \
     (self.auto_res_delay == 0 or self.auto_res_delay_timer >= self.auto_res_delay):
      if self.opkr_cruise_auto_res_option == 0:
        can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL)) if not self.longcontrol \
         else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL, clu11_speed, CS.CP.sccBus))  # auto res
        self.auto_res_starting = True
        self.res_speed = int(CS.clu_Vanz*1.1)
        self.res_speed_timer = 300
        self.resume_cnt += 1
        if self.resume_cnt > 5:
          self.resume_cnt = 0
          self.auto_res_timer = randint(10, 15)
      elif self.opkr_cruise_auto_res_option == 1:
        can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.SET_DECEL)) if not self.longcontrol \
         else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.SET_DECEL, clu11_speed, CS.CP.sccBus)) # auto res but set_decel to set current speed
        self.auto_res_starting = True
        self.v_cruise_kph_auto_res = int(CS.clu_Vanz)
        self.res_speed_timer = 50
        self.resume_cnt += 1
        if self.resume_cnt > 5:
          self.resume_cnt = 0
          self.auto_res_timer = randint(10, 15)
      elif self.opkr_cruise_auto_res_option == 2:
        if not self.longcontrol:
          can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL)) if 1 < CS.lead_distance < 149 \
           else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.SET_DECEL))
        else:
          can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.RES_ACCEL, clu11_speed, CS.CP.sccBus)) if 1 < CS.lead_distance < 149 \
           else can_sends.append(create_clu11(self.packer, frame, CS.clu11, Buttons.SET_DECEL, clu11_speed, CS.CP.sccBus))
        self.auto_res_starting = True
        self.v_cruise_kph_auto_res = int(CS.clu_Vanz)
        self.res_speed_timer = 50
        self.resume_cnt += 1
        if self.resume_cnt > 5:
          self.resume_cnt = 0
          self.auto_res_timer = randint(10, 15)

    if CS.out.brakeLights and CS.out.vEgo == 0 and not CS.cruise_active:
      self.standstill_status_timer += 1
      if self.standstill_status_timer > 200:
        self.standstill_status = 1
        self.standstill_status_timer = 0
    if self.standstill_status == 1 and CS.out.vEgo > 1:
      self.standstill_status = 0
      self.standstill_fault_reduce_timer = 0
      self.last_resume_frame = frame
      self.res_switch_timer = 0
      self.resume_cnt = 0

    if CS.out.vEgo <= 1:
      long_control_state = self.sm['controlsState'].longControlState
      if long_control_state == LongCtrlState.stopping and CS.out.vEgo < 0.1 and not CS.out.gasPressed:
        self.acc_standstill_timer += 1
        if self.acc_standstill_timer >= 200:
          self.acc_standstill_timer = 200
          self.acc_standstill = True
      else:
        self.acc_standstill_timer = 0
        self.acc_standstill = False
    elif CS.out.gasPressed or CS.out.vEgo > 1:
      self.acc_standstill = False
      self.acc_standstill_timer = 0      
    else:
      self.acc_standstill = False
      self.acc_standstill_timer = 0

    if CS.CP.mdpsBus: # send mdps12 to LKAS to prevent LKAS error
      can_sends.append(create_mdps12(self.packer, frame, CS.mdps12))

    # # tester present - w/ no response (keeps radar disabled)
    # if CS.CP.openpilotLongitudinalControl:
    #   if (frame % 100) == 0:
    #     can_sends.append([0x7D0, 0, b"\x02\x3E\x80\x00\x00\x00\x00\x00", 0])

    # if frame % 2 == 0 and CS.CP.openpilotLongitudinalControl:
    #   lead_visible = False
    #   accel = actuators.accel if enabled else 0
    #   jerk = clip(2.0 * (accel - CS.out.aEgo), -12.7, 12.7)
    #   if accel < 0:
    #     accel = interp(accel - CS.out.aEgo, [-1.0, -0.5], [2 * accel, accel])
    #   accel = clip(accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX)
    #   stopping = (actuators.longControlState == LongCtrlState.stopping)
    #   set_speed_in_units = hud_speed * (CV.MS_TO_MPH if CS.clu11["CF_Clu_SPEED_UNIT"] == 1 else CV.MS_TO_KPH)
    #   can_sends.extend(create_acc_commands(self.packer, enabled, accel, jerk, int(frame / 2), lead_visible, set_speed_in_units, stopping))

    if CS.CP.sccBus != 0 and self.counter_init and self.longcontrol:
      if frame % 2 == 0:
        self.scc12cnt += 1
        self.scc12cnt %= 0xF
        self.scc11cnt += 1
        self.scc11cnt %= 0x10
        lead_objspd = CS.lead_objspd  # vRel (km/h)
        aReqValue = CS.scc12["aReqValue"]
        accel = actuators.accel if enabled else 0
        #accel = clip(accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX)
        if 0 < CS.out.radarDistance <= 149 and self.radar_helper_option == 1:
          # neokii's logic, opkr mod
          stock_weight = 0.
          if aReqValue > 0.:
            stock_weight = interp(CS.out.radarDistance, [3.5, 25.0], [0.8, 0.0])
          elif aReqValue < 0. and self.stopping_dist_adj_enabled:
            stock_weight = interp(CS.out.radarDistance, [2.0, 4.5, 5.5, 25.0], [1.0, 0.1, 1.0, 0.0])
          elif aReqValue < 0.:
            stock_weight = interp(CS.out.radarDistance, [3.0, 25.0], [1.0, 0.0])
          else:
            stock_weight = 0.
          accel = accel * (1. - stock_weight) + aReqValue * stock_weight
        elif 0 < CS.out.radarDistance <= 149 and self.radar_helper_option == 2:
          accel = aReqValue
        elif 0 < CS.out.radarDistance <= 3.5: # use radar by force to stop anyway below 3.5m if lead car is recognized.
          stock_weight = interp(CS.out.radarDistance, [2., 3.5], [1., 0.])
          accel = accel * (1. - stock_weight) + aReqValue * stock_weight
        else:
          stock_weight = 0.
        self.aq_value = accel
        can_sends.append(create_scc11(self.packer, frame, set_speed, lead_visible, self.scc_live, self.dRel, self.vRel, self.yRel, 
         self.car_fingerprint, CS.out.vEgo * CV.MS_TO_KPH, self.acc_standstill, CS.scc11))
        if (CS.brake_check or CS.cancel_check) and self.car_fingerprint not in [CAR.NIRO_EV]:
          can_sends.append(create_scc12(self.packer, accel, enabled, self.scc_live, CS.out.gasPressed, 1, 
           CS.out.stockAeb, self.car_fingerprint, CS.out.vEgo * CV.MS_TO_KPH, CS.scc12))
        else:
          can_sends.append(create_scc12(self.packer, accel, enabled, self.scc_live, CS.out.gasPressed, CS.out.brakePressed, 
           CS.out.stockAeb, self.car_fingerprint, CS.out.vEgo * CV.MS_TO_KPH, CS.scc12))
        can_sends.append(create_scc14(self.packer, enabled, CS.scc14, CS.out.stockAeb, lead_visible, self.dRel, 
         CS.out.vEgo, self.acc_standstill, self.car_fingerprint))
        self.accel = accel
      if frame % 20 == 0:
        can_sends.append(create_scc13(self.packer, CS.scc13))
      if frame % 50 == 0:
        can_sends.append(create_scc42a(self.packer))
    elif CS.CP.sccBus == 2 and self.longcontrol:
      self.counter_init = True
      self.scc12cnt = CS.scc12init["CR_VSM_Alive"]
      self.scc11cnt = CS.scc11init["AliveCounterACC"]

    setSpeed = set_speed * CV.MS_TO_KPH
    str_log1 = 'MD={}  BS={:1.0f}/{:1.0f}  CV={:03.0f}  TQ={:03.0f}  ST={:03.0f}/{:01.0f}/{:01.0f}  FR={:03.0f}'.format(
      CS.out.cruiseState.modeSel, CS.CP.mdpsBus, CS.CP.sccBus, self.model_speed, abs(new_steer), self.p.STEER_MAX, self.p.STEER_DELTA_UP, self.p.STEER_DELTA_DOWN, self.timer1.sampleTime())
    if CS.out.cruiseState.accActive:
      str_log2 = 'AQ={:+04.2f}  VF={:03.0f}  TS={:03.0f}  SS/VS={:03.0f}/{:03.0f}  RD/LD={:04.1f}/{:03.1f}  CG={:1.0f}  FR={:03.0f}'.format(
       self.aq_value if self.longcontrol else CS.scc12["aReqValue"], v_future, self.NC.ctrl_speed , setSpeed, CS.VSetDis, CS.lead_distance, self.last_lead_distance, CS.cruiseGapSet, self.timer1.sampleTime())
    else:
      str_log2 = 'MDPS={}  LKAS={}  LEAD={}  AQ={:+04.2f}  VF={:03.0f}  CG={:1.0f}  FR={:03.0f}'.format(
       CS.out.steerWarning, CS.lkas_button_on, 0 < CS.lead_distance < 149, self.aq_value if self.longcontrol else CS.scc12["aReqValue"], v_future, CS.cruiseGapSet, self.timer1.sampleTime())
    trace1.printf2( '{}'.format( str_log2 ) )

    self.cc_timer += 1
    if self.cc_timer > 100:
      self.cc_timer = 0
      self.radar_helper_option = int(self.params.get("RadarLongHelper", encoding="utf8"))
      self.stopping_dist_adj_enabled = self.params.get_bool("StoppingDistAdj")
      self.standstill_res_count = int(self.params.get("RESCountatStandstill", encoding="utf8"))
      self.opkr_cruisegap_auto_adj = self.params.get_bool("CruiseGapAdjust")
      if self.params.get_bool("OpkrLiveTunePanelEnable"):
        if CS.CP.lateralTuning.which() == 'pid':
          self.str_log2 = 'T={:0.2f}/{:0.3f}/{:0.1f}/{:0.5f}'.format(float(Decimal(self.params.get("PidKp", encoding="utf8"))*Decimal('0.01')), \
           float(Decimal(self.params.get("PidKi", encoding="utf8"))*Decimal('0.001')), float(Decimal(self.params.get("PidKd", encoding="utf8"))*Decimal('0.01')), \
           float(Decimal(self.params.get("PidKf", encoding="utf8"))*Decimal('0.00001')))
        elif CS.CP.lateralTuning.which() == 'indi':
          self.str_log2 = 'T={:03.1f}/{:03.1f}/{:03.1f}/{:03.1f}'.format(float(Decimal(self.params.get("InnerLoopGain", encoding="utf8"))*Decimal('0.1')), \
           float(Decimal(self.params.get("OuterLoopGain", encoding="utf8"))*Decimal('0.1')), float(Decimal(self.params.get("TimeConstant", encoding="utf8"))*Decimal('0.1')), \
           float(Decimal(self.params.get("ActuatorEffectiveness", encoding="utf8"))*Decimal('0.1')))
        elif CS.CP.lateralTuning.which() == 'lqr':
          self.str_log2 = 'T={:04.0f}/{:05.3f}/{:07.5f}'.format(float(Decimal(self.params.get("Scale", encoding="utf8"))*Decimal('1.0')), \
           float(Decimal(self.params.get("LqrKi", encoding="utf8"))*Decimal('0.001')), float(Decimal(self.params.get("DcGain", encoding="utf8"))*Decimal('0.00001')))

    trace1.printf1('{}  {}'.format(str_log1, self.str_log2))

    # 20 Hz LFA MFA message
    if frame % 5 == 0 and self.car_fingerprint in FEATURES["send_lfahda_mfa"]:
      can_sends.append(create_lfahda_mfc(self.packer, lkas_active))

    elif frame % 5 == 0 and self.car_fingerprint in FEATURES["send_hda_mfa"]:
      can_sends.append(create_hda_mfc(self.packer, CS, enabled, left_lane, right_lane ))

    new_actuators = actuators.copy()
    new_actuators.steer = apply_steer / self.p.STEER_MAX
    new_actuators.accel = self.accel

    return new_actuators, can_sends
