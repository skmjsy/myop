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
void live_H(double *in_vec, double *out_769978375014327116);
void live_err_fun(double *nom_x, double *delta_x, double *out_3930581419091845154);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2964816902716136158);
void live_H_mod_fun(double *state, double *out_2094271694511580680);
void live_f_fun(double *state, double dt, double *out_7618125418436970734);
void live_F_fun(double *state, double dt, double *out_2420505287107436684);
void live_h_4(double *state, double *unused, double *out_6516895377116661716);
void live_H_4(double *state, double *unused, double *out_1148440156446637937);
void live_h_9(double *state, double *unused, double *out_7623235786031897685);
void live_H_9(double *state, double *unused, double *out_907250509817047292);
void live_h_10(double *state, double *unused, double *out_8031178198245777888);
void live_H_10(double *state, double *unused, double *out_8420786504377253541);
void live_h_12(double *state, double *unused, double *out_2524803393288813612);
void live_H_12(double *state, double *unused, double *out_527341131399044270);
void live_h_31(double *state, double *unused, double *out_9086913132901260176);
void live_H_31(double *state, double *unused, double *out_2218221900925969439);
void live_h_32(double *state, double *unused, double *out_1686507834135682794);
void live_H_32(double *state, double *unused, double *out_6398834719730307759);
void live_h_13(double *state, double *unused, double *out_1825263801085620470);
void live_H_13(double *state, double *unused, double *out_651079024379313970);
void live_h_14(double *state, double *unused, double *out_7623235786031897685);
void live_H_14(double *state, double *unused, double *out_907250509817047292);
void live_h_33(double *state, double *unused, double *out_1461014533630698934);
void live_H_33(double *state, double *unused, double *out_5368778905564827043);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}