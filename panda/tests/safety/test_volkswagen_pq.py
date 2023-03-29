#!/usr/bin/env python3
import unittest
import numpy as np
from panda import Panda
from panda.tests.safety import libpandasafety_py
import panda.tests.safety.common as common
from panda.tests.safety.common import CANPackerPanda, MAX_WRONG_COUNTERS

MAX_RATE_UP = 4
MAX_RATE_DOWN = 10
MAX_STEER = 300
MAX_RT_DELTA = 75
RT_INTERVAL = 250000

DRIVER_TORQUE_ALLOWANCE = 80
DRIVER_TORQUE_FACTOR = 3

MSG_LENKHILFE_3 = 0x0D0  # RX from EPS, for steering angle and driver steering torque
MSG_HCA_1 = 0x0D2        # TX by OP, Heading Control Assist steering torque
MSG_MOTOR_2 = 0x288      # RX from ECU, for CC state and brake switch state
MSG_MOTOR_3 = 0x380      # RX from ECU, for driver throttle input
MSG_GRA_NEU = 0x38A      # TX by OP, ACC control buttons for cancel/resume
MSG_BREMSE_1 = 0x1A0     # RX from ABS, for ego speed
MSG_LDW_1 = 0x5BE        # TX by OP, Lane line recognition and text alerts


def volkswagen_pq_checksum(msg, addr, len_msg):
  msg_bytes = bytes(msg.data)
  msg_bytes = msg_bytes[1:len_msg]

  checksum = 0
  for i in msg_bytes:
    checksum ^= i
  return checksum

