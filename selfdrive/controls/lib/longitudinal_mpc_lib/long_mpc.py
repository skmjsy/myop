#!/usr/bin/env python3
import os
import numpy as np

from common.realtime import sec_since_boot
from common.numpy_fast import clip, interp
from selfdrive.swaglog import cloudlog
from selfdrive.modeld.constants import index_function
from selfdrive.controls.lib.radar_helpers import _LEAD_ACCEL_TAU
from common.conversions import Conversions as CV
from selfdrive.ntune import ntune_scc_get

if __name__ == '__main__':  # generating code
  from pyextra.acados_template import AcadosModel, AcadosOcp, AcadosOcpSolver
else:
  from selfdrive.controls.lib.longitudinal_mpc_lib.c_generated_code.acados_ocp_solver_pyx import AcadosOcpSolverCython  # pylint: disable=no-name-in-module, import-error

from casadi import SX, vertcat

MODEL_NAME = 'long'
LONG_MPC_DIR = os.path.dirname(os.path.abspath(__file__))
EXPORT_DIR = os.path.join(LONG_MPC_DIR, "c_generated_code")
JSON_FILE = os.path.join(LONG_MPC_DIR, "acados_ocp_long.json")

SOURCES = ['lead0', 'lead1', 'cruise', 'stop']

X_DIM = 3
U_DIM = 1
PARAM_DIM = 5
COST_E_DIM = 5
COST_DIM = COST_E_DIM + 1
CONSTR_DIM = 4

X_EGO_OBSTACLE_COST = 3.
X_EGO_COST = 0.
V_EGO_COST = 0.
A_EGO_COST = 0.
J_EGO_COST = 5.0
A_CHANGE_COST = 50. # 200.# 낮을수록 선행차에 민강하게 반응. def:0.5
DANGER_ZONE_COST = 100.
CRASH_DISTANCE = .5
LIMIT_COST = 1e6
ACADOS_SOLVER_TYPE = 'SQP_RTI'


CRUISE_GAP_BP = [1., 2., 3., 4.]
CRUISE_GAP_V = [1.1, 1.3, 1.58, 2.10]

AUTO_TR_BP = [0., 50.*CV.KPH_TO_MS, 100.*CV.KPH_TO_MS, 130.*CV.KPH_TO_MS]
AUTO_TR_V = [1.0, 1.2, 1.3, 1.4]

AUTO_TR_CRUISE_GAP = 4
DIFF_RADAR_VISION = 1.5


# Fewer timestamps don't hurt performance and lead to
# much better convergence of the MPC with low iterations
N = 12
MAX_T = 10.0
T_IDXS_LST = [index_function(idx, max_val=MAX_T, max_idx=N) for idx in range(N+1)]

T_IDXS = np.array(T_IDXS_LST)
T_DIFFS = np.diff(T_IDXS, prepend=[0.])
MIN_ACCEL = -3.5
T_FOLLOW = 1.45
COMFORT_BRAKE = ntune_scc_get("COMFORT_BRAKE") #2.5
STOP_DISTANCE = ntune_scc_get("STOP_DISTANCE") #6.0

def get_stopped_equivalence_factor(v_lead):
  return (v_lead**2) / (2 * COMFORT_BRAKE)

def get_safe_obstacle_distance(v_ego, tr):
  return (v_ego**2) / (2 * COMFORT_BRAKE) + tr * v_ego + STOP_DISTANCE

def desired_follow_distance(v_ego, v_lead, tr):
  return get_safe_obstacle_distance(v_ego, tr) - get_stopped_equivalence_factor(v_lead)


