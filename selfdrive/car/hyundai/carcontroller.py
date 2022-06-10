from random import randint

from cereal import car
from common.realtime import DT_CTRL
from common.numpy_fast import clip, interp
from selfdrive.car import apply_std_steer_torque_limits
from selfdrive.car.hyundai.hyundaican import create_lkas11, create_clu11, \
  create_scc11, create_scc12, create_scc13, create_scc14, \
  create_mdps12, create_lfahda_mfc, create_hda_mfc
from selfdrive.car.hyundai.scc_smoother import SccSmoother
from selfdrive.car.hyundai.values import Buttons, CAR, FEATURES, CarControllerParams
from opendbc.can.packer import CANPacker
from selfdrive.config import Conversions as CV
from common.params import Params
from selfdrive.controls.lib.longcontrol import LongCtrlState
from selfdrive.road_speed_limiter import road_speed_limiter_get_active

VisualAlert = car.CarControl.HUDControl.VisualAlert
min_set_speed = 30 * CV.KPH_TO_MS

SP_CARS = (CAR.GENESIS, CAR.GENESIS_G70, CAR.GENESIS_G80,
           CAR.GENESIS_EQ900, CAR.GENESIS_EQ900_L, CAR.K9, CAR.GENESIS_G90)

def process_hud_alert(enabled, fingerprint, visual_alert, left_lane, right_lane,
                      left_lane_depart, right_lane_depart):

  sys_warning = (visual_alert in (VisualAlert.steerRequired, VisualAlert.ldw))

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
    left_lane_warning = 1 if fingerprint in SP_CARS else 2
  if right_lane_depart:
    right_lane_warning = 1 if fingerprint in SP_CARS else 2

  return sys_warning, sys_state, left_lane_warning, right_lane_warning


