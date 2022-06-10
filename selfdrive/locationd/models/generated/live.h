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
void live_H(double *in_vec, double *out_3425406721635510492);
void live_err_fun(double *nom_x, double *delta_x, double *out_4797896270428806172);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5798298191881520623);
void live_H_mod_fun(double *state, double *out_2609756539061633336);
void live_f_fun(double *state, double dt, double *out_4035549592486798493);
void live_F_fun(double *state, double dt, double *out_5981865355580897191);
void live_h_4(double *state, double *unused, double *out_4570714533928462214);
void live_H_4(double *state, double *unused, double *out_2576394617671821494);
void live_h_9(double *state, double *unused, double *out_9034987958628764348);
void live_H_9(double *state, double *unused, double *out_8583130520773282652);
void live_h_10(double *state, double *unused, double *out_8913626492654863476);
void live_H_10(double *state, double *unused, double *out_8669899009782713574);
void live_h_12(double *state, double *unused, double *out_7955755614367253947);
void live_H_12(double *state, double *unused, double *out_7595851025703783289);
void live_h_31(double *state, double *unused, double *out_2335013314091180118);
void live_H_31(double *state, double *unused, double *out_5943056675044428870);
void live_h_32(double *state, double *unused, double *out_134736651192051036);
void live_H_32(double *state, double *unused, double *out_343646262177346637);
void live_h_13(double *state, double *unused, double *out_2635231968562706672);
void live_H_13(double *state, double *unused, double *out_7251398425607488672);
void live_h_14(double *state, double *unused, double *out_9034987958628764348);
void live_H_14(double *state, double *unused, double *out_8583130520773282652);
void live_h_33(double *state, double *unused, double *out_8921185596023473147);
void live_H_33(double *state, double *unused, double *out_9093613679683286474);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}