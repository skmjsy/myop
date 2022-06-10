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
void car_err_fun(double *nom_x, double *delta_x, double *out_1903718844006785339);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2412479116348601426);
void car_H_mod_fun(double *state, double *out_8612893427579158752);
void car_f_fun(double *state, double dt, double *out_3493803141337671157);
void car_F_fun(double *state, double dt, double *out_2040810737274420966);
void car_h_25(double *state, double *unused, double *out_6296145852098902585);
void car_H_25(double *state, double *unused, double *out_1985174301473429595);
void car_h_24(double *state, double *unused, double *out_1572274612598268272);
void car_H_24(double *state, double *unused, double *out_2761335025493079439);
void car_h_30(double *state, double *unused, double *out_4947382264624663061);
void car_H_30(double *state, double *unused, double *out_4503507259980678222);
void car_h_26(double *state, double *unused, double *out_7568050637961288353);
void car_H_26(double *state, double *unused, double *out_1756329017400626629);
void car_h_27(double *state, double *unused, double *out_7095724083273585942);
void car_H_27(double *state, double *unused, double *out_2328743948180253311);
void car_h_29(double *state, double *unused, double *out_2342976943815965410);
void car_H_29(double *state, double *unused, double *out_5013738604295070406);
void car_h_28(double *state, double *unused, double *out_7293690097366784468);
void car_H_28(double *state, double *unused, double *out_68660412774460168);
void car_h_31(double *state, double *unused, double *out_6571339914383408474);
void car_H_31(double *state, double *unused, double *out_2382537119633978105);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}