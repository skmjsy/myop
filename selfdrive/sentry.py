"""Install exception handler for process crash."""
import sentry_sdk
from enum import Enum
from sentry_sdk.integrations.threading import ThreadingIntegration

from common.params import Params
from selfdrive.athena.registration import is_registered_device
from selfdrive.hardware import HARDWARE, PC
from selfdrive.swaglog import cloudlog
from selfdrive.version import get_branch, get_commit, get_origin, get_version, \
  is_comma_remote, is_dirty, is_tested_branch, is_jc01rho_remote
import datetime
import traceback


class SentryProject(Enum):
  # python project
  SELFDRIVE = "https://0b682fa442b544d29161eb8908cdfdc5@o346458.ingest.sentry.io/6378739"
  # native project
  SELFDRIVE_NATIVE = "https://b4857f9d28d74694964d647051498c1f@o346458.ingest.sentry.io/6378742"


def report_tombstone(fn: str, message: str, contents: str) -> None:
  cloudlog.error({'tombstone': message})

  with sentry_sdk.configure_scope() as scope:
    scope.set_extra("tombstone_fn", fn)
    scope.set_extra("tombstone", contents)
    sentry_sdk.capture_message(message=message)
    sentry_sdk.flush()


def capture_exception(*args, **kwargs) -> None:
  cloudlog.error("crash", exc_info=kwargs.get('exc_info', 1))

  try:
    with open('/data/log/last_exception', 'w') as f:
      now = datetime.datetime.now()
      f.write(now.strftime('[%Y-%m-%d %H:%M:%S]') + "\n\n" + str(traceback.format_exc()))
  except Exception:
    pass

  try:
    sentry_sdk.capture_exception(*args, **kwargs)
    sentry_sdk.flush()  # https://github.com/getsentry/sentry-python/issues/291
  except Exception:
    cloudlog.exception("sentry exception")


def set_tag(key: str, value: str) -> None:
  sentry_sdk.set_tag(key, value)


def init(project: SentryProject) -> None:
  # forks like to mess with this, so double check
  jc01rho_remote = is_jc01rho_remote() and "jc01rho" in get_origin(default="")
  if not jc01rho_remote or  PC: #or not is_registered_device()
    return

  env = "release" if is_tested_branch() else "master"
  dongle_id = Params().get("DongleId", encoding='utf-8')

  integrations = []
  if project == SentryProject.SELFDRIVE:
    integrations.append(ThreadingIntegration(propagate_hub=True))
  else:
    sentry_sdk.utils.MAX_STRING_LENGTH = 8192

  sentry_sdk.init(project.value,
                  default_integrations=False,
                  release=get_version(),
                  integrations=integrations,
                  traces_sample_rate=1.0,
                  environment=env)

  sentry_sdk.set_user({"id": dongle_id})
  sentry_sdk.set_tag("dirty", is_dirty())
  sentry_sdk.set_tag("origin", get_origin())
  sentry_sdk.set_tag("branch", get_branch())
  sentry_sdk.set_tag("commit", get_commit())
  sentry_sdk.set_tag("device", HARDWARE.get_device_type())

  if project == SentryProject.SELFDRIVE:
    sentry_sdk.Hub.current.start_session()
