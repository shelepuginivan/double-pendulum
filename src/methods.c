#include "methods.h"

void dp_rk4(DpSystem *system, DpState *state) {
    double h = system->dt;

    // k1
    DpStateDerivative *k1 = dp_state_derivative(system, state);

    // k2
    DpStateDerivative *k2_tmp_pr = dp_derivative_scale(k1, h / 2);
    DpState *k2_tmp_st = dp_state_add(state, k2_tmp_pr);
    DpStateDerivative *k2 = dp_state_derivative(system, k2_tmp_st);
    dp_derivative_destroy(k2_tmp_pr);
    dp_state_destroy(k2_tmp_st);

    // k3
    DpStateDerivative *k3_tmp_pr = dp_derivative_scale(k2, h / 2);
    DpState *k3_tmp_st = dp_state_add(state, k3_tmp_pr);
    DpStateDerivative *k3 = dp_state_derivative(system, k3_tmp_st);
    dp_derivative_destroy(k3_tmp_pr);
    dp_state_destroy(k3_tmp_st);

    // k3
    DpStateDerivative *k4_tmp_pr = dp_derivative_scale(k3, h);
    DpState *k4_tmp_st = dp_state_add(state, k4_tmp_pr);
    DpStateDerivative *k4 = dp_state_derivative(system, k4_tmp_st);
    dp_derivative_destroy(k4_tmp_pr);
    dp_state_destroy(k4_tmp_st);

    dp_derivative_scale_mut(k1, h / 6);
    dp_derivative_scale_mut(k2, h / 3);
    dp_derivative_scale_mut(k3, h / 3);
    dp_derivative_scale_mut(k4, h / 6);

    state->phi1 += k1->omega1 + k2->omega1 + k3->omega1 + k4->omega1;
    state->phi2 += k1->omega2 + k2->omega2 + k3->omega2 + k4->omega2;
    state->omega1 += k1->alpha1 + k2->alpha1 + k3->alpha1 + k4->alpha1;
    state->omega2 += k1->alpha2 + k2->alpha2 + k3->alpha2 + k4->alpha2;

    dp_derivative_destroy(k1);
    dp_derivative_destroy(k2);
    dp_derivative_destroy(k3);
    dp_derivative_destroy(k4);
}
