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
void err_fun(double *nom_x, double *delta_x, double *out_2319107410030823728) {
   out_2319107410030823728[0] = delta_x[0] + nom_x[0];
   out_2319107410030823728[1] = delta_x[1] + nom_x[1];
   out_2319107410030823728[2] = delta_x[2] + nom_x[2];
   out_2319107410030823728[3] = delta_x[3] + nom_x[3];
   out_2319107410030823728[4] = delta_x[4] + nom_x[4];
   out_2319107410030823728[5] = delta_x[5] + nom_x[5];
   out_2319107410030823728[6] = delta_x[6] + nom_x[6];
   out_2319107410030823728[7] = delta_x[7] + nom_x[7];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_4190108048568627846) {
   out_4190108048568627846[0] = -nom_x[0] + true_x[0];
   out_4190108048568627846[1] = -nom_x[1] + true_x[1];
   out_4190108048568627846[2] = -nom_x[2] + true_x[2];
   out_4190108048568627846[3] = -nom_x[3] + true_x[3];
   out_4190108048568627846[4] = -nom_x[4] + true_x[4];
   out_4190108048568627846[5] = -nom_x[5] + true_x[5];
   out_4190108048568627846[6] = -nom_x[6] + true_x[6];
   out_4190108048568627846[7] = -nom_x[7] + true_x[7];
}
void H_mod_fun(double *state, double *out_6072325280884790361) {
   out_6072325280884790361[0] = 1.0;
   out_6072325280884790361[1] = 0.0;
   out_6072325280884790361[2] = 0.0;
   out_6072325280884790361[3] = 0.0;
   out_6072325280884790361[4] = 0.0;
   out_6072325280884790361[5] = 0.0;
   out_6072325280884790361[6] = 0.0;
   out_6072325280884790361[7] = 0.0;
   out_6072325280884790361[8] = 0.0;
   out_6072325280884790361[9] = 1.0;
   out_6072325280884790361[10] = 0.0;
   out_6072325280884790361[11] = 0.0;
   out_6072325280884790361[12] = 0.0;
   out_6072325280884790361[13] = 0.0;
   out_6072325280884790361[14] = 0.0;
   out_6072325280884790361[15] = 0.0;
   out_6072325280884790361[16] = 0.0;
   out_6072325280884790361[17] = 0.0;
   out_6072325280884790361[18] = 1.0;
   out_6072325280884790361[19] = 0.0;
   out_6072325280884790361[20] = 0.0;
   out_6072325280884790361[21] = 0.0;
   out_6072325280884790361[22] = 0.0;
   out_6072325280884790361[23] = 0.0;
   out_6072325280884790361[24] = 0.0;
   out_6072325280884790361[25] = 0.0;
   out_6072325280884790361[26] = 0.0;
   out_6072325280884790361[27] = 1.0;
   out_6072325280884790361[28] = 0.0;
   out_6072325280884790361[29] = 0.0;
   out_6072325280884790361[30] = 0.0;
   out_6072325280884790361[31] = 0.0;
   out_6072325280884790361[32] = 0.0;
   out_6072325280884790361[33] = 0.0;
   out_6072325280884790361[34] = 0.0;
   out_6072325280884790361[35] = 0.0;
   out_6072325280884790361[36] = 1.0;
   out_6072325280884790361[37] = 0.0;
   out_6072325280884790361[38] = 0.0;
   out_6072325280884790361[39] = 0.0;
   out_6072325280884790361[40] = 0.0;
   out_6072325280884790361[41] = 0.0;
   out_6072325280884790361[42] = 0.0;
   out_6072325280884790361[43] = 0.0;
   out_6072325280884790361[44] = 0.0;
   out_6072325280884790361[45] = 1.0;
   out_6072325280884790361[46] = 0.0;
   out_6072325280884790361[47] = 0.0;
   out_6072325280884790361[48] = 0.0;
   out_6072325280884790361[49] = 0.0;
   out_6072325280884790361[50] = 0.0;
   out_6072325280884790361[51] = 0.0;
   out_6072325280884790361[52] = 0.0;
   out_6072325280884790361[53] = 0.0;
   out_6072325280884790361[54] = 1.0;
   out_6072325280884790361[55] = 0.0;
   out_6072325280884790361[56] = 0.0;
   out_6072325280884790361[57] = 0.0;
   out_6072325280884790361[58] = 0.0;
   out_6072325280884790361[59] = 0.0;
   out_6072325280884790361[60] = 0.0;
   out_6072325280884790361[61] = 0.0;
   out_6072325280884790361[62] = 0.0;
   out_6072325280884790361[63] = 1.0;
}
void f_fun(double *state, double dt, double *out_239005052383860602) {
   out_239005052383860602[0] = state[0];
   out_239005052383860602[1] = state[1];
   out_239005052383860602[2] = state[2];
   out_239005052383860602[3] = state[3];
   out_239005052383860602[4] = state[4];
   out_239005052383860602[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_239005052383860602[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_239005052383860602[7] = state[7];
}
void F_fun(double *state, double dt, double *out_444923468154204615) {
   out_444923468154204615[0] = 1;
   out_444923468154204615[1] = 0;
   out_444923468154204615[2] = 0;
   out_444923468154204615[3] = 0;
   out_444923468154204615[4] = 0;
   out_444923468154204615[5] = 0;
   out_444923468154204615[6] = 0;
   out_444923468154204615[7] = 0;
   out_444923468154204615[8] = 0;
   out_444923468154204615[9] = 1;
   out_444923468154204615[10] = 0;
   out_444923468154204615[11] = 0;
   out_444923468154204615[12] = 0;
   out_444923468154204615[13] = 0;
   out_444923468154204615[14] = 0;
   out_444923468154204615[15] = 0;
   out_444923468154204615[16] = 0;
   out_444923468154204615[17] = 0;
   out_444923468154204615[18] = 1;
   out_444923468154204615[19] = 0;
   out_444923468154204615[20] = 0;
   out_444923468154204615[21] = 0;
   out_444923468154204615[22] = 0;
   out_444923468154204615[23] = 0;
   out_444923468154204615[24] = 0;
   out_444923468154204615[25] = 0;
   out_444923468154204615[26] = 0;
   out_444923468154204615[27] = 1;
   out_444923468154204615[28] = 0;
   out_444923468154204615[29] = 0;
   out_444923468154204615[30] = 0;
   out_444923468154204615[31] = 0;
   out_444923468154204615[32] = 0;
   out_444923468154204615[33] = 0;
   out_444923468154204615[34] = 0;
   out_444923468154204615[35] = 0;
   out_444923468154204615[36] = 1;
   out_444923468154204615[37] = 0;
   out_444923468154204615[38] = 0;
   out_444923468154204615[39] = 0;
   out_444923468154204615[40] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_444923468154204615[41] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_444923468154204615[42] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_444923468154204615[43] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_444923468154204615[44] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_444923468154204615[45] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_444923468154204615[46] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_444923468154204615[47] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_444923468154204615[48] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_444923468154204615[49] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_444923468154204615[50] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_444923468154204615[51] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_444923468154204615[52] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_444923468154204615[53] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_444923468154204615[54] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_444923468154204615[55] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_444923468154204615[56] = 0;
   out_444923468154204615[57] = 0;
   out_444923468154204615[58] = 0;
   out_444923468154204615[59] = 0;
   out_444923468154204615[60] = 0;
   out_444923468154204615[61] = 0;
   out_444923468154204615[62] = 0;
   out_444923468154204615[63] = 1;
}
void h_25(double *state, double *unused, double *out_7491594436465106611) {
   out_7491594436465106611[0] = state[6];
}
void H_25(double *state, double *unused, double *out_6685733608476496299) {
   out_6685733608476496299[0] = 0;
   out_6685733608476496299[1] = 0;
   out_6685733608476496299[2] = 0;
   out_6685733608476496299[3] = 0;
   out_6685733608476496299[4] = 0;
   out_6685733608476496299[5] = 0;
   out_6685733608476496299[6] = 1;
   out_6685733608476496299[7] = 0;
}
void h_24(double *state, double *unused, double *out_3329563924746450911) {
   out_3329563924746450911[0] = state[4];
   out_3329563924746450911[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7292824331620972747) {
   out_7292824331620972747[0] = 0;
   out_7292824331620972747[1] = 0;
   out_7292824331620972747[2] = 0;
   out_7292824331620972747[3] = 0;
   out_7292824331620972747[4] = 1;
   out_7292824331620972747[5] = 0;
   out_7292824331620972747[6] = 0;
   out_7292824331620972747[7] = 0;
   out_7292824331620972747[8] = 0;
   out_7292824331620972747[9] = 0;
   out_7292824331620972747[10] = 0;
   out_7292824331620972747[11] = 0;
   out_7292824331620972747[12] = 0;
   out_7292824331620972747[13] = 1;
   out_7292824331620972747[14] = 0;
   out_7292824331620972747[15] = 0;
}
void h_30(double *state, double *unused, double *out_6040963043836223444) {
   out_6040963043836223444[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2147111554283872037) {
   out_2147111554283872037[0] = 0;
   out_2147111554283872037[1] = 0;
   out_2147111554283872037[2] = 0;
   out_2147111554283872037[3] = 0;
   out_2147111554283872037[4] = 1;
   out_2147111554283872037[5] = 0;
   out_2147111554283872037[6] = 0;
   out_2147111554283872037[7] = 0;
}
void h_26(double *state, double *unused, double *out_5627252274254741695) {
   out_5627252274254741695[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8027169742172143851) {
   out_8027169742172143851[0] = 0;
   out_8027169742172143851[1] = 0;
   out_8027169742172143851[2] = 0;
   out_8027169742172143851[3] = 0;
   out_8027169742172143851[4] = 0;
   out_8027169742172143851[5] = 0;
   out_8027169742172143851[6] = 0;
   out_8027169742172143851[7] = 1;
}
void h_27(double *state, double *unused, double *out_6802689604599118973) {
   out_6802689604599118973[0] = state[3];
}
void H_27(double *state, double *unused, double *out_859529566447246725) {
   out_859529566447246725[0] = 0;
   out_859529566447246725[1] = 0;
   out_859529566447246725[2] = 0;
   out_859529566447246725[3] = 1;
   out_859529566447246725[4] = 0;
   out_859529566447246725[5] = 0;
   out_859529566447246725[6] = 0;
   out_859529566447246725[7] = 0;
}
void h_29(double *state, double *unused, double *out_1078583046154930212) {
   out_1078583046154930212[0] = state[1];
}
void H_29(double *state, double *unused, double *out_1676687214844959916) {
   out_1676687214844959916[0] = 0;
   out_1676687214844959916[1] = 1;
   out_1676687214844959916[2] = 0;
   out_1676687214844959916[3] = 0;
   out_1676687214844959916[4] = 0;
   out_1676687214844959916[5] = 0;
   out_1676687214844959916[6] = 0;
   out_1676687214844959916[7] = 0;
}
void h_28(double *state, double *unused, double *out_7185805253226977804) {
   out_7185805253226977804[0] = state[5];
   out_7185805253226977804[1] = state[6];
}
void H_28(double *state, double *unused, double *out_4136087263852084003) {
   out_4136087263852084003[0] = 0;
   out_4136087263852084003[1] = 0;
   out_4136087263852084003[2] = 0;
   out_4136087263852084003[3] = 0;
   out_4136087263852084003[4] = 0;
   out_4136087263852084003[5] = 1;
   out_4136087263852084003[6] = 0;
   out_4136087263852084003[7] = 0;
   out_4136087263852084003[8] = 0;
   out_4136087263852084003[9] = 0;
   out_4136087263852084003[10] = 0;
   out_4136087263852084003[11] = 0;
   out_4136087263852084003[12] = 0;
   out_4136087263852084003[13] = 0;
   out_4136087263852084003[14] = 1;
   out_4136087263852084003[15] = 0;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_2319107410030823728) {
  err_fun(nom_x, delta_x, out_2319107410030823728);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_4190108048568627846) {
  inv_err_fun(nom_x, true_x, out_4190108048568627846);
}
void car_H_mod_fun(double *state, double *out_6072325280884790361) {
  H_mod_fun(state, out_6072325280884790361);
}
void car_f_fun(double *state, double dt, double *out_239005052383860602) {
  f_fun(state,  dt, out_239005052383860602);
}
void car_F_fun(double *state, double dt, double *out_444923468154204615) {
  F_fun(state,  dt, out_444923468154204615);
}
void car_h_25(double *state, double *unused, double *out_7491594436465106611) {
  h_25(state, unused, out_7491594436465106611);
}
void car_H_25(double *state, double *unused, double *out_6685733608476496299) {
  H_25(state, unused, out_6685733608476496299);
}
void car_h_24(double *state, double *unused, double *out_3329563924746450911) {
  h_24(state, unused, out_3329563924746450911);
}
void car_H_24(double *state, double *unused, double *out_7292824331620972747) {
  H_24(state, unused, out_7292824331620972747);
}
void car_h_30(double *state, double *unused, double *out_6040963043836223444) {
  h_30(state, unused, out_6040963043836223444);
}
void car_H_30(double *state, double *unused, double *out_2147111554283872037) {
  H_30(state, unused, out_2147111554283872037);
}
void car_h_26(double *state, double *unused, double *out_5627252274254741695) {
  h_26(state, unused, out_5627252274254741695);
}
void car_H_26(double *state, double *unused, double *out_8027169742172143851) {
  H_26(state, unused, out_8027169742172143851);
}
void car_h_27(double *state, double *unused, double *out_6802689604599118973) {
  h_27(state, unused, out_6802689604599118973);
}
void car_H_27(double *state, double *unused, double *out_859529566447246725) {
  H_27(state, unused, out_859529566447246725);
}
void car_h_29(double *state, double *unused, double *out_1078583046154930212) {
  h_29(state, unused, out_1078583046154930212);
}
void car_H_29(double *state, double *unused, double *out_1676687214844959916) {
  H_29(state, unused, out_1676687214844959916);
}
void car_h_28(double *state, double *unused, double *out_7185805253226977804) {
  h_28(state, unused, out_7185805253226977804);
}
void car_H_28(double *state, double *unused, double *out_4136087263852084003) {
  H_28(state, unused, out_4136087263852084003);
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
