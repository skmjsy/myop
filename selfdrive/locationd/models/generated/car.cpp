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
const static double MAHA_THRESH_28 = 5.991464547107981;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with sympy 1.9                        *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_4627986986511657706) {
   out_4627986986511657706[0] = delta_x[0] + nom_x[0];
   out_4627986986511657706[1] = delta_x[1] + nom_x[1];
   out_4627986986511657706[2] = delta_x[2] + nom_x[2];
   out_4627986986511657706[3] = delta_x[3] + nom_x[3];
   out_4627986986511657706[4] = delta_x[4] + nom_x[4];
   out_4627986986511657706[5] = delta_x[5] + nom_x[5];
   out_4627986986511657706[6] = delta_x[6] + nom_x[6];
   out_4627986986511657706[7] = delta_x[7] + nom_x[7];
   out_4627986986511657706[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_892478167994604206) {
   out_892478167994604206[0] = -nom_x[0] + true_x[0];
   out_892478167994604206[1] = -nom_x[1] + true_x[1];
   out_892478167994604206[2] = -nom_x[2] + true_x[2];
   out_892478167994604206[3] = -nom_x[3] + true_x[3];
   out_892478167994604206[4] = -nom_x[4] + true_x[4];
   out_892478167994604206[5] = -nom_x[5] + true_x[5];
   out_892478167994604206[6] = -nom_x[6] + true_x[6];
   out_892478167994604206[7] = -nom_x[7] + true_x[7];
   out_892478167994604206[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_1654652940774072698) {
   out_1654652940774072698[0] = 1.0;
   out_1654652940774072698[1] = 0;
   out_1654652940774072698[2] = 0;
   out_1654652940774072698[3] = 0;
   out_1654652940774072698[4] = 0;
   out_1654652940774072698[5] = 0;
   out_1654652940774072698[6] = 0;
   out_1654652940774072698[7] = 0;
   out_1654652940774072698[8] = 0;
   out_1654652940774072698[9] = 0;
   out_1654652940774072698[10] = 1.0;
   out_1654652940774072698[11] = 0;
   out_1654652940774072698[12] = 0;
   out_1654652940774072698[13] = 0;
   out_1654652940774072698[14] = 0;
   out_1654652940774072698[15] = 0;
   out_1654652940774072698[16] = 0;
   out_1654652940774072698[17] = 0;
   out_1654652940774072698[18] = 0;
   out_1654652940774072698[19] = 0;
   out_1654652940774072698[20] = 1.0;
   out_1654652940774072698[21] = 0;
   out_1654652940774072698[22] = 0;
   out_1654652940774072698[23] = 0;
   out_1654652940774072698[24] = 0;
   out_1654652940774072698[25] = 0;
   out_1654652940774072698[26] = 0;
   out_1654652940774072698[27] = 0;
   out_1654652940774072698[28] = 0;
   out_1654652940774072698[29] = 0;
   out_1654652940774072698[30] = 1.0;
   out_1654652940774072698[31] = 0;
   out_1654652940774072698[32] = 0;
   out_1654652940774072698[33] = 0;
   out_1654652940774072698[34] = 0;
   out_1654652940774072698[35] = 0;
   out_1654652940774072698[36] = 0;
   out_1654652940774072698[37] = 0;
   out_1654652940774072698[38] = 0;
   out_1654652940774072698[39] = 0;
   out_1654652940774072698[40] = 1.0;
   out_1654652940774072698[41] = 0;
   out_1654652940774072698[42] = 0;
   out_1654652940774072698[43] = 0;
   out_1654652940774072698[44] = 0;
   out_1654652940774072698[45] = 0;
   out_1654652940774072698[46] = 0;
   out_1654652940774072698[47] = 0;
   out_1654652940774072698[48] = 0;
   out_1654652940774072698[49] = 0;
   out_1654652940774072698[50] = 1.0;
   out_1654652940774072698[51] = 0;
   out_1654652940774072698[52] = 0;
   out_1654652940774072698[53] = 0;
   out_1654652940774072698[54] = 0;
   out_1654652940774072698[55] = 0;
   out_1654652940774072698[56] = 0;
   out_1654652940774072698[57] = 0;
   out_1654652940774072698[58] = 0;
   out_1654652940774072698[59] = 0;
   out_1654652940774072698[60] = 1.0;
   out_1654652940774072698[61] = 0;
   out_1654652940774072698[62] = 0;
   out_1654652940774072698[63] = 0;
   out_1654652940774072698[64] = 0;
   out_1654652940774072698[65] = 0;
   out_1654652940774072698[66] = 0;
   out_1654652940774072698[67] = 0;
   out_1654652940774072698[68] = 0;
   out_1654652940774072698[69] = 0;
   out_1654652940774072698[70] = 1.0;
   out_1654652940774072698[71] = 0;
   out_1654652940774072698[72] = 0;
   out_1654652940774072698[73] = 0;
   out_1654652940774072698[74] = 0;
   out_1654652940774072698[75] = 0;
   out_1654652940774072698[76] = 0;
   out_1654652940774072698[77] = 0;
   out_1654652940774072698[78] = 0;
   out_1654652940774072698[79] = 0;
   out_1654652940774072698[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_4480379080561767739) {
   out_4480379080561767739[0] = state[0];
   out_4480379080561767739[1] = state[1];
   out_4480379080561767739[2] = state[2];
   out_4480379080561767739[3] = state[3];
   out_4480379080561767739[4] = state[4];
   out_4480379080561767739[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_4480379080561767739[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_4480379080561767739[7] = state[7];
   out_4480379080561767739[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4412454719887663630) {
   out_4412454719887663630[0] = 1;
   out_4412454719887663630[1] = 0;
   out_4412454719887663630[2] = 0;
   out_4412454719887663630[3] = 0;
   out_4412454719887663630[4] = 0;
   out_4412454719887663630[5] = 0;
   out_4412454719887663630[6] = 0;
   out_4412454719887663630[7] = 0;
   out_4412454719887663630[8] = 0;
   out_4412454719887663630[9] = 0;
   out_4412454719887663630[10] = 1;
   out_4412454719887663630[11] = 0;
   out_4412454719887663630[12] = 0;
   out_4412454719887663630[13] = 0;
   out_4412454719887663630[14] = 0;
   out_4412454719887663630[15] = 0;
   out_4412454719887663630[16] = 0;
   out_4412454719887663630[17] = 0;
   out_4412454719887663630[18] = 0;
   out_4412454719887663630[19] = 0;
   out_4412454719887663630[20] = 1;
   out_4412454719887663630[21] = 0;
   out_4412454719887663630[22] = 0;
   out_4412454719887663630[23] = 0;
   out_4412454719887663630[24] = 0;
   out_4412454719887663630[25] = 0;
   out_4412454719887663630[26] = 0;
   out_4412454719887663630[27] = 0;
   out_4412454719887663630[28] = 0;
   out_4412454719887663630[29] = 0;
   out_4412454719887663630[30] = 1;
   out_4412454719887663630[31] = 0;
   out_4412454719887663630[32] = 0;
   out_4412454719887663630[33] = 0;
   out_4412454719887663630[34] = 0;
   out_4412454719887663630[35] = 0;
   out_4412454719887663630[36] = 0;
   out_4412454719887663630[37] = 0;
   out_4412454719887663630[38] = 0;
   out_4412454719887663630[39] = 0;
   out_4412454719887663630[40] = 1;
   out_4412454719887663630[41] = 0;
   out_4412454719887663630[42] = 0;
   out_4412454719887663630[43] = 0;
   out_4412454719887663630[44] = 0;
   out_4412454719887663630[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4412454719887663630[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4412454719887663630[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4412454719887663630[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4412454719887663630[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4412454719887663630[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4412454719887663630[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4412454719887663630[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4412454719887663630[53] = -9.8000000000000007*dt;
   out_4412454719887663630[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4412454719887663630[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4412454719887663630[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4412454719887663630[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4412454719887663630[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4412454719887663630[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4412454719887663630[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4412454719887663630[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4412454719887663630[62] = 0;
   out_4412454719887663630[63] = 0;
   out_4412454719887663630[64] = 0;
   out_4412454719887663630[65] = 0;
   out_4412454719887663630[66] = 0;
   out_4412454719887663630[67] = 0;
   out_4412454719887663630[68] = 0;
   out_4412454719887663630[69] = 0;
   out_4412454719887663630[70] = 1;
   out_4412454719887663630[71] = 0;
   out_4412454719887663630[72] = 0;
   out_4412454719887663630[73] = 0;
   out_4412454719887663630[74] = 0;
   out_4412454719887663630[75] = 0;
   out_4412454719887663630[76] = 0;
   out_4412454719887663630[77] = 0;
   out_4412454719887663630[78] = 0;
   out_4412454719887663630[79] = 0;
   out_4412454719887663630[80] = 1;
}
void h_25(double *state, double *unused, double *out_6452903653248933838) {
   out_6452903653248933838[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1102735162051213301) {
   out_1102735162051213301[0] = 0;
   out_1102735162051213301[1] = 0;
   out_1102735162051213301[2] = 0;
   out_1102735162051213301[3] = 0;
   out_1102735162051213301[4] = 0;
   out_1102735162051213301[5] = 0;
   out_1102735162051213301[6] = 1;
   out_1102735162051213301[7] = 0;
   out_1102735162051213301[8] = 0;
}
void h_24(double *state, double *unused, double *out_1335125576865583327) {
   out_1335125576865583327[0] = state[4];
   out_1335125576865583327[1] = state[5];
}
void H_24(double *state, double *unused, double *out_3275384761056712867) {
   out_3275384761056712867[0] = 0;
   out_3275384761056712867[1] = 0;
   out_3275384761056712867[2] = 0;
   out_3275384761056712867[3] = 0;
   out_3275384761056712867[4] = 1;
   out_3275384761056712867[5] = 0;
   out_3275384761056712867[6] = 0;
   out_3275384761056712867[7] = 0;
   out_3275384761056712867[8] = 0;
   out_3275384761056712867[9] = 0;
   out_3275384761056712867[10] = 0;
   out_3275384761056712867[11] = 0;
   out_3275384761056712867[12] = 0;
   out_3275384761056712867[13] = 0;
   out_3275384761056712867[14] = 1;
   out_3275384761056712867[15] = 0;
   out_3275384761056712867[16] = 0;
   out_3275384761056712867[17] = 0;
}
void h_30(double *state, double *unused, double *out_8132156944032116126) {
   out_8132156944032116126[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5630431492178821499) {
   out_5630431492178821499[0] = 0;
   out_5630431492178821499[1] = 0;
   out_5630431492178821499[2] = 0;
   out_5630431492178821499[3] = 0;
   out_5630431492178821499[4] = 1;
   out_5630431492178821499[5] = 0;
   out_5630431492178821499[6] = 0;
   out_5630431492178821499[7] = 0;
   out_5630431492178821499[8] = 0;
}
void h_26(double *state, double *unused, double *out_4447623310421999353) {
   out_4447623310421999353[0] = state[7];
}
void H_26(double *state, double *unused, double *out_4844238480925269525) {
   out_4844238480925269525[0] = 0;
   out_4844238480925269525[1] = 0;
   out_4844238480925269525[2] = 0;
   out_4844238480925269525[3] = 0;
   out_4844238480925269525[4] = 0;
   out_4844238480925269525[5] = 0;
   out_4844238480925269525[6] = 0;
   out_4844238480925269525[7] = 1;
   out_4844238480925269525[8] = 0;
}
void h_27(double *state, double *unused, double *out_7070639697197076947) {
   out_7070639697197076947[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3406837420994878282) {
   out_3406837420994878282[0] = 0;
   out_3406837420994878282[1] = 0;
   out_3406837420994878282[2] = 0;
   out_3406837420994878282[3] = 1;
   out_3406837420994878282[4] = 0;
   out_3406837420994878282[5] = 0;
   out_3406837420994878282[6] = 0;
   out_3406837420994878282[7] = 0;
   out_3406837420994878282[8] = 0;
}
void h_29(double *state, double *unused, double *out_3130483676371270481) {
   out_3130483676371270481[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5120200147864429315) {
   out_5120200147864429315[0] = 0;
   out_5120200147864429315[1] = 1;
   out_5120200147864429315[2] = 0;
   out_5120200147864429315[3] = 0;
   out_5120200147864429315[4] = 0;
   out_5120200147864429315[5] = 0;
   out_5120200147864429315[6] = 0;
   out_5120200147864429315[7] = 0;
   out_5120200147864429315[8] = 0;
}
void h_28(double *state, double *unused, double *out_2668101517703060562) {
   out_2668101517703060562[0] = state[5];
   out_2668101517703060562[1] = state[6];
}
void H_28(double *state, double *unused, double *out_2592973053697713544) {
   out_2592973053697713544[0] = 0;
   out_2592973053697713544[1] = 0;
   out_2592973053697713544[2] = 0;
   out_2592973053697713544[3] = 0;
   out_2592973053697713544[4] = 0;
   out_2592973053697713544[5] = 1;
   out_2592973053697713544[6] = 0;
   out_2592973053697713544[7] = 0;
   out_2592973053697713544[8] = 0;
   out_2592973053697713544[9] = 0;
   out_2592973053697713544[10] = 0;
   out_2592973053697713544[11] = 0;
   out_2592973053697713544[12] = 0;
   out_2592973053697713544[13] = 0;
   out_2592973053697713544[14] = 0;
   out_2592973053697713544[15] = 1;
   out_2592973053697713544[16] = 0;
   out_2592973053697713544[17] = 0;
}
void h_31(double *state, double *unused, double *out_89309674947117977) {
   out_89309674947117977[0] = state[8];
}
void H_31(double *state, double *unused, double *out_1072089200174252873) {
   out_1072089200174252873[0] = 0;
   out_1072089200174252873[1] = 0;
   out_1072089200174252873[2] = 0;
   out_1072089200174252873[3] = 0;
   out_1072089200174252873[4] = 0;
   out_1072089200174252873[5] = 0;
   out_1072089200174252873[6] = 0;
   out_1072089200174252873[7] = 0;
   out_1072089200174252873[8] = 1;
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
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_4627986986511657706) {
  err_fun(nom_x, delta_x, out_4627986986511657706);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_892478167994604206) {
  inv_err_fun(nom_x, true_x, out_892478167994604206);
}
void car_H_mod_fun(double *state, double *out_1654652940774072698) {
  H_mod_fun(state, out_1654652940774072698);
}
void car_f_fun(double *state, double dt, double *out_4480379080561767739) {
  f_fun(state,  dt, out_4480379080561767739);
}
void car_F_fun(double *state, double dt, double *out_4412454719887663630) {
  F_fun(state,  dt, out_4412454719887663630);
}
void car_h_25(double *state, double *unused, double *out_6452903653248933838) {
  h_25(state, unused, out_6452903653248933838);
}
void car_H_25(double *state, double *unused, double *out_1102735162051213301) {
  H_25(state, unused, out_1102735162051213301);
}
void car_h_24(double *state, double *unused, double *out_1335125576865583327) {
  h_24(state, unused, out_1335125576865583327);
}
void car_H_24(double *state, double *unused, double *out_3275384761056712867) {
  H_24(state, unused, out_3275384761056712867);
}
void car_h_30(double *state, double *unused, double *out_8132156944032116126) {
  h_30(state, unused, out_8132156944032116126);
}
void car_H_30(double *state, double *unused, double *out_5630431492178821499) {
  H_30(state, unused, out_5630431492178821499);
}
void car_h_26(double *state, double *unused, double *out_4447623310421999353) {
  h_26(state, unused, out_4447623310421999353);
}
void car_H_26(double *state, double *unused, double *out_4844238480925269525) {
  H_26(state, unused, out_4844238480925269525);
}
void car_h_27(double *state, double *unused, double *out_7070639697197076947) {
  h_27(state, unused, out_7070639697197076947);
}
void car_H_27(double *state, double *unused, double *out_3406837420994878282) {
  H_27(state, unused, out_3406837420994878282);
}
void car_h_29(double *state, double *unused, double *out_3130483676371270481) {
  h_29(state, unused, out_3130483676371270481);
}
void car_H_29(double *state, double *unused, double *out_5120200147864429315) {
  H_29(state, unused, out_5120200147864429315);
}
void car_h_28(double *state, double *unused, double *out_2668101517703060562) {
  h_28(state, unused, out_2668101517703060562);
}
void car_H_28(double *state, double *unused, double *out_2592973053697713544) {
  H_28(state, unused, out_2592973053697713544);
}
void car_h_31(double *state, double *unused, double *out_89309674947117977) {
  h_31(state, unused, out_89309674947117977);
}
void car_H_31(double *state, double *unused, double *out_1072089200174252873) {
  H_31(state, unused, out_1072089200174252873);
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