class TestVolkswagenPqSafety(common.PandaSafetyTest):
  cruise_engaged = False
  cnt_lenkhilfe_3 = 0
  cnt_hca_1 = 0
  cnt_gra_neu = 0

  # Transmit of GRA_Neu is allowed on bus 0 and 2 to keep compatibility with gateway and camera integration
  TX_MSGS = [[MSG_HCA_1, 0], [MSG_GRA_NEU, 0], [MSG_GRA_NEU, 2], [MSG_LDW_1, 0]]
  STANDSTILL_THRESHOLD = 1
  RELAY_MALFUNCTION_ADDR = MSG_HCA_1
  RELAY_MALFUNCTION_BUS = 0
  FWD_BLACKLISTED_ADDRS = {2: [MSG_HCA_1, MSG_LDW_1]}
  FWD_BUS_LOOKUP = {0: 2, 2: 0}

  def setUp(self):
    self.packer = CANPackerPanda("vw_golf_mk4")
    self.safety = libpandasafety_py.libpandasafety
    self.safety.set_safety_hooks(Panda.SAFETY_VOLKSWAGEN_PQ, 0)
    self.safety.init_tests()

  def _set_prev_torque(self, t):
    self.safety.set_desired_torque_last(t)
    self.safety.set_rt_torque_last(t)

  # Ego speed (Bremse_1)
  def _speed_msg(self, speed):
    values = {"Geschwindigkeit_neu__Bremse_1_": speed}
    return self.packer.make_can_msg_panda("Bremse_1", 0, values)

  # Brake light switch (shared message Motor_2)
  def _brake_msg(self, brake):
    # since this signal is used for engagement status, preserve current state
    return self._motor_2_msg(brake_pressed=brake, cruise_engaged=self.safety.get_controls_allowed())

  # ACC engaged status (shared message Motor_2)
  def _pcm_status_msg(self, enable):
    self.__class__.cruise_engaged = enable
    return self._motor_2_msg(cruise_engaged=enable)

  # Driver steering input torque
  def _lenkhilfe_3_msg(self, torque):
    values = {"LH3_LM": abs(torque), "LH3_LMSign": torque < 0,
              "LH3_Zaehler": self.cnt_lenkhilfe_3 % 16}
    # TODO: move checksum handling to CPP library with the rest
    to_calc = self.packer.make_can_msg_panda("Lenkhilfe_3", 0, values)
    values.update({"LH3_Checksumme": volkswagen_pq_checksum(to_calc[0], MSG_LENKHILFE_3, 6)})
    self.__class__.cnt_lenkhilfe_3 += 1
    return self.packer.make_can_msg_panda("Lenkhilfe_3", 0, values)

  # openpilot steering output torque
  def _hca_1_msg(self, torque):
    values = {"LM_Offset": abs(torque), "LM_OffSign": torque < 0,
              "HCA_Zaehler": self.cnt_hca_1 % 16}
    # TODO: move checksum handling to CPP library with the rest
    to_calc = self.packer.make_can_msg_panda("HCA_1", 0, values)
    values.update({"HCA_Checksumme": volkswagen_pq_checksum(to_calc[0], MSG_HCA_1, 5)})
    self.__class__.cnt_hca_1 += 1
    return self.packer.make_can_msg_panda("HCA_1", 0, values)

  # ACC engagement and brake light switch status
  # Called indirectly for compatibility with common.py tests
  def _motor_2_msg(self, brake_pressed=False, cruise_engaged=False):
    values = {"Bremslichtschalter": brake_pressed,
              "GRA_Status": cruise_engaged}
    return self.packer.make_can_msg_panda("Motor_2", 0, values)

  # Driver throttle input (Motor_3)
  def _gas_msg(self, gas):
    values = {"Fahrpedal_Rohsignal": gas}
    return self.packer.make_can_msg_panda("Motor_3", 0, values)

  # Cruise control buttons (GRA_Neu)
  def _button_msg(self, _set=False, resume=False, cancel=False):
    values = {"GRA_Neu_Setzen": _set, "GRA_Recall": resume,
              "GRA_Abbrechen": cancel, "GRA_Neu_Zaehler": self.cnt_gra_neu % 16}
    # TODO: move checksum handling to CPP library with the rest
    to_calc = self.packer.make_can_msg_panda("GRA_Neu", 2, values)
    values.update({"GRA_Checksum": volkswagen_pq_checksum(to_calc[0], MSG_GRA_NEU, 4)})
    self.__class__.cnt_gra_neu += 1
    return self.packer.make_can_msg_panda("GRA_Neu", 2, values)

  def test_steer_safety_check(self):
    for enabled in [0, 1]:
      for t in range(-500, 500):
        self.safety.set_controls_allowed(enabled)
        self._set_prev_torque(t)
        if abs(t) > MAX_STEER or (not enabled and abs(t) > 0):
          self.assertFalse(self._tx(self._hca_1_msg(t)))
        else:
          self.assertTrue(self._tx(self._hca_1_msg(t)))

  def test_spam_cancel_safety_check(self):
    self.safety.set_controls_allowed(0)
    self.assertTrue(self._tx(self._button_msg(cancel=True)))
    self.assertFalse(self._tx(self._button_msg(resume=True)))
    self.assertFalse(self._tx(self._button_msg(_set=True)))
    # do not block resume if we are engaged already
    self.safety.set_controls_allowed(1)
    self.assertTrue(self._tx(self._button_msg(resume=True)))

  def test_non_realtime_limit_up(self):
    self.safety.set_torque_driver(0, 0)
    self.safety.set_controls_allowed(True)

    self._set_prev_torque(0)
    self.assertTrue(self._tx(self._hca_1_msg(MAX_RATE_UP)))
    self._set_prev_torque(0)
    self.assertTrue(self._tx(self._hca_1_msg(-MAX_RATE_UP)))

    self._set_prev_torque(0)
    self.assertFalse(self._tx(self._hca_1_msg(MAX_RATE_UP + 1)))
    self.safety.set_controls_allowed(True)
    self._set_prev_torque(0)
    self.assertFalse(self._tx(self._hca_1_msg(-MAX_RATE_UP - 1)))

  def test_non_realtime_limit_down(self):
    self.safety.set_torque_driver(0, 0)
    self.safety.set_controls_allowed(True)

  def test_against_torque_driver(self):
    self.safety.set_controls_allowed(True)

    for sign in [-1, 1]:
      for t in np.arange(0, DRIVER_TORQUE_ALLOWANCE + 1, 1):
        t *= -sign
        self.safety.set_torque_driver(t, t)
        self._set_prev_torque(MAX_STEER * sign)
        self.assertTrue(self._tx(self._hca_1_msg(MAX_STEER * sign)))

      self.safety.set_torque_driver(DRIVER_TORQUE_ALLOWANCE + 1, DRIVER_TORQUE_ALLOWANCE + 1)
      self.assertFalse(self._tx(self._hca_1_msg(-MAX_STEER)))

    # spot check some individual cases
    for sign in [-1, 1]:
      driver_torque = (DRIVER_TORQUE_ALLOWANCE + 10) * sign
      torque_desired = (MAX_STEER - 10 * DRIVER_TORQUE_FACTOR) * sign
      delta = 1 * sign
      self._set_prev_torque(torque_desired)
      self.safety.set_torque_driver(-driver_torque, -driver_torque)
      self.assertTrue(self._tx(self._hca_1_msg(torque_desired)))
      self._set_prev_torque(torque_desired + delta)
      self.safety.set_torque_driver(-driver_torque, -driver_torque)
      self.assertFalse(self._tx(self._hca_1_msg(torque_desired + delta)))

      self._set_prev_torque(MAX_STEER * sign)
      self.safety.set_torque_driver(-MAX_STEER * sign, -MAX_STEER * sign)
      self.assertTrue(self._tx(self._hca_1_msg((MAX_STEER - MAX_RATE_DOWN) * sign)))
      self._set_prev_torque(MAX_STEER * sign)
      self.safety.set_torque_driver(-MAX_STEER * sign, -MAX_STEER * sign)
      self.assertTrue(self._tx(self._hca_1_msg(0)))
      self._set_prev_torque(MAX_STEER * sign)
      self.safety.set_torque_driver(-MAX_STEER * sign, -MAX_STEER * sign)
      self.assertFalse(self._tx(self._hca_1_msg((MAX_STEER - MAX_RATE_DOWN + 1) * sign)))

  def test_realtime_limits(self):
    self.safety.set_controls_allowed(True)

    for sign in [-1, 1]:
      self.safety.init_tests()
      self._set_prev_torque(0)
      self.safety.set_torque_driver(0, 0)
      for t in np.arange(0, MAX_RT_DELTA, 1):
        t *= sign
        self.assertTrue(self._tx(self._hca_1_msg(t)))
      self.assertFalse(self._tx(self._hca_1_msg(sign * (MAX_RT_DELTA + 1))))

      self._set_prev_torque(0)
      for t in np.arange(0, MAX_RT_DELTA, 1):
        t *= sign
        self.assertTrue(self._tx(self._hca_1_msg(t)))

      # Increase timer to update rt_torque_last
      self.safety.set_timer(RT_INTERVAL + 1)
      self.assertTrue(self._tx(self._hca_1_msg(sign * (MAX_RT_DELTA - 1))))
      self.assertTrue(self._tx(self._hca_1_msg(sign * (MAX_RT_DELTA + 1))))

  def test_torque_measurements(self):
    self._rx(self._lenkhilfe_3_msg(50))
    self._rx(self._lenkhilfe_3_msg(-50))
    self._rx(self._lenkhilfe_3_msg(0))
    self._rx(self._lenkhilfe_3_msg(0))
    self._rx(self._lenkhilfe_3_msg(0))
    self._rx(self._lenkhilfe_3_msg(0))

    self.assertEqual(-50, self.safety.get_torque_driver_min())
    self.assertEqual(50, self.safety.get_torque_driver_max())

    self._rx(self._lenkhilfe_3_msg(0))
    self.assertEqual(0, self.safety.get_torque_driver_max())
    self.assertEqual(-50, self.safety.get_torque_driver_min())

    self._rx(self._lenkhilfe_3_msg(0))
    self.assertEqual(0, self.safety.get_torque_driver_max())
    self.assertEqual(0, self.safety.get_torque_driver_min())

  def test_rx_hook(self):
    # checksum checks
    # this platform only has one relevant checksum-protected message
    self.safety.set_controls_allowed(1)
    to_push = self._lenkhilfe_3_msg(0)
    self.assertTrue(self._rx(to_push))
    to_push[0].data[4] ^= 0xFF
    self.assertFalse(self._rx(to_push))
    self.assertFalse(self.safety.get_controls_allowed())

    # counter checks
    # this platform only has one relevant counter-protected message
    # reset wrong_counters to zero by sending valid messages
    for i in range(MAX_WRONG_COUNTERS + 1):
      self.__class__.cnt_lenkhilfe_3 += 1
      if i < MAX_WRONG_COUNTERS:
        self.safety.set_controls_allowed(1)
        self._rx(self._lenkhilfe_3_msg(0))
      else:
        self.assertFalse(self._rx(self._lenkhilfe_3_msg(0)))
        self.assertFalse(self.safety.get_controls_allowed())

    # restore counters for future tests with a couple of good messages
    for i in range(2):
      self.safety.set_controls_allowed(1)
      self._rx(self._lenkhilfe_3_msg(0))
    self.assertTrue(self.safety.get_controls_allowed())


if __name__ == "__main__":
  unittest.main()
