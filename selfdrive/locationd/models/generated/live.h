#pragma once
#include "rednose/helpers/common_ekf.h"
extern "C" {
void live_update_3(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_19(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_442701853816722222);
void live_err_fun(double *nom_x, double *delta_x, double *out_6560530609368688013);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_532116959374556579);
void live_H_mod_fun(double *state, double *out_4647419937900161400);
void live_f_fun(double *state, double dt, double *out_1306338200578888374);
void live_F_fun(double *state, double dt, double *out_2565423341551973010);
void live_h_3(double *state, double *unused, double *out_2113012536703545766);
void live_H_3(double *state, double *unused, double *out_3007671518307571447);
void live_h_4(double *state, double *unused, double *out_826440808924252804);
void live_H_4(double *state, double *unused, double *out_6224370108275721720);
void live_h_9(double *state, double *unused, double *out_5372526147146034567);
void live_H_9(double *state, double *unused, double *out_9004924356658891847);
void live_h_10(double *state, double *unused, double *out_7318883143525795902);
void live_H_10(double *state, double *unused, double *out_7709452610368523833);
void live_h_12(double *state, double *unused, double *out_1378618438851656370);
void live_H_12(double *state, double *unused, double *out_749347902757498102);
void live_h_31(double *state, double *unused, double *out_1721980460729444469);
void live_H_31(double *state, double *unused, double *out_6330663091642527507);
void live_h_32(double *state, double *unused, double *out_2063288632599122000);
void live_H_32(double *state, double *unused, double *out_569862868839486882);
void live_h_13(double *state, double *unused, double *out_2036760599401214772);
void live_H_13(double *state, double *unused, double *out_4210158831443838749);
void live_h_14(double *state, double *unused, double *out_5372526147146034567);
void live_H_14(double *state, double *unused, double *out_9004924356658891847);
void live_h_19(double *state, double *unused, double *out_2807332968975466580);
void live_H_19(double *state, double *unused, double *out_5312592003718588580);
void live_h_33(double *state, double *unused, double *out_4246316989233560146);
void live_H_33(double *state, double *unused, double *out_4966582469304064126);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}