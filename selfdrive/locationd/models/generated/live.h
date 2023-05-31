#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_2447015731103578408);
void live_err_fun(double *nom_x, double *delta_x, double *out_3395837053962329545);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5210320777607407473);
void live_H_mod_fun(double *state, double *out_6820575991897210909);
void live_f_fun(double *state, double dt, double *out_8387863705841947532);
void live_F_fun(double *state, double dt, double *out_989072999699632411);
void live_h_4(double *state, double *unused, double *out_2757936583198238171);
void live_H_4(double *state, double *unused, double *out_3903282877547840763);
void live_h_9(double *state, double *unused, double *out_1776568129557794530);
void live_H_9(double *state, double *unused, double *out_3662093230918250118);
void live_h_10(double *state, double *unused, double *out_7443942934670921358);
void live_H_10(double *state, double *unused, double *out_2227811144296418959);
void live_h_12(double *state, double *unused, double *out_6099928199274381160);
void live_H_12(double *state, double *unused, double *out_1116173530484121032);
void live_h_31(double *state, double *unused, double *out_3488355203547826243);
void live_H_31(double *state, double *unused, double *out_536620820175233387);
void live_h_32(double *state, double *unused, double *out_3175782848200907409);
void live_H_32(double *state, double *unused, double *out_3549114867921689713);
void live_h_13(double *state, double *unused, double *out_6436318795258633509);
void live_H_13(double *state, double *unused, double *out_4256849022456821605);
void live_h_14(double *state, double *unused, double *out_1776568129557794530);
void live_H_14(double *state, double *unused, double *out_3662093230918250118);
void live_h_33(double *state, double *unused, double *out_9135432984271747309);
void live_H_33(double *state, double *unused, double *out_2613936184463624217);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}