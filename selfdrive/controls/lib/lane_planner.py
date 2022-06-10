import numpy as np
from cereal import log
from common.filter_simple import FirstOrderFilter
from common.numpy_fast import interp
from common.realtime import DT_MDL
from selfdrive.hardware import EON, TICI
from selfdrive.swaglog import cloudlog
from common.params import Params
from decimal import Decimal

TRAJECTORY_SIZE = 33
# camera offset is meters from center car to camera
if EON:
  CAMERA_OFFSET = float(Decimal(Params().get("CameraOffsetAdj", encoding="utf8")) * Decimal('0.001'))  # m from center car to camera
  CAMERA_OFFSET_A = CAMERA_OFFSET - 0.2
  PATH_OFFSET = float(Decimal(Params().get("PathOffsetAdj", encoding="utf8")) * Decimal('0.001'))  # default 0.0
elif TICI:
  CAMERA_OFFSET = -0.04
  PATH_OFFSET = -0.04
else:
  CAMERA_OFFSET = 0.0
  PATH_OFFSET = 0.0


class LanePlanner:
  def __init__(self, wide_camera=False):
    self.ll_t = np.zeros((TRAJECTORY_SIZE,))
    self.ll_x = np.zeros((TRAJECTORY_SIZE,))
    self.lll_y = np.zeros((TRAJECTORY_SIZE,))
    self.rll_y = np.zeros((TRAJECTORY_SIZE,))
    self.lane_width_estimate = FirstOrderFilter(3.7, 9.95, DT_MDL)
    self.lane_width_certainty = FirstOrderFilter(1.0, 0.95, DT_MDL)
    self.lane_width = 3.7

    self.lll_prob = 0.
    self.rll_prob = 0.
    self.d_prob = 0.

    self.lll_std = 0.
    self.rll_std = 0.

    self.l_lane_change_prob = 0.
    self.r_lane_change_prob = 0.

    self.camera_offset = -CAMERA_OFFSET if wide_camera else CAMERA_OFFSET
    self.path_offset = -PATH_OFFSET if wide_camera else PATH_OFFSET

    self.left_curv_offset = int(Params().get("LeftCurvOffsetAdj", encoding="utf8"))
    self.right_curv_offset = int(Params().get("RightCurvOffsetAdj", encoding="utf8"))

    self.lp_timer = 0
    self.lp_timer2 = 0

  def parse_model(self, md, sm, v_ego):
    curvature = sm['controlsState'].curvature
    mode_select = sm['carState'].cruiseState.modeSel
    Curv = round(curvature, 4)
    # right lane is minus
    lane_differ = round(abs(self.lll_y[0] + self.rll_y[0]), 2)
    lean_offset = 0
    if mode_select == 4:
      lean_offset = -0.2
    else:
      lean_offset = 0

    if (self.left_curv_offset != 0 or self.left_curv_offset != 0) and v_ego > 8:
      if curvature > 0.0008 and self.left_curv_offset < 0 and lane_differ >= 0: # left curve
        if lane_differ > 0.6:
          lane_differ = 0.6          
        lean_offset = +round(abs(self.left_curv_offset) * lane_differ * 0.05, 3) # move to left
      elif curvature > 0.0008 and self.left_curv_offset > 0 and lane_differ <= 0:
        if lane_differ > 0.6:
          lane_differ = 0.6
        lean_offset = -round(abs(self.left_curv_offset) * lane_differ * 0.05, 3) # move to right
      elif curvature < -0.0008 and self.right_curv_offset < 0 and lane_differ >= 0: # right curve
        if lane_differ > 0.6:
          lane_differ = 0.6    
        lean_offset = +round(abs(self.right_curv_offset) * lane_differ * 0.05, 3) # move to left
      elif curvature < -0.0008 and self.right_curv_offset > 0 and lane_differ <= 0:
        if lane_differ > 0.6:
          lane_differ = 0.6    
        lean_offset = -round(abs(self.right_curv_offset) * lane_differ * 0.05, 3) # move to right
      else:
        lean_offset = 0

    self.lp_timer += DT_MDL
    if self.lp_timer > 1.0:
      self.lp_timer = 0.0
      if Params().get_bool("OpkrLiveTunePanelEnable"):
        self.camera_offset = float(Decimal(Params().get("CameraOffsetAdj", encoding="utf8")) * Decimal('0.001'))

    if len(md.laneLines) == 4 and len(md.laneLines[0].t) == TRAJECTORY_SIZE:
      self.ll_t = (np.array(md.laneLines[1].t) + np.array(md.laneLines[2].t))/2
      # left and right ll x is the same
      self.ll_x = md.laneLines[1].x
      # only offset left and right lane lines; offsetting path does not make sense
      self.lll_y = np.array(md.laneLines[1].y) - self.camera_offset + lean_offset
      self.rll_y = np.array(md.laneLines[2].y) - self.camera_offset + lean_offset
      self.lll_prob = md.laneLineProbs[1]
      self.rll_prob = md.laneLineProbs[2]
      self.lll_std = md.laneLineStds[1]
      self.rll_std = md.laneLineStds[2]

    if len(md.meta.desireState):
      self.l_lane_change_prob = md.meta.desireState[log.LateralPlan.Desire.laneChangeLeft]
      self.r_lane_change_prob = md.meta.desireState[log.LateralPlan.Desire.laneChangeRight]

  def get_d_path(self, v_ego, path_t, path_xyz):
    self.lp_timer2 += DT_MDL
    if self.lp_timer2 > 1.0:
      self.lp_timer2 = 0.0
      if Params().get_bool("OpkrLiveTunePanelEnable"):
        self.path_offset = float(Decimal(Params().get("PathOffsetAdj", encoding="utf8")) * Decimal('0.001'))
    # Reduce reliance on lanelines that are too far apart or
    # will be in a few seconds
    path_xyz[:, 1] -= self.path_offset
    l_prob, r_prob = self.lll_prob, self.rll_prob
    width_pts = self.rll_y - self.lll_y
    prob_mods = []
    for t_check in [0.0, 1.5, 3.0]:
      width_at_t = interp(t_check * (v_ego + 7), self.ll_x, width_pts)
      prob_mods.append(interp(width_at_t, [4.0, 5.0], [1.0, 0.0]))
    mod = min(prob_mods)
    l_prob *= mod
    r_prob *= mod

    # Reduce reliance on uncertain lanelines
    l_std_mod = interp(self.lll_std, [.15, .3], [1.0, 0.0])
    r_std_mod = interp(self.rll_std, [.15, .3], [1.0, 0.0])
    l_prob *= l_std_mod
    r_prob *= r_std_mod

    # Find current lanewidth
    self.lane_width_certainty.update(l_prob * r_prob)
    current_lane_width = abs(self.rll_y[0] - self.lll_y[0])
    self.lane_width_estimate.update(current_lane_width)
    speed_lane_width = interp(v_ego, [0., 31.], [2.8, 3.5])
    self.lane_width = self.lane_width_certainty.x * self.lane_width_estimate.x + \
                      (1 - self.lane_width_certainty.x) * speed_lane_width

    clipped_lane_width = min(4.0, self.lane_width)
    path_from_left_lane = self.lll_y + clipped_lane_width / 2.0
    path_from_right_lane = self.rll_y - clipped_lane_width / 2.0

    self.d_prob = l_prob + r_prob - l_prob * r_prob
    lane_path_y = (l_prob * path_from_left_lane + r_prob * path_from_right_lane) / (l_prob + r_prob + 0.0001)
    safe_idxs = np.isfinite(self.ll_t)
    if safe_idxs[0]:
      lane_path_y_interp = np.interp(path_t, self.ll_t[safe_idxs], lane_path_y[safe_idxs])
      path_xyz[:,1] = self.d_prob * lane_path_y_interp + (1.0 - self.d_prob) * path_xyz[:,1]
    else:
      cloudlog.warning("Lateral mpc - NaNs in laneline times, ignoring")
    return path_xyz
