import math

from cereal import log
from common.numpy_fast import interp
from selfdrive.controls.lib.latcontrol import LatControl, MIN_STEER_SPEED
from selfdrive.controls.lib.pid import PIDController
from selfdrive.controls.lib.vehicle_model import ACCELERATION_DUE_TO_GRAVITY
from common.params import Params
from decimal import Decimal
from selfdrive.ntune import ntune_torque_get

# At higher speeds (25+mph) we can assume:
# Lateral acceleration achieved by a specific car correlates to
# torque applied to the steering rack. It does not correlate to
# wheel slip, or to speed.

# This controller applies torque to achieve desired lateral
# accelerations. To compensate for the low speed effects we
# use a LOW_SPEED_FACTOR in the error. Additionally, there is
# friction in the steering wheel that needs to be overcome to
# move it at all, this is compensated for too.

LOW_SPEED_X = [0, 10, 20, 30]
LOW_SPEED_Y = [15, 13, 10, 5] #comma
#LOW_SPEED_Y = [15, 2, 2, 2] 

class LatControlTorque(LatControl):
  def __init__(self, CP, CI):
    super().__init__(CP, CI)
    self.pid = PIDController(CP.lateralTuning.torque.kp, CP.lateralTuning.torque.ki,
                             k_f=CP.lateralTuning.torque.kf, pos_limit=self.steer_max, neg_limit=-self.steer_max)
    self.torque_from_lateral_accel = CI.torque_from_lateral_accel()
    self.use_steering_angle = CP.lateralTuning.torque.useSteeringAngle
    self.steering_angle_deadzone_deg = CP.lateralTuning.torque.steeringAngleDeadzoneDeg
    self.update_live_torque_params(CP.lateralTuning.torque.latAccelFactor, CP.lateralTuning.torque.latAccelOffset, CP.lateralTuning.torque.friction)

  def update_live_torque_params(self, latAccelFactor, latAccelOffset, friction):
    self.live_torque_params = {
      'latAccelFactor': latAccelFactor,
      'friction': friction,
      'latAccelOffset': latAccelOffset
    }

  def update(self, active, CS, VM, params, last_actuators, steer_limited, desired_curvature, desired_curvature_rate, llk):
    pid_log = log.ControlsState.LateralTorqueState.new_message()

    if CS.vEgo < MIN_STEER_SPEED or not active:
      output_torque = 0.0
      pid_log.active = False
      angle_steers_des = 0.0
    else:
      if self.use_steering_angle:
        actual_curvature = -VM.calc_curvature(math.radians(CS.steeringAngleDeg - params.angleOffsetDeg), CS.vEgo, params.roll)
        curvature_deadzone = abs(VM.calc_curvature(math.radians(self.steering_angle_deadzone_deg), CS.vEgo, 0.0))
      else:
        actual_curvature_vm = -VM.calc_curvature(math.radians(CS.steeringAngleDeg - params.angleOffsetDeg), CS.vEgo, params.roll)
        actual_curvature_llk = llk.angularVelocityCalibrated.value[2] / CS.vEgo
        actual_curvature = interp(CS.vEgo, [2.0, 5.0], [actual_curvature_vm, actual_curvature_llk])
        curvature_deadzone = 0.0
      desired_lateral_accel = desired_curvature * CS.vEgo ** 2

      # desired rate is the desired rate of change in the setpoint, not the absolute desired curvature
      # desired_lateral_jerk = desired_curvature_rate * CS.vEgo ** 2
      actual_lateral_accel = actual_curvature * CS.vEgo ** 2
      lateral_accel_deadzone = curvature_deadzone * CS.vEgo ** 2

      try:
        isLowSpeed  = ntune_torque_get('isLowSpeedFactor') if Params().get_bool('UseNpilotManager') else Params().get_bool('IsLowSpeedFactor')
      except:
        isLowSpeed  = Params().get_bool('IsLowSpeedFactor')

      if isLowSpeed:
        #low_speed_factor = interp(CS.vEgo, [0, 15], [500, 0]) # comma 1st
        low_speed_factor = interp(CS.vEgo, [0, 10, 20], [500, 500, 200]) # comma 2nd
        #low_speed_factor = interp(CS.vEgo, LOW_SPEED_X, LOW_SPEED_Y)**2
      else:
        low_speed_factor = interp(CS.vEgo, [0, 5], [300, 0])

      setpoint = desired_lateral_accel + low_speed_factor * desired_curvature
      measurement = actual_lateral_accel + low_speed_factor * actual_curvature
      
      error = setpoint - measurement
      pid_log.error = self.torque_from_lateral_accel(lateral_accel_value=error, torque_params=self.live_torque_params)

      ff = self.torque_from_lateral_accel(
        lateral_accel_value=desired_lateral_accel - params.roll * ACCELERATION_DUE_TO_GRAVITY,
        torque_params=self.live_torque_params,
        lateral_accel_error=error,
        lateral_accel_deadzone=lateral_accel_deadzone,
        friction_compensation=True
      )
      freeze_integrator = steer_limited or CS.steeringPressed or CS.vEgo < 5
      output_torque = self.pid.update(pid_log.error,
                                      feedforward=ff,
                                      speed=CS.vEgo,
                                      freeze_integrator=freeze_integrator)

      pid_log.active = True
      pid_log.p = self.pid.p
      pid_log.i = self.pid.i
      pid_log.d = self.pid.d
      pid_log.f = self.pid.f
      pid_log.output = -output_torque
      pid_log.actualLateralAccel = actual_lateral_accel
      pid_log.desiredLateralAccel = desired_lateral_accel
      pid_log.saturated = self._check_saturation(self.steer_max - abs(output_torque) < 1e-3, CS, steer_limited)

      angle_steers_des = math.degrees(VM.get_steer_from_curvature(-desired_curvature, CS.vEgo, params.roll)) + params.angleOffsetDeg

    # TODO left is positive in this convention
    return -output_torque, angle_steers_des, pid_log
