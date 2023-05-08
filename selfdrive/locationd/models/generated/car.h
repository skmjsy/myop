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
void car_err_fun(double *nom_x, double *delta_x, double *out_5260203387612762034);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2417447482292893008);
void car_H_mod_fun(double *state, double *out_736133751807541812);
void car_f_fun(double *state, double dt, double *out_6638941334623620309);
void car_F_fun(double *state, double dt, double *out_7113132913412742223);
void car_h_25(double *state, double *unused, double *out_1318570237641190876);
void car_H_25(double *state, double *unused, double *out_3892180156189245388);
void car_h_24(double *state, double *unused, double *out_8171668241739952707);
void car_H_24(double *state, double *unused, double *out_6064829755194744954);
void car_h_30(double *state, double *unused, double *out_132061154987692291);
void car_H_30(double *state, double *unused, double *out_3024510185302371367);
void car_h_26(double *state, double *unused, double *out_7078848331266529220);
void car_H_26(double *state, double *unused, double *out_7633683475063301612);
void car_h_27(double *state, double *unused, double *out_4937254484317856378);
void car_H_27(double *state, double *unused, double *out_849746873501946456);
void car_h_29(double *state, double *unused, double *out_4869612112673153354);
void car_H_29(double *state, double *unused, double *out_863615853367604577);
void car_h_28(double *state, double *unused, double *out_7060083049689172372);
void car_H_28(double *state, double *unused, double *out_5946014870437135151);
void car_h_31(double *state, double *unused, double *out_1919988395254554430);
void car_H_31(double *state, double *unused, double *out_3861534194312284960);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}