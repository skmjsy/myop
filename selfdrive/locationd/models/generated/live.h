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
void live_H(double *in_vec, double *out_7056038609068127415);
void live_err_fun(double *nom_x, double *delta_x, double *out_8728269313774193771);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2291198337074995052);
void live_H_mod_fun(double *state, double *out_7091612482427988921);
void live_f_fun(double *state, double dt, double *out_8119975525939641860);
void live_F_fun(double *state, double dt, double *out_2642813710906309799);
void live_h_4(double *state, double *unused, double *out_8718177427955779130);
void live_H_4(double *state, double *unused, double *out_4013255994484741449);
void live_h_9(double *state, double *unused, double *out_1394785126297930511);
void live_H_9(double *state, double *unused, double *out_7146269143960362697);
void live_h_10(double *state, double *unused, double *out_444825269738442303);
void live_H_10(double *state, double *unused, double *out_2888002740780520945);
void live_h_12(double *state, double *unused, double *out_3246430753777309875);
void live_H_12(double *state, double *unused, double *out_2368002382557991547);
void live_h_31(double *state, double *unused, double *out_1022804539658703843);
void live_H_31(double *state, double *unused, double *out_6668468638867834663);
void live_h_32(double *state, double *unused, double *out_4446243020354391339);
void live_H_32(double *state, double *unused, double *out_7511082836798089294);
void live_h_13(double *state, double *unused, double *out_8890749997163658878);
void live_H_13(double *state, double *unused, double *out_6180312052694428377);
void live_h_14(double *state, double *unused, double *out_1394785126297930511);
void live_H_14(double *state, double *unused, double *out_7146269143960362697);
void live_h_33(double *state, double *unused, double *out_1483982040104609827);
void live_H_33(double *state, double *unused, double *out_3517911634228977059);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}