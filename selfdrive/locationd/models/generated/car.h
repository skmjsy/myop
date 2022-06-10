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
void car_err_fun(double *nom_x, double *delta_x, double *out_4627986986511657706);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_892478167994604206);
void car_H_mod_fun(double *state, double *out_1654652940774072698);
void car_f_fun(double *state, double dt, double *out_4480379080561767739);
void car_F_fun(double *state, double dt, double *out_4412454719887663630);
void car_h_25(double *state, double *unused, double *out_6452903653248933838);
void car_H_25(double *state, double *unused, double *out_1102735162051213301);
void car_h_24(double *state, double *unused, double *out_1335125576865583327);
void car_H_24(double *state, double *unused, double *out_3275384761056712867);
void car_h_30(double *state, double *unused, double *out_8132156944032116126);
void car_H_30(double *state, double *unused, double *out_5630431492178821499);
void car_h_26(double *state, double *unused, double *out_4447623310421999353);
void car_H_26(double *state, double *unused, double *out_4844238480925269525);
void car_h_27(double *state, double *unused, double *out_7070639697197076947);
void car_H_27(double *state, double *unused, double *out_3406837420994878282);
void car_h_29(double *state, double *unused, double *out_3130483676371270481);
void car_H_29(double *state, double *unused, double *out_5120200147864429315);
void car_h_28(double *state, double *unused, double *out_2668101517703060562);
void car_H_28(double *state, double *unused, double *out_2592973053697713544);
void car_h_31(double *state, double *unused, double *out_89309674947117977);
void car_H_31(double *state, double *unused, double *out_1072089200174252873);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}