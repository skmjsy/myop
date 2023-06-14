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
void live_H(double *in_vec, double *out_7754977390746696237);
void live_err_fun(double *nom_x, double *delta_x, double *out_4634088626259696784);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5965918245666346755);
void live_H_mod_fun(double *state, double *out_1587006458605850644);
void live_f_fun(double *state, double dt, double *out_4914760753964681424);
void live_F_fun(double *state, double dt, double *out_7780560716312872727);
void live_h_4(double *state, double *unused, double *out_3388021826965404556);
void live_H_4(double *state, double *unused, double *out_4125232642405651625);
void live_h_9(double *state, double *unused, double *out_245122373985030160);
void live_H_9(double *state, double *unused, double *out_7014094194685730967);
void live_h_10(double *state, double *unused, double *out_5858231770917734999);
void live_H_10(double *state, double *unused, double *out_7714636433449217425);
void live_h_12(double *state, double *unused, double *out_1034480534582445047);
void live_H_12(double *state, double *unused, double *out_6654383117621449499);
void live_h_31(double *state, double *unused, double *out_5572486702525651784);
void live_H_31(double *state, double *unused, double *out_7491894699778259001);
void live_h_32(double *state, double *unused, double *out_1683432381118667639);
void live_H_32(double *state, double *unused, double *out_1693193560132731635);
void live_h_13(double *state, double *unused, double *out_4338524068000511598);
void live_H_13(double *state, double *unused, double *out_7496923154941271419);
void live_h_14(double *state, double *unused, double *out_245122373985030160);
void live_H_14(double *state, double *unused, double *out_7014094194685730967);
void live_h_33(double *state, double *unused, double *out_9185912846367836867);
void live_H_33(double *state, double *unused, double *out_758263080657578186);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}