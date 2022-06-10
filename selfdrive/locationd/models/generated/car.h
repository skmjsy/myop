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
void car_err_fun(double *nom_x, double *delta_x, double *out_87665246167047285);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1878561383821348938);
void car_H_mod_fun(double *state, double *out_2798184415867110840);
void car_f_fun(double *state, double dt, double *out_3062670022773927941);
void car_F_fun(double *state, double dt, double *out_310408646566726316);
void car_h_25(double *state, double *unused, double *out_3610076886235420538);
void car_H_25(double *state, double *unused, double *out_2721818741776418368);
void car_h_24(double *state, double *unused, double *out_5159704960558584881);
void car_H_24(double *state, double *unused, double *out_2325173437422558873);
void car_h_30(double *state, double *unused, double *out_5012041344385877506);
void car_H_30(double *state, double *unused, double *out_5240151700283666995);
void car_h_26(double *state, double *unused, double *out_8690464277850090514);
void car_H_26(double *state, double *unused, double *out_1019684577097637856);
void car_h_27(double *state, double *unused, double *out_1391596381425425107);
void car_H_27(double *state, double *unused, double *out_7463745771467610212);
void car_h_29(double *state, double *unused, double *out_5029045909679447457);
void car_H_29(double *state, double *unused, double *out_5750383044598059179);
void car_h_28(double *state, double *unused, double *out_6043547053003275706);
void car_H_28(double *state, double *unused, double *out_667984027528528605);
void car_h_31(double *state, double *unused, double *out_3885270948519926427);
void car_H_31(double *state, double *unused, double *out_2752464703653378796);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}