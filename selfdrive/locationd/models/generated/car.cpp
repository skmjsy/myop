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
void err_fun(double *nom_x, double *delta_x, double *out_1903718844006785339) {
   out_1903718844006785339[0] = delta_x[0] + nom_x[0];
   out_1903718844006785339[1] = delta_x[1] + nom_x[1];
   out_1903718844006785339[2] = delta_x[2] + nom_x[2];
   out_1903718844006785339[3] = delta_x[3] + nom_x[3];
   out_1903718844006785339[4] = delta_x[4] + nom_x[4];
   out_1903718844006785339[5] = delta_x[5] + nom_x[5];
   out_1903718844006785339[6] = delta_x[6] + nom_x[6];
   out_1903718844006785339[7] = delta_x[7] + nom_x[7];
   out_1903718844006785339[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2412479116348601426) {
   out_2412479116348601426[0] = -nom_x[0] + true_x[0];
   out_2412479116348601426[1] = -nom_x[1] + true_x[1];
   out_2412479116348601426[2] = -nom_x[2] + true_x[2];
   out_2412479116348601426[3] = -nom_x[3] + true_x[3];
   out_2412479116348601426[4] = -nom_x[4] + true_x[4];
   out_2412479116348601426[5] = -nom_x[5] + true_x[5];
   out_2412479116348601426[6] = -nom_x[6] + true_x[6];
   out_2412479116348601426[7] = -nom_x[7] + true_x[7];
   out_2412479116348601426[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8612893427579158752) {
   out_8612893427579158752[0] = 1.0;
   out_8612893427579158752[1] = 0;
   out_8612893427579158752[2] = 0;
   out_8612893427579158752[3] = 0;
   out_8612893427579158752[4] = 0;
   out_8612893427579158752[5] = 0;
   out_8612893427579158752[6] = 0;
   out_8612893427579158752[7] = 0;
   out_8612893427579158752[8] = 0;
   out_8612893427579158752[9] = 0;
   out_8612893427579158752[10] = 1.0;
   out_8612893427579158752[11] = 0;
   out_8612893427579158752[12] = 0;
   out_8612893427579158752[13] = 0;
   out_8612893427579158752[14] = 0;
   out_8612893427579158752[15] = 0;
   out_8612893427579158752[16] = 0;
   out_8612893427579158752[17] = 0;
   out_8612893427579158752[18] = 0;
   out_8612893427579158752[19] = 0;
   out_8612893427579158752[20] = 1.0;
   out_8612893427579158752[21] = 0;
   out_8612893427579158752[22] = 0;
   out_8612893427579158752[23] = 0;
   out_8612893427579158752[24] = 0;
   out_8612893427579158752[25] = 0;
   out_8612893427579158752[26] = 0;
   out_8612893427579158752[27] = 0;
   out_8612893427579158752[28] = 0;
   out_8612893427579158752[29] = 0;
   out_8612893427579158752[30] = 1.0;
   out_8612893427579158752[31] = 0;
   out_8612893427579158752[32] = 0;
   out_8612893427579158752[33] = 0;
   out_8612893427579158752[34] = 0;
   out_8612893427579158752[35] = 0;
   out_8612893427579158752[36] = 0;
   out_8612893427579158752[37] = 0;
   out_8612893427579158752[38] = 0;
   out_8612893427579158752[39] = 0;
   out_8612893427579158752[40] = 1.0;
   out_8612893427579158752[41] = 0;
   out_8612893427579158752[42] = 0;
   out_8612893427579158752[43] = 0;
   out_8612893427579158752[44] = 0;
   out_8612893427579158752[45] = 0;
   out_8612893427579158752[46] = 0;
   out_8612893427579158752[47] = 0;
   out_8612893427579158752[48] = 0;
   out_8612893427579158752[49] = 0;
   out_8612893427579158752[50] = 1.0;
   out_8612893427579158752[51] = 0;
   out_8612893427579158752[52] = 0;
   out_8612893427579158752[53] = 0;
   out_8612893427579158752[54] = 0;
   out_8612893427579158752[55] = 0;
   out_8612893427579158752[56] = 0;
   out_8612893427579158752[57] = 0;
   out_8612893427579158752[58] = 0;
   out_8612893427579158752[59] = 0;
   out_8612893427579158752[60] = 1.0;
   out_8612893427579158752[61] = 0;
   out_8612893427579158752[62] = 0;
   out_8612893427579158752[63] = 0;
   out_8612893427579158752[64] = 0;
   out_8612893427579158752[65] = 0;
   out_8612893427579158752[66] = 0;
   out_8612893427579158752[67] = 0;
   out_8612893427579158752[68] = 0;
   out_8612893427579158752[69] = 0;
   out_8612893427579158752[70] = 1.0;
   out_8612893427579158752[71] = 0;
   out_8612893427579158752[72] = 0;
   out_8612893427579158752[73] = 0;
   out_8612893427579158752[74] = 0;
   out_8612893427579158752[75] = 0;
   out_8612893427579158752[76] = 0;
   out_8612893427579158752[77] = 0;
   out_8612893427579158752[78] = 0;
   out_8612893427579158752[79] = 0;
   out_8612893427579158752[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3493803141337671157) {
   out_3493803141337671157[0] = state[0];
   out_3493803141337671157[1] = state[1];
   out_3493803141337671157[2] = state[2];
   out_3493803141337671157[3] = state[3];
   out_3493803141337671157[4] = state[4];
   out_3493803141337671157[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3493803141337671157[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3493803141337671157[7] = state[7];
   out_3493803141337671157[8] = state[8];
}
void F_fun(double *state, double dt, double *out_2040810737274420966) {
   out_2040810737274420966[0] = 1;
   out_2040810737274420966[1] = 0;
   out_2040810737274420966[2] = 0;
   out_2040810737274420966[3] = 0;
   out_2040810737274420966[4] = 0;
   out_2040810737274420966[5] = 0;
   out_2040810737274420966[6] = 0;
   out_2040810737274420966[7] = 0;
   out_2040810737274420966[8] = 0;
   out_2040810737274420966[9] = 0;
   out_2040810737274420966[10] = 1;
   out_2040810737274420966[11] = 0;
   out_2040810737274420966[12] = 0;
   out_2040810737274420966[13] = 0;
   out_2040810737274420966[14] = 0;
   out_2040810737274420966[15] = 0;
   out_2040810737274420966[16] = 0;
   out_2040810737274420966[17] = 0;
   out_2040810737274420966[18] = 0;
   out_2040810737274420966[19] = 0;
   out_2040810737274420966[20] = 1;
   out_2040810737274420966[21] = 0;
   out_2040810737274420966[22] = 0;
   out_2040810737274420966[23] = 0;
   out_2040810737274420966[24] = 0;
   out_2040810737274420966[25] = 0;
   out_2040810737274420966[26] = 0;
   out_2040810737274420966[27] = 0;
   out_2040810737274420966[28] = 0;
   out_2040810737274420966[29] = 0;
   out_2040810737274420966[30] = 1;
   out_2040810737274420966[31] = 0;
   out_2040810737274420966[32] = 0;
   out_2040810737274420966[33] = 0;
   out_2040810737274420966[34] = 0;
   out_2040810737274420966[35] = 0;
   out_2040810737274420966[36] = 0;
   out_2040810737274420966[37] = 0;
   out_2040810737274420966[38] = 0;
   out_2040810737274420966[39] = 0;
   out_2040810737274420966[40] = 1;
   out_2040810737274420966[41] = 0;
   out_2040810737274420966[42] = 0;
   out_2040810737274420966[43] = 0;
   out_2040810737274420966[44] = 0;
   out_2040810737274420966[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_2040810737274420966[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_2040810737274420966[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2040810737274420966[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_2040810737274420966[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_2040810737274420966[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_2040810737274420966[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_2040810737274420966[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_2040810737274420966[53] = -9.8000000000000007*dt;
   out_2040810737274420966[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_2040810737274420966[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_2040810737274420966[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2040810737274420966[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2040810737274420966[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_2040810737274420966[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_2040810737274420966[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_2040810737274420966[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_2040810737274420966[62] = 0;
   out_2040810737274420966[63] = 0;
   out_2040810737274420966[64] = 0;
   out_2040810737274420966[65] = 0;
   out_2040810737274420966[66] = 0;
   out_2040810737274420966[67] = 0;
   out_2040810737274420966[68] = 0;
   out_2040810737274420966[69] = 0;
   out_2040810737274420966[70] = 1;
   out_2040810737274420966[71] = 0;
   out_2040810737274420966[72] = 0;
   out_2040810737274420966[73] = 0;
   out_2040810737274420966[74] = 0;
   out_2040810737274420966[75] = 0;
   out_2040810737274420966[76] = 0;
   out_2040810737274420966[77] = 0;
   out_2040810737274420966[78] = 0;
   out_2040810737274420966[79] = 0;
   out_2040810737274420966[80] = 1;
}
void h_25(double *state, double *unused, double *out_6296145852098902585) {
   out_6296145852098902585[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1985174301473429595) {
   out_1985174301473429595[0] = 0;
   out_1985174301473429595[1] = 0;
   out_1985174301473429595[2] = 0;
   out_1985174301473429595[3] = 0;
   out_1985174301473429595[4] = 0;
   out_1985174301473429595[5] = 0;
   out_1985174301473429595[6] = 1;
   out_1985174301473429595[7] = 0;
   out_1985174301473429595[8] = 0;
}
void h_24(double *state, double *unused, double *out_1572274612598268272) {
   out_1572274612598268272[0] = state[4];
   out_1572274612598268272[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2761335025493079439) {
   out_2761335025493079439[0] = 0;
   out_2761335025493079439[1] = 0;
   out_2761335025493079439[2] = 0;
   out_2761335025493079439[3] = 0;
   out_2761335025493079439[4] = 1;
   out_2761335025493079439[5] = 0;
   out_2761335025493079439[6] = 0;
   out_2761335025493079439[7] = 0;
   out_2761335025493079439[8] = 0;
   out_2761335025493079439[9] = 0;
   out_2761335025493079439[10] = 0;
   out_2761335025493079439[11] = 0;
   out_2761335025493079439[12] = 0;
   out_2761335025493079439[13] = 0;
   out_2761335025493079439[14] = 1;
   out_2761335025493079439[15] = 0;
   out_2761335025493079439[16] = 0;
   out_2761335025493079439[17] = 0;
}
void h_30(double *state, double *unused, double *out_4947382264624663061) {
   out_4947382264624663061[0] = state[4];
}
void H_30(double *state, double *unused, double *out_4503507259980678222) {
   out_4503507259980678222[0] = 0;
   out_4503507259980678222[1] = 0;
   out_4503507259980678222[2] = 0;
   out_4503507259980678222[3] = 0;
   out_4503507259980678222[4] = 1;
   out_4503507259980678222[5] = 0;
   out_4503507259980678222[6] = 0;
   out_4503507259980678222[7] = 0;
   out_4503507259980678222[8] = 0;
}
void h_26(double *state, double *unused, double *out_7568050637961288353) {
   out_7568050637961288353[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1756329017400626629) {
   out_1756329017400626629[0] = 0;
   out_1756329017400626629[1] = 0;
   out_1756329017400626629[2] = 0;
   out_1756329017400626629[3] = 0;
   out_1756329017400626629[4] = 0;
   out_1756329017400626629[5] = 0;
   out_1756329017400626629[6] = 0;
   out_1756329017400626629[7] = 1;
   out_1756329017400626629[8] = 0;
}
void h_27(double *state, double *unused, double *out_7095724083273585942) {
   out_7095724083273585942[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2328743948180253311) {
   out_2328743948180253311[0] = 0;
   out_2328743948180253311[1] = 0;
   out_2328743948180253311[2] = 0;
   out_2328743948180253311[3] = 1;
   out_2328743948180253311[4] = 0;
   out_2328743948180253311[5] = 0;
   out_2328743948180253311[6] = 0;
   out_2328743948180253311[7] = 0;
   out_2328743948180253311[8] = 0;
}
void h_29(double *state, double *unused, double *out_2342976943815965410) {
   out_2342976943815965410[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5013738604295070406) {
   out_5013738604295070406[0] = 0;
   out_5013738604295070406[1] = 1;
   out_5013738604295070406[2] = 0;
   out_5013738604295070406[3] = 0;
   out_5013738604295070406[4] = 0;
   out_5013738604295070406[5] = 0;
   out_5013738604295070406[6] = 0;
   out_5013738604295070406[7] = 0;
   out_5013738604295070406[8] = 0;
}
void h_28(double *state, double *unused, double *out_7293690097366784468) {
   out_7293690097366784468[0] = state[0];
}
void H_28(double *state, double *unused, double *out_68660412774460168) {
   out_68660412774460168[0] = 1;
   out_68660412774460168[1] = 0;
   out_68660412774460168[2] = 0;
   out_68660412774460168[3] = 0;
   out_68660412774460168[4] = 0;
   out_68660412774460168[5] = 0;
   out_68660412774460168[6] = 0;
   out_68660412774460168[7] = 0;
   out_68660412774460168[8] = 0;
}
void h_31(double *state, double *unused, double *out_6571339914383408474) {
   out_6571339914383408474[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2382537119633978105) {
   out_2382537119633978105[0] = 0;
   out_2382537119633978105[1] = 0;
   out_2382537119633978105[2] = 0;
   out_2382537119633978105[3] = 0;
   out_2382537119633978105[4] = 0;
   out_2382537119633978105[5] = 0;
   out_2382537119633978105[6] = 0;
   out_2382537119633978105[7] = 0;
   out_2382537119633978105[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_1903718844006785339) {
  err_fun(nom_x, delta_x, out_1903718844006785339);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2412479116348601426) {
  inv_err_fun(nom_x, true_x, out_2412479116348601426);
}
void car_H_mod_fun(double *state, double *out_8612893427579158752) {
  H_mod_fun(state, out_8612893427579158752);
}
void car_f_fun(double *state, double dt, double *out_3493803141337671157) {
  f_fun(state,  dt, out_3493803141337671157);
}
void car_F_fun(double *state, double dt, double *out_2040810737274420966) {
  F_fun(state,  dt, out_2040810737274420966);
}
void car_h_25(double *state, double *unused, double *out_6296145852098902585) {
  h_25(state, unused, out_6296145852098902585);
}
void car_H_25(double *state, double *unused, double *out_1985174301473429595) {
  H_25(state, unused, out_1985174301473429595);
}
void car_h_24(double *state, double *unused, double *out_1572274612598268272) {
  h_24(state, unused, out_1572274612598268272);
}
void car_H_24(double *state, double *unused, double *out_2761335025493079439) {
  H_24(state, unused, out_2761335025493079439);
}
void car_h_30(double *state, double *unused, double *out_4947382264624663061) {
  h_30(state, unused, out_4947382264624663061);
}
void car_H_30(double *state, double *unused, double *out_4503507259980678222) {
  H_30(state, unused, out_4503507259980678222);
}
void car_h_26(double *state, double *unused, double *out_7568050637961288353) {
  h_26(state, unused, out_7568050637961288353);
}
void car_H_26(double *state, double *unused, double *out_1756329017400626629) {
  H_26(state, unused, out_1756329017400626629);
}
void car_h_27(double *state, double *unused, double *out_7095724083273585942) {
  h_27(state, unused, out_7095724083273585942);
}
void car_H_27(double *state, double *unused, double *out_2328743948180253311) {
  H_27(state, unused, out_2328743948180253311);
}
void car_h_29(double *state, double *unused, double *out_2342976943815965410) {
  h_29(state, unused, out_2342976943815965410);
}
void car_H_29(double *state, double *unused, double *out_5013738604295070406) {
  H_29(state, unused, out_5013738604295070406);
}
void car_h_28(double *state, double *unused, double *out_7293690097366784468) {
  h_28(state, unused, out_7293690097366784468);
}
void car_H_28(double *state, double *unused, double *out_68660412774460168) {
  H_28(state, unused, out_68660412774460168);
}
void car_h_31(double *state, double *unused, double *out_6571339914383408474) {
  h_31(state, unused, out_6571339914383408474);
}
void car_H_31(double *state, double *unused, double *out_2382537119633978105) {
  H_31(state, unused, out_2382537119633978105);
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