def gen_long_model():
  model = AcadosModel()
  model.name = MODEL_NAME

  # set up states & controls
  x_ego = SX.sym('x_ego')
  v_ego = SX.sym('v_ego')
  a_ego = SX.sym('a_ego')
  model.x = vertcat(x_ego, v_ego, a_ego)

  # controls
  j_ego = SX.sym('j_ego')
  model.u = vertcat(j_ego)

  # xdot
  x_ego_dot = SX.sym('x_ego_dot')
  v_ego_dot = SX.sym('v_ego_dot')
  a_ego_dot = SX.sym('a_ego_dot')
  model.xdot = vertcat(x_ego_dot, v_ego_dot, a_ego_dot)

  # live parameters
  a_min = SX.sym('a_min')
  a_max = SX.sym('a_max')
  x_obstacle = SX.sym('x_obstacle')
  prev_a = SX.sym('prev_a')
  tr = SX.sym('tr')
  model.p = vertcat(a_min, a_max, x_obstacle, prev_a, tr)

  # dynamics model
  f_expl = vertcat(v_ego, a_ego, j_ego)
  model.f_impl_expr = model.xdot - f_expl
  model.f_expl_expr = f_expl
  return model


def gen_long_ocp():
  ocp = AcadosOcp()
  ocp.model = gen_long_model()

  Tf = T_IDXS[-1]

  # set dimensions
  ocp.dims.N = N

  # set cost module
  ocp.cost.cost_type = 'NONLINEAR_LS'
  ocp.cost.cost_type_e = 'NONLINEAR_LS'

  QR = np.zeros((COST_DIM, COST_DIM))
  Q = np.zeros((COST_E_DIM, COST_E_DIM))

  ocp.cost.W = QR
  ocp.cost.W_e = Q

  x_ego, v_ego, a_ego = ocp.model.x[0], ocp.model.x[1], ocp.model.x[2]
  j_ego = ocp.model.u[0]

  a_min, a_max = ocp.model.p[0], ocp.model.p[1]
  x_obstacle = ocp.model.p[2]
  prev_a = ocp.model.p[3]
  tr = ocp.model.p[4]

  ocp.cost.yref = np.zeros((COST_DIM, ))
  ocp.cost.yref_e = np.zeros((COST_E_DIM, ))

  desired_dist_comfort = get_safe_obstacle_distance(v_ego, tr)

  # The main cost in normal operation is how close you are to the "desired" distance
  # from an obstacle at every timestep. This obstacle can be a lead car
  # or other object. In e2e mode we can use x_position targets as a cost
  # instead.
  costs = [((x_obstacle - x_ego) - (desired_dist_comfort)) / (v_ego + 10.),
           x_ego,
           v_ego,
           a_ego,
           a_ego - prev_a,
           j_ego]
  ocp.model.cost_y_expr = vertcat(*costs)
  ocp.model.cost_y_expr_e = vertcat(*costs[:-1])

  # Constraints on speed, acceleration and desired distance to
  # the obstacle, which is treated as a slack constraint so it
  # behaves like an asymmetrical cost.
  constraints = vertcat(v_ego,
                        (a_ego - a_min),
                        (a_max - a_ego),
                        ((x_obstacle - x_ego) - (3/4) * (desired_dist_comfort)) / (v_ego + 10.))
  ocp.model.con_h_expr = constraints

  x0 = np.zeros(X_DIM)
  ocp.constraints.x0 = x0
  ocp.parameter_values = np.array([-1.2, 1.2, 0.0, 0.0, T_FOLLOW])

  # We put all constraint cost weights to 0 and only set them at runtime
  cost_weights = np.zeros(CONSTR_DIM)
  ocp.cost.zl = cost_weights
  ocp.cost.Zl = cost_weights
  ocp.cost.Zu = cost_weights
  ocp.cost.zu = cost_weights

  ocp.constraints.lh = np.zeros(CONSTR_DIM)
  ocp.constraints.uh = 1e4*np.ones(CONSTR_DIM)
  ocp.constraints.idxsh = np.arange(CONSTR_DIM)

  # The HPIPM solver can give decent solutions even when it is stopped early
  # Which is critical for our purpose where compute time is strictly bounded
  # We use HPIPM in the SPEED_ABS mode, which ensures fastest runtime. This
  # does not cause issues since the problem is well bounded.
  ocp.solver_options.qp_solver = 'PARTIAL_CONDENSING_HPIPM'
  ocp.solver_options.hessian_approx = 'GAUSS_NEWTON'
  ocp.solver_options.integrator_type = 'ERK'
  ocp.solver_options.nlp_solver_type = ACADOS_SOLVER_TYPE
  ocp.solver_options.qp_solver_cond_N = 1

  # More iterations take too much time and less lead to inaccurate convergence in
  # some situations. Ideally we would run just 1 iteration to ensure fixed runtime.
  ocp.solver_options.qp_solver_iter_max = 10
  ocp.solver_options.qp_tol = 1e-3

  # set prediction horizon
  ocp.solver_options.tf = Tf
  ocp.solver_options.shooting_nodes = T_IDXS

  ocp.code_export_directory = EXPORT_DIR
  return ocp


