#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_2319107410030823728);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_4190108048568627846);
void car_H_mod_fun(double *state, double *out_6072325280884790361);
void car_f_fun(double *state, double dt, double *out_239005052383860602);
void car_F_fun(double *state, double dt, double *out_444923468154204615);
void car_h_25(double *state, double *unused, double *out_7491594436465106611);
void car_H_25(double *state, double *unused, double *out_6685733608476496299);
void car_h_24(double *state, double *unused, double *out_3329563924746450911);
void car_H_24(double *state, double *unused, double *out_7292824331620972747);
void car_h_30(double *state, double *unused, double *out_6040963043836223444);
void car_H_30(double *state, double *unused, double *out_2147111554283872037);
void car_h_26(double *state, double *unused, double *out_5627252274254741695);
void car_H_26(double *state, double *unused, double *out_8027169742172143851);
void car_h_27(double *state, double *unused, double *out_6802689604599118973);
void car_H_27(double *state, double *unused, double *out_859529566447246725);
void car_h_29(double *state, double *unused, double *out_1078583046154930212);
void car_H_29(double *state, double *unused, double *out_1676687214844959916);
void car_h_28(double *state, double *unused, double *out_7185805253226977804);
void car_H_28(double *state, double *unused, double *out_4136087263852084003);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}