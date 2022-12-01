import overpy
import subprocess
import numpy as np
from common.params import Params
from selfdrive.mapd.lib.geo import R
from common.log import Loger

OSM_LOCAL_PATH = "/data/media/0/osm"
OSM_VERSION = "0.7.56"
OSM_QUERY = [f"{OSM_LOCAL_PATH}/v{OSM_VERSION}/bin/osm3s_query", f"--db-dir={OSM_LOCAL_PATH}/db"]

def create_way(way_id, node_ids, from_way):
  """
  Creates and OSM Way with the given `way_id` and list of `node_ids`, copying attributes and tags from `from_way`
  """
  return overpy.Way(way_id, node_ids=node_ids, attributes={}, result=from_way._result,
                    tags=from_way.tags)


class OSM():
  def __init__(self):
    self.api = overpy.Overpass()
    self.osm_local_db_enabled = Params().get_bool("OsmLocalDb")
    # self.api = overpy.Overpass(url='http://3.65.170.21/api/interpreter')
    self.log = Loger()

  def fetch_road_ways_around_location(self, lat, lon, radius):
    # Calculate the bounding box coordinates for the bbox containing the circle around location.
    bbox_angle = np.degrees(radius / R)
    # fetch all ways and nodes on this ways in bbox
    bbox_str = f'{str(lat - bbox_angle)},{str(lon - bbox_angle)},{str(lat + bbox_angle)},{str(lon + bbox_angle)}'
    # bbox_str = f'around:25,{str(lat)},{str(lon)}'
    # q = """
    #     way(""" + bbox_str + """)
    #       [highway]
    #       [highway!~"^(footway|path|corridor|bridleway|steps|cycleway|construction|bus_guideway|escape|service|track)$"];
    #     (._;>;);
    #     out;
    #     """

    q = """
        way(""" + bbox_str + """)
          ["maxspeed"];
        (._;>;);
        out;"""

    try:
      if self.osm_local_db_enabled:
        cmd = OSM_QUERY
        cmd.append(f"--request={q}")
        completion = subprocess.run(cmd, check=True, capture_output=True)
        ways = self.api.parse_xml(completion.stdout).ways
      else:  
        ways = self.api.query(q).ways

    except Exception as e:
      print(f'Exception while querying OSM:\n{e}')
      ways = []

    return ways
