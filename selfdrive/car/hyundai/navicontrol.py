# created by atom
import math
import numpy as np

from selfdrive.config import Conversions as CV
from selfdrive.car.hyundai.values import Buttons
from common.numpy_fast import clip, interp
from cereal import log
import cereal.messaging as messaging
from common.params import Params

import common.log as trace1
import common.MoveAvg as mvAvg

LaneChangeState = log.LateralPlan.LaneChangeState

class NaviControl():
  def __init__(self, p=None):
    self.p = p
    
    self.sm = messaging.SubMaster(['liveNaviData', 'lateralPlan', 'radarState', 'controlsState', 'liveMapData'])

    self.btn_cnt = 0
    self.seq_command = 0
    self.target_speed = 0
    self.set_point = 0
    self.wait_timer2 = 0
    self.wait_timer3 = 0

    self.moveAvg = mvAvg.MoveAvg()

    self.gasPressed_old = 0

    self.map_spdlimit_offset = int(Params().get("OpkrSpeedLimitOffset", encoding="utf8"))
    self.map_spdlimit_offset_option = int(Params().get("OpkrSpeedLimitOffsetOption", encoding="utf8"))
    self.safetycam_decel_dist_gain = int(Params().get("SafetyCamDecelDistGain", encoding="utf8"))

    self.map_speed_block = False
    self.map_speed_dist = 0
    self.map_speed = 0
    self.onSpeedControl = False
    self.curvSpeedControl = False
    self.ctrl_speed = 0
    self.vision_curv_speed = [int(Params().get("VCurvSpeed30", encoding="utf8")), int(Params().get("VCurvSpeed50", encoding="utf8")),
     int(Params().get("VCurvSpeed70", encoding="utf8")), int(Params().get("VCurvSpeed90", encoding="utf8"))]
    self.osm_curv_speed_offset = int(Params().get("OCurvOffset", encoding="utf8"))
    self.osm_wait_timer = 0
    self.stock_navi_info_enabled = Params().get_bool("StockNaviSpeedEnabled")
    self.osm_speedlimit_enabled = Params().get_bool("OSMSpeedLimitEnable")

  def update_lateralPlan(self):
    self.sm.update(0)
    path_plan = self.sm['lateralPlan']
    return path_plan

  def button_status(self, CS):
    if not CS.cruise_active or CS.cruise_buttons != Buttons.NONE: 
      self.wait_timer2 = 80 
    elif self.wait_timer2: 
      self.wait_timer2 -= 1
    else:
      return 1
    return 0

  # buttn acc,dec control
  def switch(self, seq_cmd):
      self.case_name = "case_" + str(seq_cmd)
      self.case_func = getattr( self, self.case_name, lambda:"default")
      return self.case_func()

  def reset_btn(self):
      if self.seq_command != 3:
        self.seq_command = 0

  def case_default(self):
      self.seq_command = 0
      return None

  def case_0(self):
      self.btn_cnt = 0
      self.target_speed = self.set_point
      delta_speed = round(self.target_speed - self.VSetDis)
      if delta_speed > 0:
        self.seq_command = 1  # case_1 번으로 이동.
      elif delta_speed < 0:
        self.seq_command = 2  # case_2 번으로 이동.
      return None

  def case_1(self):  # acc
      btn_signal = Buttons.RES_ACCEL
      self.btn_cnt += 1
      if self.target_speed == self.VSetDis:
        self.btn_cnt = 0
        self.seq_command = 3        # case_3 번으로 이동.    
      elif self.btn_cnt > 5:
        self.btn_cnt = 0
        self.seq_command = 3        # case_3 번으로 이동.
      return btn_signal

  def case_2(self):  # dec
      btn_signal = Buttons.SET_DECEL
      self.btn_cnt += 1
      if self.target_speed == self.VSetDis:
        self.btn_cnt = 0
        self.seq_command = 3      # case_3 번으로 이동.      
      elif self.btn_cnt > 5:
        self.btn_cnt = 0
        self.seq_command = 3# case_3 번으로 이동.
      return btn_signal

  def case_3(self):  # None  버튼 off 유지시간. 크르즈 속도제어오류 발생시 아래의 수치를 조금 변경하여 보십시오. 수치가 크면 속도변경이 느려지고, 작으면 빨라집니다.
      btn_signal = None  # Buttons.NONE
      
      self.btn_cnt += 1
      #if self.btn_cnt == 1:
      #  btn_signal = Buttons.NONE
      if self.btn_cnt > 7:    # 버튼 클릭후 일정시간 기다린다.  (반드시 필요함)
        self.seq_command = 0   # case_0 번으로 이동.  (다음 명령을 실행) 
      return btn_signal

  def ascc_button_control(self, CS, set_speed):
    self.set_point = max(20 if CS.is_set_speed_in_mph else 30, set_speed)
    self.curr_speed = CS.out.vEgo * CV.MS_TO_KPH
    self.VSetDis = round(CS.VSetDis)
    btn_signal = self.switch(self.seq_command)

    return btn_signal

  def get_navi_speed(self, sm, CS, cruiseState_speed):
    cruise_set_speed_kph = cruiseState_speed
    v_ego_kph = CS.out.vEgo * CV.MS_TO_KPH
    self.liveNaviData = sm['liveNaviData']
    # speedLimit = self.liveNaviData.speedLimit
    # speedLimitDistance = self.liveNaviData.speedLimitDistance  #speedLimitDistance
    # safetySign = self.liveNaviData.safetySign
    #mapValid = self.liveNaviData.mapValid
    #trafficType = self.liveNaviData.trafficType
    
    #if not mapValid or trafficType == 0:
    #  return  cruise_set_speed_kph

    if int(self.sm['liveMapData'].speedLimit) > 19 and self.osm_speedlimit_enabled:  # osm speedlimit
      self.onSpeedControl = True
      spdTarget = self.sm['liveMapData'].speedLimit
      if self.map_spdlimit_offset_option == 0:
        cruise_set_speed_kph = spdTarget + round(spdTarget*0.01*self.map_spdlimit_offset)
      else:
        cruise_set_speed_kph = spdTarget + self.map_spdlimit_offset
    elif CS.map_enabled and self.liveNaviData.speedLimit > 19:  # mappy speedlimit
      self.map_speed_dist = max(0, self.liveNaviData.speedLimitDistance - 30)
      self.map_speed = self.liveNaviData.speedLimit
      if self.map_speed_dist > 1250:
        self.map_speed_block = True
      else:
        self.map_speed_block = False
      cam_distance_calc = 0
      cam_distance_calc = interp(v_ego_kph, [30, 60, 110], [2.6, 3.1, 3.9]) if CS.CP.sccBus == 0 else interp(v_ego_kph, [30, 60, 110], [2.5, 3.0, 3.8])
      consider_speed = interp((v_ego_kph - self.map_speed), [0, 50], [1, 2])
      min_control_dist = interp(self.map_speed, [30, 110], [40, 250])
      final_cam_decel_start_dist = cam_distance_calc*consider_speed*v_ego_kph * (1 + self.safetycam_decel_dist_gain*0.01)
      if self.map_speed_dist < final_cam_decel_start_dist:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.map_speed_dist >= final_cam_decel_start_dist and self.map_speed_block:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.map_speed_dist < min_control_dist:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.onSpeedControl and self.map_speed > 19:
        spdTarget = self.map_speed
      else:
        self.onSpeedControl = False
        return cruise_set_speed_kph
      if self.map_spdlimit_offset_option == 0:
        cruise_set_speed_kph = spdTarget + round(spdTarget*0.01*self.map_spdlimit_offset)
      else:
        cruise_set_speed_kph = spdTarget + self.map_spdlimit_offset
    elif CS.safety_sign > 19 and self.stock_navi_info_enabled:  # cat stock navi speedlimit
      self.map_speed_dist = max(0, CS.safety_dist - 30)
      self.map_speed = CS.safety_sign
      if CS.safety_block_remain_dist < 255:
        self.map_speed_block = True
      else:
        self.map_speed_block = False
      cam_distance_calc = 0
      cam_distance_calc = interp(v_ego_kph, [30, 60, 110], [2.6, 3.1, 3.9])  if CS.CP.sccBus == 0 else interp(v_ego_kph, [30, 60, 110], [2.5, 3.0, 3.8])
      consider_speed = interp((v_ego_kph - (self.map_speed * (CV.MPH_TO_KPH if CS.is_set_speed_in_mph else 1))), [0, 50], [1, 2])
      min_control_dist = interp(self.map_speed, [30, 110], [40, 250])
      final_cam_decel_start_dist = cam_distance_calc*consider_speed*v_ego_kph * (1 + self.safetycam_decel_dist_gain*0.01)
      if self.map_speed_dist < final_cam_decel_start_dist:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.map_speed_dist >= final_cam_decel_start_dist and self.map_speed_block:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.map_speed_dist < min_control_dist:
        spdTarget = self.map_speed
        self.onSpeedControl = True
      elif self.onSpeedControl and self.map_speed > 19:
        spdTarget = self.map_speed
      else:
        self.onSpeedControl = False
        return cruise_set_speed_kph
      if self.map_spdlimit_offset_option == 0:
        cruise_set_speed_kph = spdTarget + round(spdTarget*0.01*self.map_spdlimit_offset)
      else:
        cruise_set_speed_kph = spdTarget + self.map_spdlimit_offset
    else:
      spdTarget = cruise_set_speed_kph
      self.onSpeedControl = False
      self.map_speed = 0
      self.map_speed_dist = 0
      self.map_speed_block = False

    # elif speedLimitDistance >= 50:
    #   if speedLimit <= 60:
    #     spdTarget = interp(speedLimitDistance, [50, 600], [ speedLimit, speedLimit + 50 ])
    #   else:
    #     spdTarget = interp(speedLimitDistance, [150, 900], [ speedLimit, speedLimit + 30 ])
    # else:
    #   spdTarget = speedLimit

    # if v_ego_kph < speedLimit:
    #   v_ego_kph = speedLimit

    # print('cruise_set_speed_kph={}'.format(cruise_set_speed_kph))

    return cruise_set_speed_kph

  def auto_speed_control(self, CS, navi_speed, path_plan):
    modelSpeed = path_plan.modelSpeed
    min_control_speed = 20 if CS.is_set_speed_in_mph else 30
    self.lead_0 = self.sm['radarState'].leadOne
    self.lead_1 = self.sm['radarState'].leadTwo

    if CS.driverAcc_time:
      return min(CS.clu_Vanz + 3, navi_speed)
    # elif self.gasPressed_old:
    #   clu_Vanz = CS.clu_Vanz
    #   ctrl_speed = max(min_control_speed, ctrl_speed, clu_Vanz)
    #   CS.set_cruise_speed(ctrl_speed)
    elif CS.CP.resSpeed > 20:
      res_speed = max(min_control_speed, CS.CP.resSpeed)
      return min(res_speed, navi_speed)
    elif CS.cruise_set_mode in [1,2,4]:
      if self.lead_0.status and CS.CP.vFuture >= min_control_speed-7:
        dRel = int(self.lead_0.dRel)
        vRel = int(self.lead_0.vRel * CV.MS_TO_KPH)
        if vRel >= -5:
          var_speed = min(CS.CP.vFuture + max(0, dRel*0.2+vRel), navi_speed)
        else:
          var_speed = min(CS.CP.vFuture, navi_speed)
      elif self.lead_0.status and CS.CP.vFuture < min_control_speed:
        var_speed = min(CS.CP.vFuture, navi_speed)
      else:
        var_speed = navi_speed
    else:
      var_speed = navi_speed

    if CS.cruise_set_mode in [1,3,4] and CS.out.vEgo * CV.MS_TO_KPH > 40 and modelSpeed < 90 and \
     path_plan.laneChangeState == LaneChangeState.off and not (CS.out.leftBlinker or CS.out.rightBlinker) and not abs(CS.out.steeringTorque) > 170:
      v_curv_speed = min(var_speed, interp(modelSpeed, [30, 50, 70, 90], self.vision_curv_speed)) # curve speed ratio
    else:
      v_curv_speed = navi_speed

    if CS.cruise_set_mode in [1,3,4]:
      if self.sm['liveMapData'].turnSpeedLimitEndDistance > 30:
        o_curv_speed = self.sm['liveMapData'].turnSpeedLimit * (1 + (self.osm_curv_speed_offset*0.01))
        self.osm_wait_timer += 1 if modelSpeed > 90 else 0
        if self.osm_wait_timer > 100:
          o_curv_speed = 255
      else:
        o_curv_speed = 255
        self.osm_wait_timer = 0
    else:
      o_curv_speed = 255
      self.osm_wait_timer = 0

    # self.gasPressed_old = CS.gasPressed
    if var_speed > min(v_curv_speed, o_curv_speed):
      self.curvSpeedControl = True
    else:
      self.curvSpeedControl = False
    return min(var_speed, v_curv_speed, o_curv_speed)

  def update(self, CS, path_plan):
    btn_signal = None
    if not self.button_status(CS):  # 사용자가 버튼클릭하면 일정시간 기다린다.
      pass
    elif CS.cruise_active:
      cruiseState_speed = self.sm['controlsState'].vCruise
      kph_set_vEgo = self.get_navi_speed(self.sm, CS, cruiseState_speed) # camspeed
      navi_speed = min(cruiseState_speed, kph_set_vEgo)

      if CS.cruise_set_mode != 5:
        self.ctrl_speed = self.auto_speed_control(CS, navi_speed, path_plan) # lead, curve speed
      else:
        self.ctrl_speed = navi_speed # navi speed

      # print('self.ctrl_speed={}  cruiseState_speed={}'.format(self.ctrl_speed, cruiseState_speed))      

      btn_signal = self.ascc_button_control(CS, self.ctrl_speed)

    return btn_signal