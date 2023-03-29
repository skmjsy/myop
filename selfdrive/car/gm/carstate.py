from cereal import car
from common.numpy_fast import mean
from opendbc.can.can_define import CANDefine
from opendbc.can.parser import CANParser
from selfdrive.car.interfaces import CarStateBase
from selfdrive.car.gm.values import DBC, CAR, AccState, CanBus, \
                                    CruiseButtons, STEER_THRESHOLD
from common.conversions import Conversions as CV
from common.params import Params

class CarState(CarStateBase):
  def __init__(self, CP):
    super().__init__(CP)
    can_define = CANDefine(DBC[CP.carFingerprint]["pt"])
    self.shifter_values = can_define.dv["ECMPRDNL"]["PRNDL"]
    self.adaptive_Cruise = False
    self.enable_lkas = False
    self.lka_steering_cmd_counter = 0
    self.cruiseState_speed = 0

    self.acc_mode = False
    self.cruise_gap = 1
    self.brake_pressed = False
    self.gas_pressed = False
    self.standstill = False
    self.cruiseState_enabled = False
    self.use_cluster_speed = Params().get_bool('UseClusterSpeed')


  def update(self, pt_cp, loopback_cp):
    ret = car.CarState.new_message()
    ret.adaptiveCruise = self.adaptive_Cruise
    ret.lkasEnable = self.enable_lkas
    self.prev_cruise_buttons = self.cruise_buttons
    self.cruise_buttons = pt_cp.vl["ASCMSteeringButton"]["ACCButtons"]

    ret.wheelSpeeds = self.get_wheel_speeds(
      pt_cp.vl["EBCMWheelSpdFront"]["FLWheelSpd"],
      pt_cp.vl["EBCMWheelSpdFront"]["FRWheelSpd"],
      pt_cp.vl["EBCMWheelSpdRear"]["RLWheelSpd"],
      pt_cp.vl["EBCMWheelSpdRear"]["RRWheelSpd"],
    )

    vEgoRawClu = (pt_cp.vl["ECMVehicleSpeed"]["VehicleSpeed"] * CV.MPH_TO_MS)
    vEgoClu, aEgoClu = self.update_clu_speed_kf(vEgoRawClu)

    vEgoRawWheel = mean([ret.wheelSpeeds.fl, ret.wheelSpeeds.fr, ret.wheelSpeeds.rl, ret.wheelSpeeds.rr])
    vEgoWheel, aEgoWheel = self.update_speed_kf(vEgoRawWheel)

    if self.use_cluster_speed:
      ret.vEgoRaw = vEgoRawClu
      ret.vEgo = vEgoClu
      ret.aEgo = aEgoClu
    else:
      ret.vEgoRaw = vEgoRawWheel
      ret.vEgo = vEgoWheel
      ret.aEgo = aEgoWheel



    ###for neokii integration
    ret.cluSpeedMs = vEgoRawClu
    self.ECMVehicleSpeed = pt_cp.vl["ECMVehicleSpeed"]
    ret.vCluRatio = (vEgoWheel / vEgoClu) if (vEgoClu > 3. and vEgoWheel > 3.) else 1.0
    ###for neokii integration ends

    ret.standstill = ret.vEgoRaw < 0.01

    ret.gearShifter = self.parse_gear_shifter(self.shifter_values.get(pt_cp.vl["ECMPRDNL"]["PRNDL"], None))
    ret.brake = pt_cp.vl["EBCMBrakePedalPosition"]["BrakePedalPosition"] / 0xd0
    # Brake pedal's potentiometer returns near-zero reading even when pedal is not pressed.
    if ret.brake < 10/0xd0:
      ret.brake = 0.

    if self.CP.enableGasInterceptor:
      ret.gas = (pt_cp.vl["GAS_SENSOR"]["INTERCEPTOR_GAS"] + pt_cp.vl["GAS_SENSOR"]["INTERCEPTOR_GAS2"]) / 2.
      ret.gasPressed = ret.gas > 20
      if pt_cp.vl["AcceleratorPedal"]["AcceleratorPedal"] / 254. > 1e-5 :
        self.gas_pressed = True
    else:
      ret.gas = pt_cp.vl["AcceleratorPedal"]["AcceleratorPedal"] / 254.
      ret.gasPressed = ret.gas > 1e-5
      self.gas_pressed =  ret.gasPressed

    ret.steeringAngleDeg = pt_cp.vl["PSCMSteeringAngle"]["SteeringWheelAngle"]
    ret.steeringRateDeg = pt_cp.vl["PSCMSteeringAngle"]["SteeringWheelRate"]
    ret.steeringTorque = pt_cp.vl["PSCMStatus"]["LKADriverAppldTrq"]
    ret.steeringTorqueEps = pt_cp.vl["PSCMStatus"]["LKATorqueDelivered"]
    ret.steeringPressed = abs(ret.steeringTorque) > STEER_THRESHOLD
    self.lka_steering_cmd_counter = loopback_cp.vl["ASCMLKASteeringCmd"]["RollingCounter"]

    # 0 inactive, 1 active, 2 temporarily limited, 3 failed
    self.lkas_status = pt_cp.vl["PSCMStatus"]["LKATorqueDeliveredStatus"]
    ret.steerFaultTemporary = self.lkas_status == 2
    ret.steerFaultPermanent = self.lkas_status == 3

    # 1 - open, 0 - closed
    ret.doorOpen = (pt_cp.vl["BCMDoorBeltStatus"]["FrontLeftDoor"] == 1 or
                    pt_cp.vl["BCMDoorBeltStatus"]["FrontRightDoor"] == 1 or
                    pt_cp.vl["BCMDoorBeltStatus"]["RearLeftDoor"] == 1 or
                    pt_cp.vl["BCMDoorBeltStatus"]["RearRightDoor"] == 1)

    # 1 - latched
    ret.seatbeltUnlatched = pt_cp.vl["BCMDoorBeltStatus"]["LeftSeatBelt"] == 0
    ret.leftBlinker = pt_cp.vl["BCMTurnSignals"]["TurnSignals"] == 1
    ret.rightBlinker = pt_cp.vl["BCMTurnSignals"]["TurnSignals"] == 2

    self.park_brake = pt_cp.vl["EPBStatus"]["EPBClosed"]
    self.main_on = bool(pt_cp.vl["ECMEngineStatus"]["CruiseMainOn"])
    ret.mainOn = self.main_on
    ret.espDisabled = pt_cp.vl["ESPStatus"]["TractionControlOn"] != 1
    self.pcm_acc_status = pt_cp.vl["AcceleratorPedal2"]["CruiseState"]
    ret.cruiseState.available = self.pcm_acc_status != 0
    ret.cruiseState.standstill = False

    ret.brakePressed = ret.brake > 1e-5
    self.brake_pressed = ret.brakePressed
    ret.regenPressed = False
    if  self.car_fingerprint == CAR.BOLT_EV:
      ret.regenPressed = bool(pt_cp.vl["EBCMRegenPaddle"]["RegenPaddle"])
    brake_light_enable = False
    if self.car_fingerprint == CAR.BOLT_EV:
      if ret.aEgo < -1.3:
        brake_light_enable = True
    ret.brakeLights = ret.brakePressed or brake_light_enable

    ret.cruiseState.enabled = self.main_on or ret.adaptiveCruise
    self.cruiseState_enabled = ret.cruiseState.enabled
    ###for neokii integration
    ret.cruiseState.enabledAcc = ret.cruiseState.enabled
    ###for neokii integration ends


    return ret

  @staticmethod
  def get_can_parser(CP):
    # this function generates lists for signal, messages and initial values
    signals = [
      # sig_name, sig_address, default
      ("BrakePedalPosition", "EBCMBrakePedalPosition", 0),
      ("FrontLeftDoor", "BCMDoorBeltStatus", 0),
      ("FrontRightDoor", "BCMDoorBeltStatus", 0),
      ("RearLeftDoor", "BCMDoorBeltStatus", 0),
      ("RearRightDoor", "BCMDoorBeltStatus", 0),
      ("LeftSeatBelt", "BCMDoorBeltStatus", 0),
      ("RightSeatBelt", "BCMDoorBeltStatus", 0),
      ("TurnSignals", "BCMTurnSignals", 0),
      ("AcceleratorPedal", "AcceleratorPedal", 0),
      ("CruiseState", "AcceleratorPedal2", 0),
      ("ACCButtons", "ASCMSteeringButton", CruiseButtons.UNPRESS),
      ("SteeringWheelAngle", "PSCMSteeringAngle", 0),
      ("SteeringWheelRate", "PSCMSteeringAngle", 0),
      ("FLWheelSpd", "EBCMWheelSpdFront", 0),
      ("FRWheelSpd", "EBCMWheelSpdFront", 0),
      ("RLWheelSpd", "EBCMWheelSpdRear", 0),
      ("RRWheelSpd", "EBCMWheelSpdRear", 0),
      ("PRNDL", "ECMPRDNL", 0),
      ("LKADriverAppldTrq", "PSCMStatus", 0),
      ("LKATorqueDelivered", "PSCMStatus", 0),
      ("LKATorqueDeliveredStatus", "PSCMStatus", 0),
      ("TractionControlOn", "ESPStatus", 0),
      ("EPBClosed", "EPBStatus", 0),
      ("CruiseMainOn", "ECMEngineStatus", 0),
      ("ACCCmdActive", "ASCMActiveCruiseControlStatus", 0),
      ("LKATotalTorqueDelivered", "PSCMStatus", 0),
      ("VehicleSpeed", "ECMVehicleSpeed", 0),

    ]


    # if CP.carFingerprint == CAR.BOLT_EV:
    signals += [
      ("RegenPaddle", "EBCMRegenPaddle", 0),
    ]


    if CP.enableGasInterceptor:
      signals += [
        ("INTERCEPTOR_GAS", "GAS_SENSOR", 0),
        ("INTERCEPTOR_GAS2", "GAS_SENSOR", 0)
      ]


    return CANParser(DBC[CP.carFingerprint]['pt'], signals, [], CanBus.POWERTRAIN, enforce_checks=False)

  @staticmethod
  def get_loopback_can_parser(CP):
    signals = [
      ("RollingCounter", "ASCMLKASteeringCmd", 0),
    ]

    checks = [
      ("ASCMLKASteeringCmd", 50),
    ]

    return CANParser(DBC[CP.carFingerprint]["pt"], signals, checks, CanBus.LOOPBACK)