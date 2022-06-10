#include "car.h"

namespace {
#define DIM 8
#define EDIM 8
#define MEDIM 8
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 5.991464547107981;

/******************************************************************************
 *                       Code generated with sympy 1.8                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_2428597387138040012) {
   out_2428597387138040012[0] = delta_x[0] + nom_x[0];
   out_2428597387138040012[1] = delta_x[1] + nom_x[1];
   out_2428597387138040012[2] = delta_x[2] + nom_x[2];
   out_2428597387138040012[3] = delta_x[3] + nom_x[3];
   out_2428597387138040012[4] = delta_x[4] + nom_x[4];
   out_2428597387138040012[5] = delta_x[5] + nom_x[5];
   out_2428597387138040012[6] = delta_x[6] + nom_x[6];
   out_2428597387138040012[7] = delta_x[7] + nom_x[7];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2035103532906739771) {
   out_2035103532906739771[0] = -nom_x[0] + true_x[0];
   out_2035103532906739771[1] = -nom_x[1] + true_x[1];
   out_2035103532906739771[2] = -nom_x[2] + true_x[2];
   out_2035103532906739771[3] = -nom_x[3] + true_x[3];
   out_2035103532906739771[4] = -nom_x[4] + true_x[4];
   out_2035103532906739771[5] = -nom_x[5] + true_x[5];
   out_2035103532906739771[6] = -nom_x[6] + true_x[6];
   out_2035103532906739771[7] = -nom_x[7] + true_x[7];
}
void H_mod_fun(double *state, double *out_928234429571031611) {
   out_928234429571031611[0] = 1.0;
   out_928234429571031611[1] = 0.0;
   out_928234429571031611[2] = 0.0;
   out_928234429571031611[3] = 0.0;
   out_928234429571031611[4] = 0.0;
   out_928234429571031611[5] = 0.0;
   out_928234429571031611[6] = 0.0;
   out_928234429571031611[7] = 0.0;
   out_928234429571031611[8] = 0.0;
   out_928234429571031611[9] = 1.0;
   out_928234429571031611[10] = 0.0;
   out_928234429571031611[11] = 0.0;
   out_928234429571031611[12] = 0.0;
   out_928234429571031611[13] = 0.0;
   out_928234429571031611[14] = 0.0;
   out_928234429571031611[15] = 0.0;
   out_928234429571031611[16] = 0.0;
   out_928234429571031611[17] = 0.0;
   out_928234429571031611[18] = 1.0;
   out_928234429571031611[19] = 0.0;
   out_928234429571031611[20] = 0.0;
   out_928234429571031611[21] = 0.0;
   out_928234429571031611[22] = 0.0;
   out_928234429571031611[23] = 0.0;
   out_928234429571031611[24] = 0.0;
   out_928234429571031611[25] = 0.0;
   out_928234429571031611[26] = 0.0;
   out_928234429571031611[27] = 1.0;
   out_928234429571031611[28] = 0.0;
   out_928234429571031611[29] = 0.0;
   out_928234429571031611[30] = 0.0;
   out_928234429571031611[31] = 0.0;
   out_928234429571031611[32] = 0.0;
   out_928234429571031611[33] = 0.0;
   out_928234429571031611[34] = 0.0;
   out_928234429571031611[35] = 0.0;
   out_928234429571031611[36] = 1.0;
   out_928234429571031611[37] = 0.0;
   out_928234429571031611[38] = 0.0;
   out_928234429571031611[39] = 0.0;
   out_928234429571031611[40] = 0.0;
   out_928234429571031611[41] = 0.0;
   out_928234429571031611[42] = 0.0;
   out_928234429571031611[43] = 0.0;
   out_928234429571031611[44] = 0.0;
   out_928234429571031611[45] = 1.0;
   out_928234429571031611[46] = 0.0;
   out_928234429571031611[47] = 0.0;
   out_928234429571031611[48] = 0.0;
   out_928234429571031611[49] = 0.0;
   out_928234429571031611[50] = 0.0;
   out_928234429571031611[51] = 0.0;
   out_928234429571031611[52] = 0.0;
   out_928234429571031611[53] = 0.0;
   out_928234429571031611[54] = 1.0;
   out_928234429571031611[55] = 0.0;
   out_928234429571031611[56] = 0.0;
   out_928234429571031611[57] = 0.0;
   out_928234429571031611[58] = 0.0;
   out_928234429571031611[59] = 0.0;
   out_928234429571031611[60] = 0.0;
   out_928234429571031611[61] = 0.0;
   out_928234429571031611[62] = 0.0;
   out_928234429571031611[63] = 1.0;
}
void f_fun(double *state, double dt, double *out_3029201977852988162) {
   out_3029201977852988162[0] = state[0];
   out_3029201977852988162[1] = state[1];
   out_3029201977852988162[2] = state[2];
   out_3029201977852988162[3] = state[3];
   out_3029201977852988162[4] = state[4];
   out_3029201977852988162[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3029201977852988162[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3029201977852988162[7] = state[7];
}
void F_fun(double *state, double dt, double *out_1663015017050046851) {
   out_1663015017050046851[0] = 1;
   out_1663015017050046851[1] = 0;
   out_1663015017050046851[2] = 0;
   out_1663015017050046851[3] = 0;
   out_1663015017050046851[4] = 0;
   out_1663015017050046851[5] = 0;
   out_1663015017050046851[6] = 0;
   out_1663015017050046851[7] = 0;
   out_1663015017050046851[8] = 0;
   out_1663015017050046851[9] = 1;
   out_1663015017050046851[10] = 0;
   out_1663015017050046851[11] = 0;
   out_1663015017050046851[12] = 0;
   out_1663015017050046851[13] = 0;
   out_1663015017050046851[14] = 0;
   out_1663015017050046851[15] = 0;
   out_1663015017050046851[16] = 0;
   out_1663015017050046851[17] = 0;
   out_1663015017050046851[18] = 1;
   out_1663015017050046851[19] = 0;
   out_1663015017050046851[20] = 0;
   out_1663015017050046851[21] = 0;
   out_1663015017050046851[22] = 0;
   out_1663015017050046851[23] = 0;
   out_1663015017050046851[24] = 0;
   out_1663015017050046851[25] = 0;
   out_1663015017050046851[26] = 0;
   out_1663015017050046851[27] = 1;
   out_1663015017050046851[28] = 0;
   out_1663015017050046851[29] = 0;
   out_1663015017050046851[30] = 0;
   out_1663015017050046851[31] = 0;
   out_1663015017050046851[32] = 0;
   out_1663015017050046851[33] = 0;
   out_1663015017050046851[34] = 0;
   out_1663015017050046851[35] = 0;
   out_1663015017050046851[36] = 1;
   out_1663015017050046851[37] = 0;
   out_1663015017050046851[38] = 0;
   out_1663015017050046851[39] = 0;
   out_1663015017050046851[40] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1663015017050046851[41] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1663015017050046851[42] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1663015017050046851[43] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1663015017050046851[44] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1663015017050046851[45] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1663015017050046851[46] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1663015017050046851[47] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1663015017050046851[48] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1663015017050046851[49] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1663015017050046851[50] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1663015017050046851[51] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1663015017050046851[52] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1663015017050046851[53] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1663015017050046851[54] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1663015017050046851[55] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1663015017050046851[56] = 0;
   out_1663015017050046851[57] = 0;
   out_1663015017050046851[58] = 0;
   out_1663015017050046851[59] = 0;
   out_1663015017050046851[60] = 0;
   out_1663015017050046851[61] = 0;
   out_1663015017050046851[62] = 0;
   out_1663015017050046851[63] = 1;
}
void h_25(double *state, double *unused, double *out_737311645957241244) {
   out_737311645957241244[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1773621138033824960) {
   out_1773621138033824960[0] = 0;
   out_1773621138033824960[1] = 0;
   out_1773621138033824960[2] = 0;
   out_1773621138033824960[3] = 0;
   out_1773621138033824960[4] = 0;
   out_1773621138033824960[5] = 0;
   out_1773621138033824960[6] = 1;
   out_1773621138033824960[7] = 0;
}
void h_24(double *state, double *unused, double *out_878871673552023650) {
   out_878871673552023650[0] = state[4];
   out_878871673552023650[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4539615154683746688) {
   out_4539615154683746688[0] = 0;
   out_4539615154683746688[1] = 0;
   out_4539615154683746688[2] = 0;
   out_4539615154683746688[3] = 0;
   out_4539615154683746688[4] = 1;
   out_4539615154683746688[5] = 0;
   out_4539615154683746688[6] = 0;
   out_4539615154683746688[7] = 0;
   out_4539615154683746688[8] = 0;
   out_4539615154683746688[9] = 0;
   out_4539615154683746688[10] = 0;
   out_4539615154683746688[11] = 0;
   out_4539615154683746688[12] = 0;
   out_4539615154683746688[13] = 1;
   out_4539615154683746688[14] = 0;
   out_4539615154683746688[15] = 0;
}
void h_30(double *state, double *unused, double *out_776950091763988880) {
   out_776950091763988880[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7059224024726543376) {
   out_7059224024726543376[0] = 0;
   out_7059224024726543376[1] = 0;
   out_7059224024726543376[2] = 0;
   out_7059224024726543376[3] = 0;
   out_7059224024726543376[4] = 1;
   out_7059224024726543376[5] = 0;
   out_7059224024726543376[6] = 0;
   out_7059224024726543376[7] = 0;
}
void h_26(double *state, double *unused, double *out_4224723175896856765) {
   out_4224723175896856765[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3115057271729472512) {
   out_3115057271729472512[0] = 0;
   out_3115057271729472512[1] = 0;
   out_3115057271729472512[2] = 0;
   out_3115057271729472512[3] = 0;
   out_3115057271729472512[4] = 0;
   out_3115057271729472512[5] = 0;
   out_3115057271729472512[6] = 0;
   out_3115057271729472512[7] = 1;
}
void h_27(double *state, double *unused, double *out_4106715781963480021) {
   out_4106715781963480021[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1274387251744938761) {
   out_1274387251744938761[0] = 0;
   out_1274387251744938761[1] = 0;
   out_1274387251744938761[2] = 0;
   out_1274387251744938761[3] = 1;
   out_1274387251744938761[4] = 0;
   out_1274387251744938761[5] = 0;
   out_1274387251744938761[6] = 0;
   out_1274387251744938761[7] = 0;
}
void h_29(double *state, double *unused, double *out_5202311007062220413) {
   out_5202311007062220413[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7633782638582079551) {
   out_7633782638582079551[0] = 0;
   out_7633782638582079551[1] = 1;
   out_7633782638582079551[2] = 0;
   out_7633782638582079551[3] = 0;
   out_7633782638582079551[4] = 0;
   out_7633782638582079551[5] = 0;
   out_7633782638582079551[6] = 0;
   out_7633782638582079551[7] = 0;
}
void h_28(double *state, double *unused, double *out_7658855218861588919) {
   out_7658855218861588919[0] = state[5];
   out_7658855218861588919[1] = state[6];
}
void H_28(double *state, double *unused, double *out_2478217323552748178) {
   out_2478217323552748178[0] = 0;
   out_2478217323552748178[1] = 0;
   out_2478217323552748178[2] = 0;
   out_2478217323552748178[3] = 0;
   out_2478217323552748178[4] = 0;
   out_2478217323552748178[5] = 1;
   out_2478217323552748178[6] = 0;
   out_2478217323552748178[7] = 0;
   out_2478217323552748178[8] = 0;
   out_2478217323552748178[9] = 0;
   out_2478217323552748178[10] = 0;
   out_2478217323552748178[11] = 0;
   out_2478217323552748178[12] = 0;
   out_2478217323552748178[13] = 0;
   out_2478217323552748178[14] = 1;
   out_2478217323552748178[15] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_2428597387138040012) {
  err_fun(nom_x, delta_x, out_2428597387138040012);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2035103532906739771) {
  inv_err_fun(nom_x, true_x, out_2035103532906739771);
}
void car_H_mod_fun(double *state, double *out_928234429571031611) {
  H_mod_fun(state, out_928234429571031611);
}
void car_f_fun(double *state, double dt, double *out_3029201977852988162) {
  f_fun(state,  dt, out_3029201977852988162);
}
void car_F_fun(double *state, double dt, double *out_1663015017050046851) {
  F_fun(state,  dt, out_1663015017050046851);
}
void car_h_25(double *state, double *unused, double *out_737311645957241244) {
  h_25(state, unused, out_737311645957241244);
}
void car_H_25(double *state, double *unused, double *out_1773621138033824960) {
  H_25(state, unused, out_1773621138033824960);
}
void car_h_24(double *state, double *unused, double *out_878871673552023650) {
  h_24(state, unused, out_878871673552023650);
}
void car_H_24(double *state, double *unused, double *out_4539615154683746688) {
  H_24(state, unused, out_4539615154683746688);
}
void car_h_30(double *state, double *unused, double *out_776950091763988880) {
  h_30(state, unused, out_776950091763988880);
}
void car_H_30(double *state, double *unused, double *out_7059224024726543376) {
  H_30(state, unused, out_7059224024726543376);
}
void car_h_26(double *state, double *unused, double *out_4224723175896856765) {
  h_26(state, unused, out_4224723175896856765);
}
void car_H_26(double *state, double *unused, double *out_3115057271729472512) {
  H_26(state, unused, out_3115057271729472512);
}
void car_h_27(double *state, double *unused, double *out_4106715781963480021) {
  h_27(state, unused, out_4106715781963480021);
}
void car_H_27(double *state, double *unused, double *out_1274387251744938761) {
  H_27(state, unused, out_1274387251744938761);
}
void car_h_29(double *state, double *unused, double *out_5202311007062220413) {
  h_29(state, unused, out_5202311007062220413);
}
void car_H_29(double *state, double *unused, double *out_7633782638582079551) {
  H_29(state, unused, out_7633782638582079551);
}
void car_h_28(double *state, double *unused, double *out_7658855218861588919) {
  h_28(state, unused, out_7658855218861588919);
}
void car_H_28(double *state, double *unused, double *out_2478217323552748178) {
  H_28(state, unused, out_2478217323552748178);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_init(car);
