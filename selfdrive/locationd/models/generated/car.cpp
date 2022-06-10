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
void err_fun(double *nom_x, double *delta_x, double *out_87665246167047285) {
   out_87665246167047285[0] = delta_x[0] + nom_x[0];
   out_87665246167047285[1] = delta_x[1] + nom_x[1];
   out_87665246167047285[2] = delta_x[2] + nom_x[2];
   out_87665246167047285[3] = delta_x[3] + nom_x[3];
   out_87665246167047285[4] = delta_x[4] + nom_x[4];
   out_87665246167047285[5] = delta_x[5] + nom_x[5];
   out_87665246167047285[6] = delta_x[6] + nom_x[6];
   out_87665246167047285[7] = delta_x[7] + nom_x[7];
   out_87665246167047285[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1878561383821348938) {
   out_1878561383821348938[0] = -nom_x[0] + true_x[0];
   out_1878561383821348938[1] = -nom_x[1] + true_x[1];
   out_1878561383821348938[2] = -nom_x[2] + true_x[2];
   out_1878561383821348938[3] = -nom_x[3] + true_x[3];
   out_1878561383821348938[4] = -nom_x[4] + true_x[4];
   out_1878561383821348938[5] = -nom_x[5] + true_x[5];
   out_1878561383821348938[6] = -nom_x[6] + true_x[6];
   out_1878561383821348938[7] = -nom_x[7] + true_x[7];
   out_1878561383821348938[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_2798184415867110840) {
   out_2798184415867110840[0] = 1.0;
   out_2798184415867110840[1] = 0;
   out_2798184415867110840[2] = 0;
   out_2798184415867110840[3] = 0;
   out_2798184415867110840[4] = 0;
   out_2798184415867110840[5] = 0;
   out_2798184415867110840[6] = 0;
   out_2798184415867110840[7] = 0;
   out_2798184415867110840[8] = 0;
   out_2798184415867110840[9] = 0;
   out_2798184415867110840[10] = 1.0;
   out_2798184415867110840[11] = 0;
   out_2798184415867110840[12] = 0;
   out_2798184415867110840[13] = 0;
   out_2798184415867110840[14] = 0;
   out_2798184415867110840[15] = 0;
   out_2798184415867110840[16] = 0;
   out_2798184415867110840[17] = 0;
   out_2798184415867110840[18] = 0;
   out_2798184415867110840[19] = 0;
   out_2798184415867110840[20] = 1.0;
   out_2798184415867110840[21] = 0;
   out_2798184415867110840[22] = 0;
   out_2798184415867110840[23] = 0;
   out_2798184415867110840[24] = 0;
   out_2798184415867110840[25] = 0;
   out_2798184415867110840[26] = 0;
   out_2798184415867110840[27] = 0;
   out_2798184415867110840[28] = 0;
   out_2798184415867110840[29] = 0;
   out_2798184415867110840[30] = 1.0;
   out_2798184415867110840[31] = 0;
   out_2798184415867110840[32] = 0;
   out_2798184415867110840[33] = 0;
   out_2798184415867110840[34] = 0;
   out_2798184415867110840[35] = 0;
   out_2798184415867110840[36] = 0;
   out_2798184415867110840[37] = 0;
   out_2798184415867110840[38] = 0;
   out_2798184415867110840[39] = 0;
   out_2798184415867110840[40] = 1.0;
   out_2798184415867110840[41] = 0;
   out_2798184415867110840[42] = 0;
   out_2798184415867110840[43] = 0;
   out_2798184415867110840[44] = 0;
   out_2798184415867110840[45] = 0;
   out_2798184415867110840[46] = 0;
   out_2798184415867110840[47] = 0;
   out_2798184415867110840[48] = 0;
   out_2798184415867110840[49] = 0;
   out_2798184415867110840[50] = 1.0;
   out_2798184415867110840[51] = 0;
   out_2798184415867110840[52] = 0;
   out_2798184415867110840[53] = 0;
   out_2798184415867110840[54] = 0;
   out_2798184415867110840[55] = 0;
   out_2798184415867110840[56] = 0;
   out_2798184415867110840[57] = 0;
   out_2798184415867110840[58] = 0;
   out_2798184415867110840[59] = 0;
   out_2798184415867110840[60] = 1.0;
   out_2798184415867110840[61] = 0;
   out_2798184415867110840[62] = 0;
   out_2798184415867110840[63] = 0;
   out_2798184415867110840[64] = 0;
   out_2798184415867110840[65] = 0;
   out_2798184415867110840[66] = 0;
   out_2798184415867110840[67] = 0;
   out_2798184415867110840[68] = 0;
   out_2798184415867110840[69] = 0;
   out_2798184415867110840[70] = 1.0;
   out_2798184415867110840[71] = 0;
   out_2798184415867110840[72] = 0;
   out_2798184415867110840[73] = 0;
   out_2798184415867110840[74] = 0;
   out_2798184415867110840[75] = 0;
   out_2798184415867110840[76] = 0;
   out_2798184415867110840[77] = 0;
   out_2798184415867110840[78] = 0;
   out_2798184415867110840[79] = 0;
   out_2798184415867110840[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3062670022773927941) {
   out_3062670022773927941[0] = state[0];
   out_3062670022773927941[1] = state[1];
   out_3062670022773927941[2] = state[2];
   out_3062670022773927941[3] = state[3];
   out_3062670022773927941[4] = state[4];
   out_3062670022773927941[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3062670022773927941[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3062670022773927941[7] = state[7];
   out_3062670022773927941[8] = state[8];
}
void F_fun(double *state, double dt, double *out_310408646566726316) {
   out_310408646566726316[0] = 1;
   out_310408646566726316[1] = 0;
   out_310408646566726316[2] = 0;
   out_310408646566726316[3] = 0;
   out_310408646566726316[4] = 0;
   out_310408646566726316[5] = 0;
   out_310408646566726316[6] = 0;
   out_310408646566726316[7] = 0;
   out_310408646566726316[8] = 0;
   out_310408646566726316[9] = 0;
   out_310408646566726316[10] = 1;
   out_310408646566726316[11] = 0;
   out_310408646566726316[12] = 0;
   out_310408646566726316[13] = 0;
   out_310408646566726316[14] = 0;
   out_310408646566726316[15] = 0;
   out_310408646566726316[16] = 0;
   out_310408646566726316[17] = 0;
   out_310408646566726316[18] = 0;
   out_310408646566726316[19] = 0;
   out_310408646566726316[20] = 1;
   out_310408646566726316[21] = 0;
   out_310408646566726316[22] = 0;
   out_310408646566726316[23] = 0;
   out_310408646566726316[24] = 0;
   out_310408646566726316[25] = 0;
   out_310408646566726316[26] = 0;
   out_310408646566726316[27] = 0;
   out_310408646566726316[28] = 0;
   out_310408646566726316[29] = 0;
   out_310408646566726316[30] = 1;
   out_310408646566726316[31] = 0;
   out_310408646566726316[32] = 0;
   out_310408646566726316[33] = 0;
   out_310408646566726316[34] = 0;
   out_310408646566726316[35] = 0;
   out_310408646566726316[36] = 0;
   out_310408646566726316[37] = 0;
   out_310408646566726316[38] = 0;
   out_310408646566726316[39] = 0;
   out_310408646566726316[40] = 1;
   out_310408646566726316[41] = 0;
   out_310408646566726316[42] = 0;
   out_310408646566726316[43] = 0;
   out_310408646566726316[44] = 0;
   out_310408646566726316[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_310408646566726316[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_310408646566726316[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_310408646566726316[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_310408646566726316[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_310408646566726316[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_310408646566726316[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_310408646566726316[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_310408646566726316[53] = -9.8000000000000007*dt;
   out_310408646566726316[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_310408646566726316[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_310408646566726316[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_310408646566726316[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_310408646566726316[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_310408646566726316[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_310408646566726316[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_310408646566726316[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_310408646566726316[62] = 0;
   out_310408646566726316[63] = 0;
   out_310408646566726316[64] = 0;
   out_310408646566726316[65] = 0;
   out_310408646566726316[66] = 0;
   out_310408646566726316[67] = 0;
   out_310408646566726316[68] = 0;
   out_310408646566726316[69] = 0;
   out_310408646566726316[70] = 1;
   out_310408646566726316[71] = 0;
   out_310408646566726316[72] = 0;
   out_310408646566726316[73] = 0;
   out_310408646566726316[74] = 0;
   out_310408646566726316[75] = 0;
   out_310408646566726316[76] = 0;
   out_310408646566726316[77] = 0;
   out_310408646566726316[78] = 0;
   out_310408646566726316[79] = 0;
   out_310408646566726316[80] = 1;
}
void h_25(double *state, double *unused, double *out_3610076886235420538) {
   out_3610076886235420538[0] = state[6];
}
void H_25(double *state, double *unused, double *out_2721818741776418368) {
   out_2721818741776418368[0] = 0;
   out_2721818741776418368[1] = 0;
   out_2721818741776418368[2] = 0;
   out_2721818741776418368[3] = 0;
   out_2721818741776418368[4] = 0;
   out_2721818741776418368[5] = 0;
   out_2721818741776418368[6] = 1;
   out_2721818741776418368[7] = 0;
   out_2721818741776418368[8] = 0;
}
void h_24(double *state, double *unused, double *out_5159704960558584881) {
   out_5159704960558584881[0] = state[4];
   out_5159704960558584881[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2325173437422558873) {
   out_2325173437422558873[0] = 0;
   out_2325173437422558873[1] = 0;
   out_2325173437422558873[2] = 0;
   out_2325173437422558873[3] = 0;
   out_2325173437422558873[4] = 1;
   out_2325173437422558873[5] = 0;
   out_2325173437422558873[6] = 0;
   out_2325173437422558873[7] = 0;
   out_2325173437422558873[8] = 0;
   out_2325173437422558873[9] = 0;
   out_2325173437422558873[10] = 0;
   out_2325173437422558873[11] = 0;
   out_2325173437422558873[12] = 0;
   out_2325173437422558873[13] = 0;
   out_2325173437422558873[14] = 1;
   out_2325173437422558873[15] = 0;
   out_2325173437422558873[16] = 0;
   out_2325173437422558873[17] = 0;
}
void h_30(double *state, double *unused, double *out_5012041344385877506) {
   out_5012041344385877506[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5240151700283666995) {
   out_5240151700283666995[0] = 0;
   out_5240151700283666995[1] = 0;
   out_5240151700283666995[2] = 0;
   out_5240151700283666995[3] = 0;
   out_5240151700283666995[4] = 1;
   out_5240151700283666995[5] = 0;
   out_5240151700283666995[6] = 0;
   out_5240151700283666995[7] = 0;
   out_5240151700283666995[8] = 0;
}
void h_26(double *state, double *unused, double *out_8690464277850090514) {
   out_8690464277850090514[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1019684577097637856) {
   out_1019684577097637856[0] = 0;
   out_1019684577097637856[1] = 0;
   out_1019684577097637856[2] = 0;
   out_1019684577097637856[3] = 0;
   out_1019684577097637856[4] = 0;
   out_1019684577097637856[5] = 0;
   out_1019684577097637856[6] = 0;
   out_1019684577097637856[7] = 1;
   out_1019684577097637856[8] = 0;
}
void h_27(double *state, double *unused, double *out_1391596381425425107) {
   out_1391596381425425107[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7463745771467610212) {
   out_7463745771467610212[0] = 0;
   out_7463745771467610212[1] = 0;
   out_7463745771467610212[2] = 0;
   out_7463745771467610212[3] = 1;
   out_7463745771467610212[4] = 0;
   out_7463745771467610212[5] = 0;
   out_7463745771467610212[6] = 0;
   out_7463745771467610212[7] = 0;
   out_7463745771467610212[8] = 0;
}
void h_29(double *state, double *unused, double *out_5029045909679447457) {
   out_5029045909679447457[0] = state[1];
}
void H_29(double *state, double *unused, double *out_5750383044598059179) {
   out_5750383044598059179[0] = 0;
   out_5750383044598059179[1] = 1;
   out_5750383044598059179[2] = 0;
   out_5750383044598059179[3] = 0;
   out_5750383044598059179[4] = 0;
   out_5750383044598059179[5] = 0;
   out_5750383044598059179[6] = 0;
   out_5750383044598059179[7] = 0;
   out_5750383044598059179[8] = 0;
}
void h_28(double *state, double *unused, double *out_6043547053003275706) {
   out_6043547053003275706[0] = state[0];
}
void H_28(double *state, double *unused, double *out_667984027528528605) {
   out_667984027528528605[0] = 1;
   out_667984027528528605[1] = 0;
   out_667984027528528605[2] = 0;
   out_667984027528528605[3] = 0;
   out_667984027528528605[4] = 0;
   out_667984027528528605[5] = 0;
   out_667984027528528605[6] = 0;
   out_667984027528528605[7] = 0;
   out_667984027528528605[8] = 0;
}
void h_31(double *state, double *unused, double *out_3885270948519926427) {
   out_3885270948519926427[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2752464703653378796) {
   out_2752464703653378796[0] = 0;
   out_2752464703653378796[1] = 0;
   out_2752464703653378796[2] = 0;
   out_2752464703653378796[3] = 0;
   out_2752464703653378796[4] = 0;
   out_2752464703653378796[5] = 0;
   out_2752464703653378796[6] = 0;
   out_2752464703653378796[7] = 0;
   out_2752464703653378796[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_87665246167047285) {
  err_fun(nom_x, delta_x, out_87665246167047285);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1878561383821348938) {
  inv_err_fun(nom_x, true_x, out_1878561383821348938);
}
void car_H_mod_fun(double *state, double *out_2798184415867110840) {
  H_mod_fun(state, out_2798184415867110840);
}
void car_f_fun(double *state, double dt, double *out_3062670022773927941) {
  f_fun(state,  dt, out_3062670022773927941);
}
void car_F_fun(double *state, double dt, double *out_310408646566726316) {
  F_fun(state,  dt, out_310408646566726316);
}
void car_h_25(double *state, double *unused, double *out_3610076886235420538) {
  h_25(state, unused, out_3610076886235420538);
}
void car_H_25(double *state, double *unused, double *out_2721818741776418368) {
  H_25(state, unused, out_2721818741776418368);
}
void car_h_24(double *state, double *unused, double *out_5159704960558584881) {
  h_24(state, unused, out_5159704960558584881);
}
void car_H_24(double *state, double *unused, double *out_2325173437422558873) {
  H_24(state, unused, out_2325173437422558873);
}
void car_h_30(double *state, double *unused, double *out_5012041344385877506) {
  h_30(state, unused, out_5012041344385877506);
}
void car_H_30(double *state, double *unused, double *out_5240151700283666995) {
  H_30(state, unused, out_5240151700283666995);
}
void car_h_26(double *state, double *unused, double *out_8690464277850090514) {
  h_26(state, unused, out_8690464277850090514);
}
void car_H_26(double *state, double *unused, double *out_1019684577097637856) {
  H_26(state, unused, out_1019684577097637856);
}
void car_h_27(double *state, double *unused, double *out_1391596381425425107) {
  h_27(state, unused, out_1391596381425425107);
}
void car_H_27(double *state, double *unused, double *out_7463745771467610212) {
  H_27(state, unused, out_7463745771467610212);
}
void car_h_29(double *state, double *unused, double *out_5029045909679447457) {
  h_29(state, unused, out_5029045909679447457);
}
void car_H_29(double *state, double *unused, double *out_5750383044598059179) {
  H_29(state, unused, out_5750383044598059179);
}
void car_h_28(double *state, double *unused, double *out_6043547053003275706) {
  h_28(state, unused, out_6043547053003275706);
}
void car_H_28(double *state, double *unused, double *out_667984027528528605) {
  H_28(state, unused, out_667984027528528605);
}
void car_h_31(double *state, double *unused, double *out_3885270948519926427) {
  h_31(state, unused, out_3885270948519926427);
}
void car_H_31(double *state, double *unused, double *out_2752464703653378796) {
  H_31(state, unused, out_2752464703653378796);
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
