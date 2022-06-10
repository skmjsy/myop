#!/usr/bin/env python3
import os
import sys
import errno
import shutil
from urllib.request import urlopen
from glob import glob
import subprocess
import importlib.util

# NOTE: Do NOT import anything here that needs be built (e.g. params)
from common.basedir import BASEDIR
from common.spinner import Spinner
from selfdrive.hardware import TICI


OPSPLINE_SPEC = importlib.util.find_spec('opspline') or importlib.util.find_spec('scipy')
OVERPY_SPEC = importlib.util.find_spec('overpy')
MAX_BUILD_PROGRESS = 100
TMP_DIR = '/data/tmp'
PYEXTRA_DIR = '/data/openpilot/pyextra'


def wait_for_internet_connection():
  while True:
    try:
      _ = urlopen('https://www.google.com/', timeout=10)
      return
    except Exception:
      pass


def install_dep(spinner):
  wait_for_internet_connection()

  if TICI:
    TOTAL_PIP_STEPS = 2986

    try:
      os.makedirs(TMP_DIR)
    except OSError as e:
      if e.errno != errno.EEXIST:
        raise
    my_env = os.environ.copy()
    my_env['TMPDIR'] = TMP_DIR

    pip_target = [f'--target={PYEXTRA_DIR}']
    packages = []
    if OPSPLINE_SPEC is None:
      packages.append('scipy==1.7.1')
    if OVERPY_SPEC is None:
      packages.append('overpy==0.6')

    pip = subprocess.Popen([sys.executable, "-m", "pip", "install", "-v"] + pip_target + packages,
                           stdout=subprocess.PIPE, env=my_env)
  else:
    TOTAL_PIP_STEPS = 24
    # mount system rw so apt and pip can do its thing
    subprocess.check_call(['mount', '-o', 'rw,remount', '/system'])

    # Run preparation script for pip installation
    subprocess.check_call(['sh', './install_gfortran.sh'], cwd=os.path.join(BASEDIR, 'installer/custom/'))

    # install pip from git
    package = 'git+https://github.com/move-fast/opspline.git@master'
    pip = subprocess.Popen([sys.executable, "-m", "pip", "install", "-v", package], stdout=subprocess.PIPE)

  # Read progress from pip and update spinner
  steps = 0
  while True:
    output = pip.stdout.readline()
    if pip.poll() is not None:
      break
    if output:
      steps += 1
      spinner.update_progress(MAX_BUILD_PROGRESS * min(1., steps / TOTAL_PIP_STEPS), 100.)
      print(output.decode('utf8', 'replace'))
  if TICI:
    shutil.rmtree(TMP_DIR)
    os.unsetenv('TMPDIR')

    # remove numpy installed to PYEXTRA_DIR since numpy is already present in the AGNOS image
    if OPSPLINE_SPEC is None:
      for directory in glob(f'{PYEXTRA_DIR}/numpy*'):
        shutil.rmtree(directory)
      shutil.rmtree(f'{PYEXTRA_DIR}/bin')


if __name__ == "__main__" and (OPSPLINE_SPEC is None or OVERPY_SPEC is None):
  spinner = Spinner()
  spinner.update_progress(0, 100)
  install_dep(spinner)
