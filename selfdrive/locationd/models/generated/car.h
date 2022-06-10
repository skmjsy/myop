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
void car_err_fun(double *nom_x, double *delta_x, double *out_2428597387138040012);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2035103532906739771);
void car_H_mod_fun(double *state, double *out_928234429571031611);
void car_f_fun(double *state, double dt, double *out_3029201977852988162);
void car_F_fun(double *state, double dt, double *out_1663015017050046851);
void car_h_25(double *state, double *unused, double *out_737311645957241244);
void car_H_25(double *state, double *unused, double *out_1773621138033824960);
void car_h_24(double *state, double *unused, double *out_878871673552023650);
void car_H_24(double *state, double *unused, double *out_4539615154683746688);
void car_h_30(double *state, double *unused, double *out_776950091763988880);
void car_H_30(double *state, double *unused, double *out_7059224024726543376);
void car_h_26(double *state, double *unused, double *out_4224723175896856765);
void car_H_26(double *state, double *unused, double *out_3115057271729472512);
void car_h_27(double *state, double *unused, double *out_4106715781963480021);
void car_H_27(double *state, double *unused, double *out_1274387251744938761);
void car_h_29(double *state, double *unused, double *out_5202311007062220413);
void car_H_29(double *state, double *unused, double *out_7633782638582079551);
void car_h_28(double *state, double *unused, double *out_7658855218861588919);
void car_H_28(double *state, double *unused, double *out_2478217323552748178);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}