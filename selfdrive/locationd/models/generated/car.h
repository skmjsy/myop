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
void car_err_fun(double *nom_x, double *delta_x, double *out_7719621718569340755);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_7132580926423717226);
void car_H_mod_fun(double *state, double *out_9057155257999617967);
void car_f_fun(double *state, double dt, double *out_6835096410921863672);
void car_F_fun(double *state, double dt, double *out_6493140464273918723);
void car_h_25(double *state, double *unused, double *out_3161467451908753406);
void car_H_25(double *state, double *unused, double *out_3598641471216530860);
void car_h_24(double *state, double *unused, double *out_951557180481355989);
void car_H_24(double *state, double *unused, double *out_5771291070222030426);
void car_h_30(double *state, double *unused, double *out_6421165196907777024);
void car_H_30(double *state, double *unused, double *out_1080308512709282233);
void car_h_26(double *state, double *unused, double *out_9025570517716474675);
void car_H_26(double *state, double *unused, double *out_7340144790090587084);
void car_h_27(double *state, double *unused, double *out_6198253503353086091);
void car_H_27(double *state, double *unused, double *out_3255071824509707144);
void car_h_29(double *state, double *unused, double *out_5477655344006114589);
void car_H_29(double *state, double *unused, double *out_570077168394890049);
void car_h_28(double *state, double *unused, double *out_5594937618676608574);
void car_H_28(double *state, double *unused, double *out_5652476185464420623);
void car_h_31(double *state, double *unused, double *out_4299091312340134919);
void car_H_31(double *state, double *unused, double *out_7966352892323938560);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}