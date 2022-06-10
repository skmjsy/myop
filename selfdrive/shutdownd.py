#!/usr/bin/env python3

import os
import time
import cereal.messaging as messaging
from common.numpy_fast import interp
from common.params import Params

def main():
  opkrAutoShutdown = interp(int(Params().get("OpkrAutoShutdown", encoding="utf8")), [0,1,2,3,4,5,6,7,8,9], [0,5,30,60,180,300,600,1800,3600,10800])
  shutdown_at = opkrAutoShutdown
  shutdown_count = 0
  device_state_sock = messaging.sub_sock('deviceState')

  while 1:
    msg = messaging.recv_sock(device_state_sock, wait=True)
    if msg is not None:
      if not msg.deviceState.usbOnline:
        shutdown_count += 5
      else:
        shutdown_count = 0

      print('current', shutdown_count, 'shutdown_at', shutdown_at)

      if shutdown_count >= shutdown_at > 0:
        os.system('LD_LIBRARY_PATH="" svc power shutdown')

    time.sleep(5)


if __name__ == "__main__":
  main()
