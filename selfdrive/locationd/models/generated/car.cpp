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
void err_fun(double *nom_x, double *delta_x, double *out_5260203387612762034) {
   out_5260203387612762034[0] = delta_x[0] + nom_x[0];
   out_5260203387612762034[1] = delta_x[1] + nom_x[1];
   out_5260203387612762034[2] = delta_x[2] + nom_x[2];
   out_5260203387612762034[3] = delta_x[3] + nom_x[3];
   out_5260203387612762034[4] = delta_x[4] + nom_x[4];
   out_5260203387612762034[5] = delta_x[5] + nom_x[5];
   out_5260203387612762034[6] = delta_x[6] + nom_x[6];
   out_5260203387612762034[7] = delta_x[7] + nom_x[7];
   out_5260203387612762034[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2417447482292893008) {
   out_2417447482292893008[0] = -nom_x[0] + true_x[0];
   out_2417447482292893008[1] = -nom_x[1] + true_x[1];
   out_2417447482292893008[2] = -nom_x[2] + true_x[2];
   out_2417447482292893008[3] = -nom_x[3] + true_x[3];
   out_2417447482292893008[4] = -nom_x[4] + true_x[4];
   out_2417447482292893008[5] = -nom_x[5] + true_x[5];
   out_2417447482292893008[6] = -nom_x[6] + true_x[6];
   out_2417447482292893008[7] = -nom_x[7] + true_x[7];
   out_2417447482292893008[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_736133751807541812) {
   out_736133751807541812[0] = 1.0;
   out_736133751807541812[1] = 0;
   out_736133751807541812[2] = 0;
   out_736133751807541812[3] = 0;
   out_736133751807541812[4] = 0;
   out_736133751807541812[5] = 0;
   out_736133751807541812[6] = 0;
   out_736133751807541812[7] = 0;
   out_736133751807541812[8] = 0;
   out_736133751807541812[9] = 0;
   out_736133751807541812[10] = 1.0;
   out_736133751807541812[11] = 0;
   out_736133751807541812[12] = 0;
   out_736133751807541812[13] = 0;
   out_736133751807541812[14] = 0;
   out_736133751807541812[15] = 0;
   out_736133751807541812[16] = 0;
   out_736133751807541812[17] = 0;
   out_736133751807541812[18] = 0;
   out_736133751807541812[19] = 0;
   out_736133751807541812[20] = 1.0;
   out_736133751807541812[21] = 0;
   out_736133751807541812[22] = 0;
   out_736133751807541812[23] = 0;
   out_736133751807541812[24] = 0;
   out_736133751807541812[25] = 0;
   out_736133751807541812[26] = 0;
   out_736133751807541812[27] = 0;
   out_736133751807541812[28] = 0;
   out_736133751807541812[29] = 0;
   out_736133751807541812[30] = 1.0;
   out_736133751807541812[31] = 0;
   out_736133751807541812[32] = 0;
   out_736133751807541812[33] = 0;
   out_736133751807541812[34] = 0;
   out_736133751807541812[35] = 0;
   out_736133751807541812[36] = 0;
   out_736133751807541812[37] = 0;
   out_736133751807541812[38] = 0;
   out_736133751807541812[39] = 0;
   out_736133751807541812[40] = 1.0;
   out_736133751807541812[41] = 0;
   out_736133751807541812[42] = 0;
   out_736133751807541812[43] = 0;
   out_736133751807541812[44] = 0;
   out_736133751807541812[45] = 0;
   out_736133751807541812[46] = 0;
   out_736133751807541812[47] = 0;
   out_736133751807541812[48] = 0;
   out_736133751807541812[49] = 0;
   out_736133751807541812[50] = 1.0;
   out_736133751807541812[51] = 0;
   out_736133751807541812[52] = 0;
   out_736133751807541812[53] = 0;
   out_736133751807541812[54] = 0;
   out_736133751807541812[55] = 0;
   out_736133751807541812[56] = 0;
   out_736133751807541812[57] = 0;
   out_736133751807541812[58] = 0;
   out_736133751807541812[59] = 0;
   out_736133751807541812[60] = 1.0;
   out_736133751807541812[61] = 0;
   out_736133751807541812[62] = 0;
   out_736133751807541812[63] = 0;
   out_736133751807541812[64] = 0;
   out_736133751807541812[65] = 0;
   out_736133751807541812[66] = 0;
   out_736133751807541812[67] = 0;
   out_736133751807541812[68] = 0;
   out_736133751807541812[69] = 0;
   out_736133751807541812[70] = 1.0;
   out_736133751807541812[71] = 0;
   out_736133751807541812[72] = 0;
   out_736133751807541812[73] = 0;
   out_736133751807541812[74] = 0;
   out_736133751807541812[75] = 0;
   out_736133751807541812[76] = 0;
   out_736133751807541812[77] = 0;
   out_736133751807541812[78] = 0;
   out_736133751807541812[79] = 0;
   out_736133751807541812[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_6638941334623620309) {
   out_6638941334623620309[0] = state[0];
   out_6638941334623620309[1] = state[1];
   out_6638941334623620309[2] = state[2];
   out_6638941334623620309[3] = state[3];
   out_6638941334623620309[4] = state[4];
   out_6638941334623620309[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_6638941334623620309[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_6638941334623620309[7] = state[7];
   out_6638941334623620309[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7113132913412742223) {
   out_7113132913412742223[0] = 1;
   out_7113132913412742223[1] = 0;
   out_7113132913412742223[2] = 0;
   out_7113132913412742223[3] = 0;
   out_7113132913412742223[4] = 0;
   out_7113132913412742223[5] = 0;
   out_7113132913412742223[6] = 0;
   out_7113132913412742223[7] = 0;
   out_7113132913412742223[8] = 0;
   out_7113132913412742223[9] = 0;
   out_7113132913412742223[10] = 1;
   out_7113132913412742223[11] = 0;
   out_7113132913412742223[12] = 0;
   out_7113132913412742223[13] = 0;
   out_7113132913412742223[14] = 0;
   out_7113132913412742223[15] = 0;
   out_7113132913412742223[16] = 0;
   out_7113132913412742223[17] = 0;
   out_7113132913412742223[18] = 0;
   out_7113132913412742223[19] = 0;
   out_7113132913412742223[20] = 1;
   out_7113132913412742223[21] = 0;
   out_7113132913412742223[22] = 0;
   out_7113132913412742223[23] = 0;
   out_7113132913412742223[24] = 0;
   out_7113132913412742223[25] = 0;
   out_7113132913412742223[26] = 0;
   out_7113132913412742223[27] = 0;
   out_7113132913412742223[28] = 0;
   out_7113132913412742223[29] = 0;
   out_7113132913412742223[30] = 1;
   out_7113132913412742223[31] = 0;
   out_7113132913412742223[32] = 0;
   out_7113132913412742223[33] = 0;
   out_7113132913412742223[34] = 0;
   out_7113132913412742223[35] = 0;
   out_7113132913412742223[36] = 0;
   out_7113132913412742223[37] = 0;
   out_7113132913412742223[38] = 0;
   out_7113132913412742223[39] = 0;
   out_7113132913412742223[40] = 1;
   out_7113132913412742223[41] = 0;
   out_7113132913412742223[42] = 0;
   out_7113132913412742223[43] = 0;
   out_7113132913412742223[44] = 0;
   out_7113132913412742223[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7113132913412742223[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7113132913412742223[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7113132913412742223[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7113132913412742223[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7113132913412742223[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7113132913412742223[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7113132913412742223[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7113132913412742223[53] = -9.8000000000000007*dt;
   out_7113132913412742223[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7113132913412742223[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7113132913412742223[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7113132913412742223[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7113132913412742223[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7113132913412742223[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7113132913412742223[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7113132913412742223[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7113132913412742223[62] = 0;
   out_7113132913412742223[63] = 0;
   out_7113132913412742223[64] = 0;
   out_7113132913412742223[65] = 0;
   out_7113132913412742223[66] = 0;
   out_7113132913412742223[67] = 0;
   out_7113132913412742223[68] = 0;
   out_7113132913412742223[69] = 0;
   out_7113132913412742223[70] = 1;
   out_7113132913412742223[71] = 0;
   out_7113132913412742223[72] = 0;
   out_7113132913412742223[73] = 0;
   out_7113132913412742223[74] = 0;
   out_7113132913412742223[75] = 0;
   out_7113132913412742223[76] = 0;
   out_7113132913412742223[77] = 0;
   out_7113132913412742223[78] = 0;
   out_7113132913412742223[79] = 0;
   out_7113132913412742223[80] = 1;
}
void h_25(double *state, double *unused, double *out_1318570237641190876) {
   out_1318570237641190876[0] = state[6];
}
void H_25(double *state, double *unused, double *out_3892180156189245388) {
   out_3892180156189245388[0] = 0;
   out_3892180156189245388[1] = 0;
   out_3892180156189245388[2] = 0;
   out_3892180156189245388[3] = 0;
   out_3892180156189245388[4] = 0;
   out_3892180156189245388[5] = 0;
   out_3892180156189245388[6] = 1;
   out_3892180156189245388[7] = 0;
   out_3892180156189245388[8] = 0;
}
void h_24(double *state, double *unused, double *out_8171668241739952707) {
   out_8171668241739952707[0] = state[4];
   out_8171668241739952707[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6064829755194744954) {
   out_6064829755194744954[0] = 0;
   out_6064829755194744954[1] = 0;
   out_6064829755194744954[2] = 0;
   out_6064829755194744954[3] = 0;
   out_6064829755194744954[4] = 1;
   out_6064829755194744954[5] = 0;
   out_6064829755194744954[6] = 0;
   out_6064829755194744954[7] = 0;
   out_6064829755194744954[8] = 0;
   out_6064829755194744954[9] = 0;
   out_6064829755194744954[10] = 0;
   out_6064829755194744954[11] = 0;
   out_6064829755194744954[12] = 0;
   out_6064829755194744954[13] = 0;
   out_6064829755194744954[14] = 1;
   out_6064829755194744954[15] = 0;
   out_6064829755194744954[16] = 0;
   out_6064829755194744954[17] = 0;
}
void h_30(double *state, double *unused, double *out_132061154987692291) {
   out_132061154987692291[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3024510185302371367) {
   out_3024510185302371367[0] = 0;
   out_3024510185302371367[1] = 0;
   out_3024510185302371367[2] = 0;
   out_3024510185302371367[3] = 0;
   out_3024510185302371367[4] = 1;
   out_3024510185302371367[5] = 0;
   out_3024510185302371367[6] = 0;
   out_3024510185302371367[7] = 0;
   out_3024510185302371367[8] = 0;
}
void h_26(double *state, double *unused, double *out_7078848331266529220) {
   out_7078848331266529220[0] = state[7];
}
void H_26(double *state, double *unused, double *out_7633683475063301612) {
   out_7633683475063301612[0] = 0;
   out_7633683475063301612[1] = 0;
   out_7633683475063301612[2] = 0;
   out_7633683475063301612[3] = 0;
   out_7633683475063301612[4] = 0;
   out_7633683475063301612[5] = 0;
   out_7633683475063301612[6] = 0;
   out_7633683475063301612[7] = 1;
   out_7633683475063301612[8] = 0;
}
void h_27(double *state, double *unused, double *out_4937254484317856378) {
   out_4937254484317856378[0] = state[3];
}
void H_27(double *state, double *unused, double *out_849746873501946456) {
   out_849746873501946456[0] = 0;
   out_849746873501946456[1] = 0;
   out_849746873501946456[2] = 0;
   out_849746873501946456[3] = 1;
   out_849746873501946456[4] = 0;
   out_849746873501946456[5] = 0;
   out_849746873501946456[6] = 0;
   out_849746873501946456[7] = 0;
   out_849746873501946456[8] = 0;
}
void h_29(double *state, double *unused, double *out_4869612112673153354) {
   out_4869612112673153354[0] = state[1];
}
void H_29(double *state, double *unused, double *out_863615853367604577) {
   out_863615853367604577[0] = 0;
   out_863615853367604577[1] = 1;
   out_863615853367604577[2] = 0;
   out_863615853367604577[3] = 0;
   out_863615853367604577[4] = 0;
   out_863615853367604577[5] = 0;
   out_863615853367604577[6] = 0;
   out_863615853367604577[7] = 0;
   out_863615853367604577[8] = 0;
}
void h_28(double *state, double *unused, double *out_7060083049689172372) {
   out_7060083049689172372[0] = state[0];
}
void H_28(double *state, double *unused, double *out_5946014870437135151) {
   out_5946014870437135151[0] = 1;
   out_5946014870437135151[1] = 0;
   out_5946014870437135151[2] = 0;
   out_5946014870437135151[3] = 0;
   out_5946014870437135151[4] = 0;
   out_5946014870437135151[5] = 0;
   out_5946014870437135151[6] = 0;
   out_5946014870437135151[7] = 0;
   out_5946014870437135151[8] = 0;
}
void h_31(double *state, double *unused, double *out_1919988395254554430) {
   out_1919988395254554430[0] = state[8];
}
void H_31(double *state, double *unused, double *out_3861534194312284960) {
   out_3861534194312284960[0] = 0;
   out_3861534194312284960[1] = 0;
   out_3861534194312284960[2] = 0;
   out_3861534194312284960[3] = 0;
   out_3861534194312284960[4] = 0;
   out_3861534194312284960[5] = 0;
   out_3861534194312284960[6] = 0;
   out_3861534194312284960[7] = 0;
   out_3861534194312284960[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_5260203387612762034) {
  err_fun(nom_x, delta_x, out_5260203387612762034);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2417447482292893008) {
  inv_err_fun(nom_x, true_x, out_2417447482292893008);
}
void car_H_mod_fun(double *state, double *out_736133751807541812) {
  H_mod_fun(state, out_736133751807541812);
}
void car_f_fun(double *state, double dt, double *out_6638941334623620309) {
  f_fun(state,  dt, out_6638941334623620309);
}
void car_F_fun(double *state, double dt, double *out_7113132913412742223) {
  F_fun(state,  dt, out_7113132913412742223);
}
void car_h_25(double *state, double *unused, double *out_1318570237641190876) {
  h_25(state, unused, out_1318570237641190876);
}
void car_H_25(double *state, double *unused, double *out_3892180156189245388) {
  H_25(state, unused, out_3892180156189245388);
}
void car_h_24(double *state, double *unused, double *out_8171668241739952707) {
  h_24(state, unused, out_8171668241739952707);
}
void car_H_24(double *state, double *unused, double *out_6064829755194744954) {
  H_24(state, unused, out_6064829755194744954);
}
void car_h_30(double *state, double *unused, double *out_132061154987692291) {
  h_30(state, unused, out_132061154987692291);
}
void car_H_30(double *state, double *unused, double *out_3024510185302371367) {
  H_30(state, unused, out_3024510185302371367);
}
void car_h_26(double *state, double *unused, double *out_7078848331266529220) {
  h_26(state, unused, out_7078848331266529220);
}
void car_H_26(double *state, double *unused, double *out_7633683475063301612) {
  H_26(state, unused, out_7633683475063301612);
}
void car_h_27(double *state, double *unused, double *out_4937254484317856378) {
  h_27(state, unused, out_4937254484317856378);
}
void car_H_27(double *state, double *unused, double *out_849746873501946456) {
  H_27(state, unused, out_849746873501946456);
}
void car_h_29(double *state, double *unused, double *out_4869612112673153354) {
  h_29(state, unused, out_4869612112673153354);
}
void car_H_29(double *state, double *unused, double *out_863615853367604577) {
  H_29(state, unused, out_863615853367604577);
}
void car_h_28(double *state, double *unused, double *out_7060083049689172372) {
  h_28(state, unused, out_7060083049689172372);
}
void car_H_28(double *state, double *unused, double *out_5946014870437135151) {
  H_28(state, unused, out_5946014870437135151);
}
void car_h_31(double *state, double *unused, double *out_1919988395254554430) {
  h_31(state, unused, out_1919988395254554430);
}
void car_H_31(double *state, double *unused, double *out_3861534194312284960) {
  H_31(state, unused, out_3861534194312284960);
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
