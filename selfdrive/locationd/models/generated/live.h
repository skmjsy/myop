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
void live_H(double *in_vec, double *out_2870835747538521324);
void live_err_fun(double *nom_x, double *delta_x, double *out_7368662874055828039);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_4745738344135790059);
void live_H_mod_fun(double *state, double *out_3913231476138700744);
void live_f_fun(double *state, double dt, double *out_5773650460671003289);
void live_F_fun(double *state, double dt, double *out_4091781263626630367);
void live_h_4(double *state, double *unused, double *out_8563271857498967158);
void live_H_4(double *state, double *unused, double *out_3677695303996204487);
void live_h_9(double *state, double *unused, double *out_3999300133775259038);
void live_H_9(double *state, double *unused, double *out_3436505657366613842);
void live_h_10(double *state, double *unused, double *out_704828927513678872);
void live_H_10(double *state, double *unused, double *out_5362701568243957618);
void live_h_12(double *state, double *unused, double *out_8851125533277454888);
void live_H_12(double *state, double *unused, double *out_1341761104035757308);
void live_h_31(double *state, double *unused, double *out_4114263551627878889);
void live_H_31(double *state, double *unused, double *out_311033246623597111);
void live_h_32(double *state, double *unused, double *out_1593627762583460040);
void live_H_32(double *state, double *unused, double *out_7225897750317713467);
void live_h_13(double *state, double *unused, double *out_3335436485774091998);
void live_H_13(double *state, double *unused, double *out_4404602760385322838);
void live_h_14(double *state, double *unused, double *out_3999300133775259038);
void live_H_14(double *state, double *unused, double *out_3436505657366613842);
void live_h_33(double *state, double *unused, double *out_7404134896959636176);
void live_H_33(double *state, double *unused, double *out_2839523758015260493);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}