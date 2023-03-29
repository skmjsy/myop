#!/usr/bin/env python3
import unittest
from panda import Panda
from panda.tests.safety import libpandasafety_py
import panda.tests.safety.common as common
from panda.tests.safety.common import CANPackerPanda

MAX_RATE_UP = 10
MAX_RATE_DOWN = 25
MAX_STEER = 2047

MAX_RT_DELTA = 940
RT_INTERVAL = 250000

DRIVER_TORQUE_ALLOWANCE = 15
DRIVER_TORQUE_FACTOR = 1


class TestMazdaSafety(common.PandaSafetyTest):

  TX_MSGS = [[0x243, 0], [0x09d, 0], [0x440, 0]]
  STANDSTILL_THRESHOLD = .1
  RELAY_MALFUNCTION_ADDR = 0x243
  RELAY_MALFUNCTION_BUS = 0
  FWD_BLACKLISTED_ADDRS = {2: [0x243, 0x440]}
  FWD_BUS_LOOKUP = {0: 2, 2: 0}

  def setUp(self):
    self.packer = CANPackerPanda("mazda_2017")
    self.safety = libpandasafety_py.libpandasafety
    self.safety.set_safety_hooks(Panda.SAFETY_MAZDA, 0)
    self.safety.init_tests()

  def _torque_meas_msg(self, torque):
    values = {"STEER_TORQUE_MOTOR": torque}
    return self.packer.make_can_msg_panda("STEER_TORQUE", 0, values)

#  def _torque_driver_msg(self, torque):
#    values = {"STEER_TORQUE_DRIVER": torque}
#    return self.packer.make_can_msg_panda("STEER_TORQUE", 0, values)

  def _torque_msg(self, torque):
    values = {"LKAS_REQUEST": torque}
    return self.packer.make_can_msg_panda("CAM_LKAS", 0, values)

  def _speed_msg(self, speed):
    values = {"SPEED": speed}
    return self.packer.make_can_msg_panda("ENGINE_DATA", 0, values)

  def _brake_msg(self, brake):
    values = {"BRAKE_ON": brake}
    return self.packer.make_can_msg_panda("PEDALS", 0, values)

  def _gas_msg(self, gas):
    values = {"PEDAL_GAS": gas}
    return self.packer.make_can_msg_panda("ENGINE_DATA", 0, values)

  def _pcm_status_msg(self, enable):
    values = {"CRZ_ACTIVE": enable}
    return self.packer.make_can_msg_panda("CRZ_CTRL", 0, values)

  def _button_msg(self, resume=False, cancel=False):
    values = {
      "CAN_OFF": cancel,
      "CAN_OFF_INV": (cancel + 1) % 2,
      "RES": resume,
      "RES_INV": (resume + 1) % 2,
    }
    return self.packer.make_can_msg_panda("CRZ_BTNS", 0, values)

  def test_buttons(self):
    # only cancel allows while controls not allowed
    self.safety.set_controls_allowed(0)
    self.assertTrue(self._tx(self._button_msg(cancel=True)))
    self.assertFalse(self._tx(self._button_msg(resume=True)))

    # do not block resume if we are engaged already
    self.safety.set_controls_allowed(1)
    self.assertTrue(self._tx(self._button_msg(cancel=True)))
    self.assertTrue(self._tx(self._button_msg(resume=True)))


if __name__ == "__main__":
  unittest.main()