class LongitudinalMpc:
  def __init__(self, e2e=False):
    self.e2e = e2e
    self.solver = AcadosOcpSolverCython(MODEL_NAME, ACADOS_SOLVER_TYPE, N)
    self.reset()
    self.source = SOURCES[2]
    #apilot
    self.trafficState = 0
    #oprk
    self.v_ego = 0.
    self.lead_0_obstacle = np.zeros(13, dtype=np.float64)
    self.lead_1_obstacle = np.zeros(13, dtype=np.float64)
    self.e2e_x = np.zeros(13, dtype=np.float64)
    self.cruise_target = np.zeros(13, dtype=np.float64)
    self.stopline = np.zeros(13, dtype=np.float64)
    self.stop_prob = 0.0
    self.on_stopping = False
    self.stop_line = ntune_scc_get("StopAtStopSign")
    self.x_ego_obstacle_cost = ntune_scc_get("X_EGO_OBSTACLE_COST")
    self.stop_line_offset = ntune_scc_get("STOP_LINE_OFFSET")
    self.lo_timer = 0 

  def reset(self):
    # self.solver = AcadosOcpSolverCython(MODEL_NAME, ACADOS_SOLVER_TYPE, N)
    self.solver.reset()
    # self.solver.options_set('print_level', 2)
    self.v_solution = np.zeros(N+1)
    self.a_solution = np.zeros(N+1)
    self.prev_a = np.array(self.a_solution)
    self.j_solution = np.zeros(N)
    self.yref = np.zeros((N+1, COST_DIM))
    for i in range(N):
      self.solver.cost_set(i, "yref", self.yref[i])
    self.solver.cost_set(N, "yref", self.yref[N][:COST_E_DIM])
    self.x_sol = np.zeros((N+1, X_DIM))
    self.u_sol = np.zeros((N,1))
    self.params = np.zeros((N+1, PARAM_DIM))
    self.param_tr = T_FOLLOW
    self.x_ego_obstacle_cost = X_EGO_OBSTACLE_COST
    self.stop_line_offset = 1.0
    for i in range(N+1):
      self.solver.set(i, 'x', np.zeros(X_DIM))
    self.last_cloudlog_t = 0
    self.status = False
    self.crash_cnt = 0.0
    self.solution_status = 0
    # timers
    self.solve_time = 0.0
    self.time_qp_solution = 0.0
    self.time_linearization = 0.0
    self.time_integrator = 0.0
    self.x0 = np.zeros(X_DIM)
    self.set_weights()

  def set_cost_weights(self, cost_weights, constraint_cost_weights):
    W = np.asfortranarray(np.diag(cost_weights))
    for i in range(N):
      # TODO don't hardcode A_CHANGE_COST idx
      # reduce the cost on (a-a_prev) later in the horizon.
      W[4,4] = cost_weights[4] * np.interp(T_IDXS[i], [0.0, 1.0, 2.0], [1.0, 1.0, 0.0])
      self.solver.cost_set(i, 'W', W)
    # Setting the slice without the copy make the array not contiguous,
    # causing issues with the C interface.
    self.solver.cost_set(N, 'W', np.copy(W[:COST_E_DIM, :COST_E_DIM]))

    # Set L2 slack cost on lower bound constraints
    Zl = np.array(constraint_cost_weights)
    for i in range(N):
      self.solver.cost_set(i, 'Zl', Zl)

  def set_weights(self, prev_accel_constraint=True):
    a_change_cost = A_CHANGE_COST if prev_accel_constraint else 0
    cost_weights = [self.x_ego_obstacle_cost, X_EGO_COST, V_EGO_COST, A_EGO_COST, a_change_cost, J_EGO_COST]
    constraint_cost_weights = [LIMIT_COST, LIMIT_COST, LIMIT_COST, DANGER_ZONE_COST]

    self.set_cost_weights(cost_weights, constraint_cost_weights)

  def set_cur_state(self, v, a):
    v_prev = self.x0[1]
    self.x0[1] = v
    self.x0[2] = a
    if abs(v_prev - v) > 2.: # probably only helps if v < v_prev
      for i in range(0, N+1):
        self.solver.set(i, 'x', self.x0)

  @staticmethod
  def extrapolate_lead(x_lead, v_lead, a_lead, a_lead_tau):
    a_lead_traj = a_lead * np.exp(-a_lead_tau * (T_IDXS**2)/2.)
    v_lead_traj = np.clip(v_lead + np.cumsum(T_DIFFS * a_lead_traj), 0.0, 1e8)
    x_lead_traj = x_lead + np.cumsum(T_DIFFS * v_lead_traj)
    lead_xv = np.column_stack((x_lead_traj, v_lead_traj))
    return lead_xv

  def process_lead(self, lead):
    v_ego = self.x0[1]
    if lead is not None and lead.status:
      x_lead = lead.dRel if lead.radar else max(lead.dRel - DIFF_RADAR_VISION, 0.)
      v_lead = lead.vLead
      a_lead = lead.aLeadK
      a_lead_tau = lead.aLeadTau
    else:
      # Fake a fast lead car, so mpc can keep running in the same mode
      x_lead = 50.0
      v_lead = v_ego + 10.0
      a_lead = 0.0
      a_lead_tau = _LEAD_ACCEL_TAU

    # MPC will not converge if immediate crash is expected
    # Clip lead distance to what is still possible to brake for
    min_x_lead = ((v_ego + v_lead)/2) * (v_ego - v_lead) / (-MIN_ACCEL * 2)
    x_lead = clip(x_lead, min_x_lead, 1e8)
    v_lead = clip(v_lead, 0.0, 1e8)
    a_lead = clip(a_lead, -10., 5.)
    lead_xv = self.extrapolate_lead(x_lead, v_lead, a_lead, a_lead_tau)
    return lead_xv

  def set_accel_limits(self, min_a, max_a):
    self.cruise_min_a = min_a
    self.cruise_max_a = max_a

  def update(self, carstate, radarstate, model, v_cruise, x, v, a, j, prev_accel_constraint):
    #opkr
    self.lo_timer += 1
    if self.lo_timer > 200:
      self.lo_timer = 0
      self.stop_line_offset = ntune_scc_get("STOP_LINE_OFFSET")

    #apilot
    self.trafficState = 0

    v_ego = self.x0[1]

    #opkr
    #####################################################################
    xforward = ((v[1:] + v[:-1]) / 2) * (T_IDXS[1:] - T_IDXS[:-1])
    x = np.cumsum(np.insert(xforward, 0, x[0]))
    self.yref[:,1] = x
    self.yref[:,2] = v
    self.yref[:,3] = a
    #####################################################################

    self.status = radarstate.leadOne.status or radarstate.leadTwo.status

    lead_xv_0 = self.process_lead(radarstate.leadOne)
    lead_xv_1 = self.process_lead(radarstate.leadTwo)

    # set accel limits in params
    self.params[:,0] = interp(float(self.status), [0.0, 1.0], [self.cruise_min_a, MIN_ACCEL])
    self.params[:,1] = self.cruise_max_a

    # neokii
    cruise_gap = int(clip(carstate.cruiseGap, 1., 4.))
    if cruise_gap == AUTO_TR_CRUISE_GAP:
      tr = interp(carstate.vEgo, AUTO_TR_BP, AUTO_TR_V)
    else:
      tr = interp(float(cruise_gap), CRUISE_GAP_BP, CRUISE_GAP_V)

    self.param_tr = tr

    # To estimate a safe distance from a moving lead, we calculate how much stopping
    # distance that lead needs as a minimum. We can add that to the current distance
    # and then treat that as a stopped car/obstacle at this new distance.
    lead_0_obstacle = lead_xv_0[:,0] + get_stopped_equivalence_factor(lead_xv_0[:,1])
    lead_1_obstacle = lead_xv_1[:,0] + get_stopped_equivalence_factor(lead_xv_1[:,1])

    # Fake an obstacle for cruise, this ensures smooth acceleration to set speed
    # when the leads are no factor.
    v_lower = v_ego + (T_IDXS * self.cruise_min_a * 1.05)
    v_upper = v_ego + (T_IDXS * self.cruise_max_a * 1.05)
    v_cruise_clipped = np.clip(v_cruise * np.ones(N+1),
                                 v_lower,
                                 v_upper)

    cruise_obstacle = np.cumsum(T_DIFFS * v_cruise_clipped) + get_safe_obstacle_distance(v_cruise_clipped, tr)

    self.x_ego_obstacle_cost = X_EGO_OBSTACLE_COST
    self.set_weights(prev_accel_constraint)
    x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle])


    #apilot
    probe = model.stopLine.prob if abs(carstate.steeringAngleDeg) < 20 else 0.0
    startSign = v[-1] > 5.0
    stopSign = (probe > 0.3) and ((v[-1] < 3.0) or (v[-1] < v_ego*0.95))
    stopline_x = (model.stopLine.x)
    
    if self.status and (radarstate.leadOne.dRel - x[N]) < 2.0:
      self.trafficState = 0 # "OFF"  onroad.cc - trafficLight 
    elif stopSign:
      self.trafficState = 1 # "RED"
    elif startSign:
      self.trafficState = 2 # "GREEN"

    stopline = (stopline_x) * np.ones(N+1) if (stopSign) else 400.0 * np.ones(N+1)
    x = (x[N]) * np.ones(N+1)

    stopping = True if (self.stop_line and self.trafficState == 1 and not self.status and stopline_x < 100) else False

    #self.stop_line_offset = interp(self.v_ego, [0, 10, 20, 25, 30], [1.0, 0.9, 0.8, 0.6, 0.4]) #10m/s = 22mph, 15m/s = 33mph 54kph, 20m/s = 45mph 72kph, 25m/s = 56mph, 30m/s = 67mph 108kph
    #self.stop_line_offset = interp(self.v_ego, [0, 5, 15, 20, 25, 35, 40, 45, 50], [1.0, 1.10, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 2.0]) #
    self.stop_line_offset = interp(self.v_ego*CV.MS_TO_MPH, [0, 5, 15, 20, 25, 35, 40, 45, 50], [1.0, 1.10, 1.15, 1.25, 1.35, 1.45, 1.55, 1.65, 2.0]) #

    if stopping:
     self.on_stopping = True
     self.param_tr = 0
     self.x_ego_obstacle_cost = ntune_scc_get("X_EGO_OBSTACLE_COST")
     self.set_weights(prev_accel_constraint)
     cruise_obstacle = np.cumsum(T_DIFFS * v_cruise_clipped) + get_safe_obstacle_distance(v_cruise_clipped, 0)
     x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle, stopline * self.stop_line_offset])

    #opkr
    #####################################################################################################################
    # stopping = True if (self.stop_line and self.trafficState == 1 and not self.status) else False

    # stopline = (model.stopLine.x + 5.0) * np.ones(N+1) if stopping else 400 * np.ones(N+1)
    # x = (x[N] + 5.0) * np.ones(N+1)

    # #self.stop_line_offset = interp(self.v_ego, [0, 10, 20, 25, 30], [0.9, 0.8, 0.6, 0.2, 0.0]) #15m/s = 33mph 54kph,  20m/s 45mph 72kph, 30m/s = 67mph 108kph

    # if stopping:
    #   self.x_ego_obstacle_cost = ntune_scc_get("X_EGO_OBSTACLE_COST")
    #   self.set_weights(prev_accel_constraint)

    # if self.status and not self.on_stopping:
    #   x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle])
    # elif x[N] > 30 and stopline[N] < 30 and self.v_ego < 6.0:  # < 13mph 21.6kph
    #   self.on_stopping = False
    #   x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle, x])
    # elif x[N] < 150 and stopline[N] < 150:
    #   self.on_stopping = True
    #   x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle*2, (stopline*0.2)+(x*self.stop_line_offset)])
    # elif x[N] < 150 and self.on_stopping:
    #   x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle*2, x])
    # else:
    #   self.on_stopping = False
    #   x_obstacles = np.column_stack([lead_0_obstacle, lead_1_obstacle, cruise_obstacle])
    #####################################################################################################################

    self.source = SOURCES[np.argmin(x_obstacles[N])]

    self.params[:,2] = np.min(x_obstacles, axis=1)
    self.params[:,3] = np.copy(self.prev_a)
    self.params[:,4] = self.param_tr

    #opkr
    self.e2e_x = x[:]
    self.lead_0_obstacle = lead_0_obstacle[:]
    self.lead_1_obstacle = lead_1_obstacle[:]
    self.cruise_target = cruise_obstacle[:]
    self.stopline = stopline[:]
    self.stop_prob = model.stopLine.prob

    self.run()
    if (np.any(lead_xv_0[:,0] - self.x_sol[:,0] < CRASH_DISTANCE) and
            radarstate.leadOne.modelProb > 0.9):
      self.crash_cnt += 1
    else:
      self.crash_cnt = 0

  def run(self):
    # t0 = sec_since_boot()
    # reset = 0
    for i in range(N+1):
      self.solver.set(i, 'p', self.params[i])
    self.solver.constraints_set(0, "lbx", self.x0)
    self.solver.constraints_set(0, "ubx", self.x0)

    self.solution_status = self.solver.solve()
    self.solve_time = float(self.solver.get_stats('time_tot')[0])
    self.time_qp_solution = float(self.solver.get_stats('time_qp')[0])
    self.time_linearization = float(self.solver.get_stats('time_lin')[0])
    self.time_integrator = float(self.solver.get_stats('time_sim')[0])

    # qp_iter = self.solver.get_stats('statistics')[-1][-1] # SQP_RTI specific
    # print(f"long_mpc timings: tot {self.solve_time:.2e}, qp {self.time_qp_solution:.2e}, lin {self.time_linearization:.2e}, integrator {self.time_integrator:.2e}, qp_iter {qp_iter}")
    # res = self.solver.get_residuals()
    # print(f"long_mpc residuals: {res[0]:.2e}, {res[1]:.2e}, {res[2]:.2e}, {res[3]:.2e}")
    # self.solver.print_statistics()

    for i in range(N+1):
      self.x_sol[i] = self.solver.get(i, 'x')
    for i in range(N):
      self.u_sol[i] = self.solver.get(i, 'u')

    self.v_solution = self.x_sol[:,1]
    self.a_solution = self.x_sol[:,2]
    self.j_solution = self.u_sol[:,0]

    self.prev_a = np.interp(T_IDXS + 0.05, T_IDXS, self.a_solution)

    t = sec_since_boot()
    if self.solution_status != 0:
      if t > self.last_cloudlog_t + 5.0:
        self.last_cloudlog_t = t
        cloudlog.warning(f"Long mpc reset, solution_status: {self.solution_status}")
      self.reset()
      # reset = 1
    # print(f"long_mpc timings: total internal {self.solve_time:.2e}, external: {(sec_since_boot() - t0):.2e} qp {self.time_qp_solution:.2e}, lin {self.time_linearization:.2e} qp_iter {qp_iter}, reset {reset}")


if __name__ == "__main__":
  ocp = gen_long_ocp()
  AcadosOcpSolver.generate(ocp, json_file=JSON_FILE)
  # AcadosOcpSolver.build(ocp.code_export_directory, with_cython=True)

