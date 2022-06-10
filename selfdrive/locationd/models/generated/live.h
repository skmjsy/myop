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
void live_H(double *in_vec, double *out_8234830301498350503);
void live_err_fun(double *nom_x, double *delta_x, double *out_9205728283257919471);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1121107328928305263);
void live_H_mod_fun(double *state, double *out_6630457861198022915);
void live_f_fun(double *state, double dt, double *out_4594302555446211754);
void live_F_fun(double *state, double dt, double *out_3899290995433609093);
void live_h_4(double *state, double *unused, double *out_3495821887468815505);
void live_H_4(double *state, double *unused, double *out_8522651919324043022);
void live_h_9(double *state, double *unused, double *out_6629576861282118105);
void live_H_9(double *state, double *unused, double *out_8763841565953633667);
void live_h_10(double *state, double *unused, double *out_1782463083523657948);
void live_H_10(double *state, double *unused, double *out_7840508142764785759);
void live_h_12(double *state, double *unused, double *out_4602718504022440759);
void live_H_12(double *state, double *unused, double *out_4904635746353546799);
void live_h_31(double *state, double *unused, double *out_1341934715987041353);
void live_H_31(double *state, double *unused, double *out_2159072714028533090);
void live_h_32(double *state, double *unused, double *out_2604619187550527596);
void live_H_32(double *state, double *unused, double *out_624585450389884503);
void live_h_13(double *state, double *unused, double *out_6859828326726688004);
void live_H_13(double *state, double *unused, double *out_5851007935052113609);
void live_h_14(double *state, double *unused, double *out_6629576861282118105);
void live_H_14(double *state, double *unused, double *out_8763841565953633667);
void live_h_33(double *state, double *unused, double *out_125438815012891861);
void live_H_33(double *state, double *unused, double *out_3406873092374043614);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}