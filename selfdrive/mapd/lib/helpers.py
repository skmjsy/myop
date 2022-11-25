import overpy
import subprocess
import requests
import email.utils as eut
import time

from common.params import Params
from selfdrive.hardware import TICI

OSM_LOCAL_PATH = "/data/media/0/osm"
OSM_DB_STAMP_FILE = OSM_LOCAL_PATH + "/db_stamp"
OSM_VERSION = "0.7.57" if TICI else "0.7.56"
OSM_QUERY = [f"{OSM_LOCAL_PATH}/v{OSM_VERSION}/bin/osm3s_query", f"--db-dir={OSM_LOCAL_PATH}/db"]

def get_current_s3_osm_db_timestamp():
  local_osm_db_name = Params().get("OsmLocationName")
  if local_osm_db_name is None:
    return None
  local_osm_db_name = local_osm_db_name.decode("utf-8")
  r = requests.head(f"https://mkumard.synology.me/osm/{local_osm_db_name}.txt")
  if r.status_code != 200:
    print(f'Failed to fetch HEAD for S3 OSM db.\n\n{r.status_code}')
    return None

  timestamp_string = r.headers.get('Last-Modified', None)
  if timestamp_string is None:
    print(f'HEAD for S3 OSM db contained no "Last-Modified" value.\n\n{r.headers}')
    return None

  try:
    parsed_date = eut.parsedate(timestamp_string)
    return time.mktime(parsed_date)
  except Exception as e:
    print(f'Could not parse last modified timestamp for S3 local osm db.\n\n{e}')
    return None


def persist_s3_osm_db_timestamp(timestamp):
  try:
    with open(OSM_DB_STAMP_FILE, 'w') as file:
      file.write(f'{timestamp}')
  except Exception as e:
    print(f'Failed to timestamp local OSM db.\n\n{e}')


def get_local_osm_timestamp():
  try:
    with open(OSM_DB_STAMP_FILE, 'r') as file:
      return float(file.readline())
  except Exception as e:
    print(f'Failed to read timestamp for local OSM db.\n\n{e}')
    return None


def is_osm_db_up_to_date():
  current_osm_ts = get_local_osm_timestamp()
  if current_osm_ts is None:
    return False

  current_s3_osm_ts = get_current_s3_osm_db_timestamp()
  if current_s3_osm_ts is None:
    return True

  return current_osm_ts == current_s3_osm_ts


def timestamp_local_osm_db():
  current_s3_osm_ts = get_current_s3_osm_db_timestamp()
  if current_s3_osm_ts is not None:
    persist_s3_osm_db_timestamp(current_s3_osm_ts)


def is_local_osm_installed():
  api = overpy.Overpass()
  waypoint = Params().get("OsmWayTest")
  if waypoint is None:
    return False
  waypoint = waypoint.decode("utf-8")
  q = f"""
      way({waypoint});
      (._;>;);
      out;
      """

  try:
    cmd = OSM_QUERY
    cmd.append(f"--request={q}")
    completion = subprocess.run(cmd, check=True, capture_output=True)
    print(f'OSM local query returned with exit code: {completion.returncode}')

    if completion.returncode != 0:
      return False

    print(f'OSM Local query returned:\n\n{completion.stdout}')

    ways = api.parse_xml(completion.stdout).ways
    success = len(ways) == 1
    print(f"Test osm script returned {len(ways)} ways")
    print(f'OSM local server query {"succeeded" if success else "failed"}')

    return success

  except Exception as e:
    print(e)
    return False
