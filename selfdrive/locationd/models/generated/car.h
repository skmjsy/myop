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
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_5302370020658839938);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1069713919102498170);
void car_H_mod_fun(double *state, double *out_1207848624305694604);
void car_f_fun(double *state, double dt, double *out_2686290029376618796);
void car_F_fun(double *state, double dt, double *out_3538402960370055645);
void car_h_25(double *state, double *unused, double *out_3001770872954712799);
void car_H_25(double *state, double *unused, double *out_2759483432311638569);
void car_h_24(double *state, double *unused, double *out_2822160027725109363);
void car_H_24(double *state, double *unused, double *out_582269008704488596);
void car_h_30(double *state, double *unused, double *out_9157218068795372928);
void car_H_30(double *state, double *unused, double *out_241150473804389942);
void car_h_26(double *state, double *unused, double *out_7806964202284876886);
void car_H_26(double *state, double *unused, double *out_6500986751185694793);
void car_h_27(double *state, double *unused, double *out_189218769684859416);
void car_H_27(double *state, double *unused, double *out_2415913785604814853);
void car_h_29(double *state, double *unused, double *out_6581616456665491520);
void car_H_29(double *state, double *unused, double *out_269080870510002242);
void car_h_28(double *state, double *unused, double *out_792534654361336933);
void car_H_28(double *state, double *unused, double *out_4813318146559528332);
void car_h_31(double *state, double *unused, double *out_2726576810670206910);
void car_H_31(double *state, double *unused, double *out_7127194853419046269);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}