class CarController():
  def __init__(self, dbc_name, CP, VM):
    self.car_fingerprint = CP.carFingerprint
    self.packer = CANPacker(dbc_name)
    self.apply_steer_last = 0
    self.accel = 0
    self.lkas11_cnt = 0

    self.resume_cnt = 0
    self.last_lead_distance = 0
    self.resume_wait_timer = 0

    self.turning_signal_timer = 0
    self.longcontrol = CP.openpilotLongitudinalControl
    self.scc_live = not CP.radarOffCan

    self.turning_indicator_alert = False

    param = Params()

    self.mad_mode_enabled = param.get_bool('MadModeEnabled')
    self.keep_steering_turn_signals = param.get_bool('KeepSteeringTurnSignals')
    self.warning_over_speed_limit = param.get_bool('WarningOverSpeedLimit')

    self.scc_smoother = SccSmoother()
    self.last_blinker_frame = 0

  def update(self, enabled, CS, frame, CC, actuators, pcm_cancel_cmd, visual_alert,
             left_lane, right_lane, left_lane_depart, right_lane_depart, set_speed, lead_visible, controls):


    # Steering Torque
    new_steer = int(round(actuators.steer * CarControllerParams.STEER_MAX))
    apply_steer = apply_std_steer_torque_limits(new_steer, self.apply_steer_last, CS.out.steeringTorque,
                                                CarControllerParams)

    # disable if steer angle reach 90 deg, otherwise mdps fault in some models
    lkas_active = enabled and not CS.out.steerWarning and abs(CS.out.steeringAngleDeg) < CS.CP.maxSteeringAngleDeg

    # Disable steering while turning blinker on and speed below 60 kph
    if CS.out.leftBlinker or CS.out.rightBlinker:
      self.turning_signal_timer = 0.5 / DT_CTRL  # Disable for 0.5 Seconds after blinker turned off
    if self.turning_indicator_alert: # set and clear by interface
      lkas_active = 0
    if self.turning_signal_timer > 0:
      self.turning_signal_timer -= 1

    if not lkas_active:
      apply_steer = 0

    self.apply_steer_last = apply_steer

    if self.warning_over_speed_limit:
      recent_blinker = (controls.sm.frame - self.last_blinker_frame) * DT_CTRL < 5.0
      if not recent_blinker and self.scc_smoother.over_speed_limit:
        left_lane_depart = True
        self.last_blinker_frame = controls.sm.frame

    sys_warning, sys_state, left_lane_warning, right_lane_warning = \
      process_hud_alert(enabled, self.car_fingerprint, visual_alert,
                        left_lane, right_lane, left_lane_depart, right_lane_depart)

    clu11_speed = CS.clu11["CF_Clu_Vanz"]
    enabled_speed = 38 if CS.is_set_speed_in_mph else 60
    if clu11_speed > enabled_speed or not lkas_active:
      enabled_speed = clu11_speed

    if not (min_set_speed < set_speed < 255 * CV.KPH_TO_MS):
      set_speed = min_set_speed
    set_speed *= CV.MS_TO_MPH if CS.is_set_speed_in_mph else CV.MS_TO_KPH

    if frame == 0:  # initialize counts from last received count signals
      self.lkas11_cnt = CS.lkas11["CF_Lkas_MsgCount"]

    self.prev_scc_cnt = CS.scc11["AliveCounterACC"]

    self.lkas11_cnt = (self.lkas11_cnt + 1) % 0x10

    can_sends = []
    can_sends.append(create_lkas11(self.packer, frame, self.car_fingerprint, apply_steer, lkas_active,
                                   CS.lkas11, sys_warning, sys_state, enabled, left_lane, right_lane,
                                   left_lane_warning, right_lane_warning, 0))

    # if pcm_cancel_cmd and (self.longcontrol and not self.mad_mode_enabled):
    #   can_sends.append(create_clu11(self.packer, frame % 0x10, CS.scc_bus, CS.clu11, Buttons.CANCEL, clu11_speed))
    # else:
    #   can_sends.append(create_mdps12(self.packer, frame, CS.mdps12))

    can_sends.append(create_mdps12(self.packer, frame, CS.mdps12))  

    # fix auto resume - by neokii
    if CS.out.cruiseState.standstill and not CS.out.gasPressed:

      if self.last_lead_distance == 0:
        self.last_lead_distance = CS.lead_distance
        self.resume_cnt = 0
        self.resume_wait_timer = 0

      # scc smoother
      elif self.scc_smoother.is_active(frame):
        pass

      elif self.resume_wait_timer > 0:
        self.resume_wait_timer -= 1

      elif abs(CS.lead_distance - self.last_lead_distance) > 0.1:
        # can_sends.append(create_clu11(self.packer, self.resume_cnt, CS.scc_bus, CS.clu11, Buttons.RES_ACCEL, clu11_speed))
        can_sends.append(create_clu11(self.packer, self.resume_cnt, 0, CS.clu11, Buttons.RES_ACCEL, clu11_speed))
        self.resume_cnt += 1

        if self.resume_cnt >= randint(6, 8):
          self.resume_cnt = 0
          self.resume_wait_timer = randint(30, 36)

    # reset lead distnce after the car starts moving
    elif self.last_lead_distance != 0:
      self.last_lead_distance = 0

    # scc smoother
    self.scc_smoother.update(enabled, can_sends, self.packer, CC, CS, frame, controls)

    # 20 Hz LFA MFA message
    if frame % 5 == 0:
      activated_hda = road_speed_limiter_get_active()
      # activated_hda: 0 - off, 1 - main road, 2 - highway
      if self.car_fingerprint in FEATURES["send_lfa_mfa"]:
        can_sends.append(create_lfahda_mfc(self.packer, enabled, activated_hda))
      # elif CS.mdps_bus == 0:
      #   state = 2 if self.car_fingerprint in FEATURES["send_hda_state_2"] else 1
      #   can_sends.append(create_hda_mfc(self.packer, activated_hda, state))

    new_actuators = actuators.copy()
    new_actuators.steer = apply_steer / CarControllerParams.STEER_MAX
    new_actuators.accel = self.accel

    return new_actuators, can_sends
