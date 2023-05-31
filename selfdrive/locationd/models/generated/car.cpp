#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
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
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with sympy 1.9                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7719621718569340755) {
   out_7719621718569340755[0] = delta_x[0] + nom_x[0];
   out_7719621718569340755[1] = delta_x[1] + nom_x[1];
   out_7719621718569340755[2] = delta_x[2] + nom_x[2];
   out_7719621718569340755[3] = delta_x[3] + nom_x[3];
   out_7719621718569340755[4] = delta_x[4] + nom_x[4];
   out_7719621718569340755[5] = delta_x[5] + nom_x[5];
   out_7719621718569340755[6] = delta_x[6] + nom_x[6];
   out_7719621718569340755[7] = delta_x[7] + nom_x[7];
   out_7719621718569340755[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_7132580926423717226) {
   out_7132580926423717226[0] = -nom_x[0] + true_x[0];
   out_7132580926423717226[1] = -nom_x[1] + true_x[1];
   out_7132580926423717226[2] = -nom_x[2] + true_x[2];
   out_7132580926423717226[3] = -nom_x[3] + true_x[3];
   out_7132580926423717226[4] = -nom_x[4] + true_x[4];
   out_7132580926423717226[5] = -nom_x[5] + true_x[5];
   out_7132580926423717226[6] = -nom_x[6] + true_x[6];
   out_7132580926423717226[7] = -nom_x[7] + true_x[7];
   out_7132580926423717226[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_9057155257999617967) {
   out_9057155257999617967[0] = 1.0;
   out_9057155257999617967[1] = 0;
   out_9057155257999617967[2] = 0;
   out_9057155257999617967[3] = 0;
   out_9057155257999617967[4] = 0;
   out_9057155257999617967[5] = 0;
   out_9057155257999617967[6] = 0;
   out_9057155257999617967[7] = 0;
   out_9057155257999617967[8] = 0;
   out_9057155257999617967[9] = 0;
   out_9057155257999617967[10] = 1.0;
   out_9057155257999617967[11] = 0;
   out_9057155257999617967[12] = 0;
   out_9057155257999617967[13] = 0;
   out_9057155257999617967[14] = 0;
   out_9057155257999617967[15] = 0;
   out_9057155257999617967[16] = 0;
   out_9057155257999617967[17] = 0;
   out_9057155257999617967[18] = 0;
   out_9057155257999617967[19] = 0;
   out_9057155257999617967[20] = 1.0;
   out_9057155257999617967[21] = 0;
   out_9057155257999617967[22] = 0;
   out_9057155257999617967[23] = 0;
   out_9057155257999617967[24] = 0;
   out_9057155257999617967[25] = 0;
   out_9057155257999617967[26] = 0;
   out_9057155257999617967[27] = 0;
   out_9057155257999617967[28] = 0;
   out_9057155257999617967[29] = 0;
   out_9057155257999617967[30] = 1.0;
   out_9057155257999617967[31] = 0;
   out_9057155257999617967[32] = 0;
   out_9057155257999617967[33] = 0;
   out_9057155257999617967[34] = 0;
   out_9057155257999617967[35] = 0;
   out_9057155257999617967[36] = 0;
   out_9057155257999617967[37] = 0;
   out_9057155257999617967[38] = 0;
   out_9057155257999617967[39] = 0;
   out_9057155257999617967[40] = 1.0;
   out_9057155257999617967[41] = 0;
   out_9057155257999617967[42] = 0;
   out_9057155257999617967[43] = 0;
   out_9057155257999617967[44] = 0;
   out_9057155257999617967[45] = 0;
   out_9057155257999617967[46] = 0;
   out_9057155257999617967[47] = 0;
   out_9057155257999617967[48] = 0;
   out_9057155257999617967[49] = 0;
   out_9057155257999617967[50] = 1.0;
   out_9057155257999617967[51] = 0;
   out_9057155257999617967[52] = 0;
   out_9057155257999617967[53] = 0;
   out_9057155257999617967[54] = 0;
   out_9057155257999617967[55] = 0;
   out_9057155257999617967[56] = 0;
   out_9057155257999617967[57] = 0;
   out_9057155257999617967[58] = 0;
   out_9057155257999617967[59] = 0;
   out_9057155257999617967[60] = 1.0;
   out_9057155257999617967[61] = 0;
   out_9057155257999617967[62] = 0;
   out_9057155257999617967[63] = 0;
   out_9057155257999617967[64] = 0;
   out_9057155257999617967[65] = 0;
   out_9057155257999617967[66] = 0;
   out_9057155257999617967[67] = 0;
   out_9057155257999617967[68] = 0;
   out_9057155257999617967[69] = 0;
   out_9057155257999617967[70] = 1.0;
   out_9057155257999617967[71] = 0;
   out_9057155257999617967[72] = 0;
   out_9057155257999617967[73] = 0;
   out_9057155257999617967[74] = 0;
   out_9057155257999617967[75] = 0;
   out_9057155257999617967[76] = 0;
   out_9057155257999617967[77] = 0;
   out_9057155257999617967[78] = 0;
   out_9057155257999617967[79] = 0;
   out_9057155257999617967[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_6835096410921863672) {
   out_6835096410921863672[0] = state[0];
   out_6835096410921863672[1] = state[1];
   out_6835096410921863672[2] = state[2];
   out_6835096410921863672[3] = state[3];
   out_6835096410921863672[4] = state[4];
   out_6835096410921863672[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_6835096410921863672[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_6835096410921863672[7] = state[7];
   out_6835096410921863672[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6493140464273918723) {
   out_6493140464273918723[0] = 1;
   out_6493140464273918723[1] = 0;
   out_6493140464273918723[2] = 0;
   out_6493140464273918723[3] = 0;
   out_6493140464273918723[4] = 0;
   out_6493140464273918723[5] = 0;
   out_6493140464273918723[6] = 0;
   out_6493140464273918723[7] = 0;
   out_6493140464273918723[8] = 0;
   out_6493140464273918723[9] = 0;
   out_6493140464273918723[10] = 1;
   out_6493140464273918723[11] = 0;
   out_6493140464273918723[12] = 0;
   out_6493140464273918723[13] = 0;
   out_6493140464273918723[14] = 0;
   out_6493140464273918723[15] = 0;
   out_6493140464273918723[16] = 0;
   out_6493140464273918723[17] = 0;
   out_6493140464273918723[18] = 0;
   out_6493140464273918723[19] = 0;
   out_6493140464273918723[20] = 1;
   out_6493140464273918723[21] = 0;
   out_6493140464273918723[22] = 0;
   out_6493140464273918723[23] = 0;
   out_6493140464273918723[24] = 0;
   out_6493140464273918723[25] = 0;
   out_6493140464273918723[26] = 0;
   out_6493140464273918723[27] = 0;
   out_6493140464273918723[28] = 0;
   out_6493140464273918723[29] = 0;
   out_6493140464273918723[30] = 1;
   out_6493140464273918723[31] = 0;
   out_6493140464273918723[32] = 0;
   out_6493140464273918723[33] = 0;
   out_6493140464273918723[34] = 0;
   out_6493140464273918723[35] = 0;
   out_6493140464273918723[36] = 0;
   out_6493140464273918723[37] = 0;
   out_6493140464273918723[38] = 0;
   out_6493140464273918723[39] = 0;
   out_6493140464273918723[40] = 1;
   out_6493140464273918723[41] = 0;
   out_6493140464273918723[42] = 0;
   out_6493140464273918723[43] = 0;
   out_6493140464273918723[44] = 0;
   out_6493140464273918723[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6493140464273918723[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6493140464273918723[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6493140464273918723[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6493140464273918723[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6493140464273918723[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6493140464273918723[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6493140464273918723[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6493140464273918723[53] = -9.8000000000000007*dt;
   out_6493140464273918723[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6493140464273918723[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6493140464273918723[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6493140464273918723[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6493140464273918723[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6493140464273918723[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6493140464273918723[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6493140464273918723[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6493140464273918723[62] = 0;
   out_6493140464273918723[63] = 0;
   out_6493140464273918723[64] = 0;
   out_6493140464273918723[65] = 0;
   out_6493140464273918723[66] = 0;
   out_6493140464273918723[67] = 0;
   out_6493140464273918723[68] = 0;
   out_6493140464273918723[69] = 0;
   out_6493140464273918723[70] = 1;
   out_6493140464273918723[71] = 0;
   out_6493140464273918723[72] = 0;
   out_6493140464273918723[73] = 0;
   out_6493140464273918723[74] = 0;
   out_6493140464273918723[75] = 0;
   out_6493140464273918723[76] = 0;
   out_6493140464273918723[77] = 0;
   out_6493140464273918723[78] = 0;
   out_6493140464273918723[79] = 0;
   out_6493140464273918723[80] = 1;
}
void h_25(double *state, double *unused, double *out_3161467451908753406) {
   out_3161467451908753406[0] = state[6];
}
void H_25(double *state, double *unused, double *out_3598641471216530860) {
   out_3598641471216530860[0] = 0;
   out_3598641471216530860[1] = 0;
   out_3598641471216530860[2] = 0;
   out_3598641471216530860[3] = 0;
   out_3598641471216530860[4] = 0;
   out_3598641471216530860[5] = 0;
   out_3598641471216530860[6] = 1;
   out_3598641471216530860[7] = 0;
   out_3598641471216530860[8] = 0;
}
void h_24(double *state, double *unused, double *out_951557180481355989) {
   out_951557180481355989[0] = state[4];
   out_951557180481355989[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5771291070222030426) {
   out_5771291070222030426[0] = 0;
   out_5771291070222030426[1] = 0;
   out_5771291070222030426[2] = 0;
   out_5771291070222030426[3] = 0;
   out_5771291070222030426[4] = 1;
   out_5771291070222030426[5] = 0;
   out_5771291070222030426[6] = 0;
   out_5771291070222030426[7] = 0;
   out_5771291070222030426[8] = 0;
   out_5771291070222030426[9] = 0;
   out_5771291070222030426[10] = 0;
   out_5771291070222030426[11] = 0;
   out_5771291070222030426[12] = 0;
   out_5771291070222030426[13] = 0;
   out_5771291070222030426[14] = 1;
   out_5771291070222030426[15] = 0;
   out_5771291070222030426[16] = 0;
   out_5771291070222030426[17] = 0;
}
void h_30(double *state, double *unused, double *out_6421165196907777024) {
   out_6421165196907777024[0] = state[4];
}
void H_30(double *state, double *unused, double *out_1080308512709282233) {
   out_1080308512709282233[0] = 0;
   out_1080308512709282233[1] = 0;
   out_1080308512709282233[2] = 0;
   out_1080308512709282233[3] = 0;
   out_1080308512709282233[4] = 1;
   out_1080308512709282233[5] = 0;
   out_1080308512709282233[6] = 0;
   out_1080308512709282233[7] = 0;
   out_1080308512709282233[8] = 0;
}
void h_26(double *state, double *unused, double *out_9025570517716474675) {
   out_9025570517716474675[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7340144790090587084) {
   out_7340144790090587084[0] = 0;
   out_7340144790090587084[1] = 0;
   out_7340144790090587084[2] = 0;
   out_7340144790090587084[3] = 0;
   out_7340144790090587084[4] = 0;
   out_7340144790090587084[5] = 0;
   out_7340144790090587084[6] = 0;
   out_7340144790090587084[7] = 1;
   out_7340144790090587084[8] = 0;
}
void h_27(double *state, double *unused, double *out_6198253503353086091) {
   out_6198253503353086091[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3255071824509707144) {
   out_3255071824509707144[0] = 0;
   out_3255071824509707144[1] = 0;
   out_3255071824509707144[2] = 0;
   out_3255071824509707144[3] = 1;
   out_3255071824509707144[4] = 0;
   out_3255071824509707144[5] = 0;
   out_3255071824509707144[6] = 0;
   out_3255071824509707144[7] = 0;
   out_3255071824509707144[8] = 0;
}
void h_29(double *state, double *unused, double *out_5477655344006114589) {
   out_5477655344006114589[0] = state[1];
}
void H_29(double *state, double *unused, double *out_570077168394890049) {
   out_570077168394890049[0] = 0;
   out_570077168394890049[1] = 1;
   out_570077168394890049[2] = 0;
   out_570077168394890049[3] = 0;
   out_570077168394890049[4] = 0;
   out_570077168394890049[5] = 0;
   out_570077168394890049[6] = 0;
   out_570077168394890049[7] = 0;
   out_570077168394890049[8] = 0;
}
void h_28(double *state, double *unused, double *out_5594937618676608574) {
   out_5594937618676608574[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5652476185464420623) {
   out_5652476185464420623[0] = 1;
   out_5652476185464420623[1] = 0;
   out_5652476185464420623[2] = 0;
   out_5652476185464420623[3] = 0;
   out_5652476185464420623[4] = 0;
   out_5652476185464420623[5] = 0;
   out_5652476185464420623[6] = 0;
   out_5652476185464420623[7] = 0;
   out_5652476185464420623[8] = 0;
}
void h_31(double *state, double *unused, double *out_4299091312340134919) {
   out_4299091312340134919[0] = state[8];
}
void H_31(double *state, double *unused, double *out_7966352892323938560) {
   out_7966352892323938560[0] = 0;
   out_7966352892323938560[1] = 0;
   out_7966352892323938560[2] = 0;
   out_7966352892323938560[3] = 0;
   out_7966352892323938560[4] = 0;
   out_7966352892323938560[5] = 0;
   out_7966352892323938560[6] = 0;
   out_7966352892323938560[7] = 0;
   out_7966352892323938560[8] = 1;
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
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_7719621718569340755) {
  err_fun(nom_x, delta_x, out_7719621718569340755);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7132580926423717226) {
  inv_err_fun(nom_x, true_x, out_7132580926423717226);
}
void car_H_mod_fun(double *state, double *out_9057155257999617967) {
  H_mod_fun(state, out_9057155257999617967);
}
void car_f_fun(double *state, double dt, double *out_6835096410921863672) {
  f_fun(state,  dt, out_6835096410921863672);
}
void car_F_fun(double *state, double dt, double *out_6493140464273918723) {
  F_fun(state,  dt, out_6493140464273918723);
}
void car_h_25(double *state, double *unused, double *out_3161467451908753406) {
  h_25(state, unused, out_3161467451908753406);
}
void car_H_25(double *state, double *unused, double *out_3598641471216530860) {
  H_25(state, unused, out_3598641471216530860);
}
void car_h_24(double *state, double *unused, double *out_951557180481355989) {
  h_24(state, unused, out_951557180481355989);
}
void car_H_24(double *state, double *unused, double *out_5771291070222030426) {
  H_24(state, unused, out_5771291070222030426);
}
void car_h_30(double *state, double *unused, double *out_6421165196907777024) {
  h_30(state, unused, out_6421165196907777024);
}
void car_H_30(double *state, double *unused, double *out_1080308512709282233) {
  H_30(state, unused, out_1080308512709282233);
}
void car_h_26(double *state, double *unused, double *out_9025570517716474675) {
  h_26(state, unused, out_9025570517716474675);
}
void car_H_26(double *state, double *unused, double *out_7340144790090587084) {
  H_26(state, unused, out_7340144790090587084);
}
void car_h_27(double *state, double *unused, double *out_6198253503353086091) {
  h_27(state, unused, out_6198253503353086091);
}
void car_H_27(double *state, double *unused, double *out_3255071824509707144) {
  H_27(state, unused, out_3255071824509707144);
}
void car_h_29(double *state, double *unused, double *out_5477655344006114589) {
  h_29(state, unused, out_5477655344006114589);
}
void car_H_29(double *state, double *unused, double *out_570077168394890049) {
  H_29(state, unused, out_570077168394890049);
}
void car_h_28(double *state, double *unused, double *out_5594937618676608574) {
  h_28(state, unused, out_5594937618676608574);
}
void car_H_28(double *state, double *unused, double *out_5652476185464420623) {
  H_28(state, unused, out_5652476185464420623);
}
void car_h_31(double *state, double *unused, double *out_4299091312340134919) {
  h_31(state, unused, out_4299091312340134919);
}
void car_H_31(double *state, double *unused, double *out_7966352892323938560) {
  H_31(state, unused, out_7966352892323938560);
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
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
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
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
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
