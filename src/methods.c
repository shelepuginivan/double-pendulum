#include "methods.h"

void dp_rk_base_(DpState *state, DpSystem *system, int order, const double tableau[],
                 DpStateDerivative *k[order]) {
    double h = system->dt;
    int t_ptr = 0;

    // k_1
    k[0] = dp_state_derivative(state, system);

    // k_2, ..., k_s
    for (int s = 1; s < order; s++) {
        DpState *state_s = dp_state_copy(state);

        for (int i = 0; i < s; i++) {
            double a_si = tableau[t_ptr++];
            DpStateDerivative *ki_scaled = dp_derivative_scale(k[i], h * a_si);
            dp_state_add_mut(state_s, ki_scaled);
            dp_derivative_destroy(ki_scaled);
        }

        k[s] = dp_state_derivative(state_s, system);
        dp_state_destroy(state_s);
    }
}

void dp_rk_explicit_(DpState *state, DpSystem *system, int order, const double tableau[]) {
    double h = system->dt;
    int t_ptr = order * (order - 1) / 2;

    DpStateDerivative *k[order];
    dp_rk_base_(state, system, order, tableau, k);

    // b_1, ..., b_s
    for (int s = 0; s < order; s++) {
        double b_i = tableau[t_ptr++];
        dp_derivative_scale_mut(k[s], h * b_i);

        state->phi1 += k[s]->omega1;
        state->phi2 += k[s]->omega2;
        state->omega1 += k[s]->alpha1;
        state->omega2 += k[s]->alpha2;

        dp_derivative_destroy(k[s]);
    }
}

double dp_rk_embedded_(DpState *state, DpSystem *system, int order, const double tableau[]) {
    double h = system->dt;
    int t_ptr = order * (order - 1) / 2;

    DpStateDerivative *k[order];
    dp_rk_base_(state, system, order, tableau, k);

    DpState *state_hat = dp_state_copy(state);

    // b_1, ..., b_s
    // b*_1, ..., b*_s
    for (int s = 0; s < order; s++) {
        double b_i = tableau[t_ptr];
        double bhat_i = tableau[order + t_ptr++];

        state->phi1 += h * b_i * k[s]->omega1;
        state->phi2 += h * b_i * k[s]->omega2;
        state->omega1 += h * b_i * k[s]->alpha1;
        state->omega2 += h * b_i * k[s]->alpha2;

        state_hat->phi1 += h * bhat_i * k[s]->omega1;
        state_hat->phi2 += h * bhat_i * k[s]->omega2;
        state_hat->omega1 += h * bhat_i * k[s]->alpha1;
        state_hat->omega2 += h * bhat_i * k[s]->alpha2;

        dp_derivative_destroy(k[s]);
    }

    double d_phi_1 = fabs(state->phi1 - state_hat->phi1);
    double d_phi_2 = fabs(state->phi2 - state_hat->phi2);
    double d_omega_1 = fabs(state->omega1 - state_hat->omega1);
    double d_omega_2 = fabs(state->omega2 - state_hat->omega2);

    double err_phi_1 = d_phi_1 / (system->atol_phi1 + system->rtol_phi1 * state->phi1);
    double err_phi_2 = d_phi_2 / (system->atol_phi2 + system->rtol_phi2 * state->phi2);
    double err_omega_1 = d_omega_1 / (system->atol_omega1 + system->rtol_omega1 * state->omega1);
    double err_omega_2 = d_omega_2 / (system->atol_omega2 + system->rtol_omega2 * state->omega2);

    return fmax(fmax(err_phi_1, err_phi_2), fmax(err_omega_1, err_omega_2));
}

void dp_rk_ralston(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/2,
        0.0,   3.0/4,
        2.0/9, 1.0/3, 4.0/9,
    };
    // clang-format on
    dp_rk_explicit_(state, system, 3, tableau);
}

void dp_rk4(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/2,
        0.0,   1.0/2,
        0.0,   0.0,   1.0,
        1.0/6, 1.0/3, 1.0/3, 1.0/6,
    };
    // clang-format on
    dp_rk_explicit_(state, system, 4, tableau);
}

void dp_rk38(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
         1.0/3,
        -1.0/3,  1.0,
         1.0,   -1.0,   1.0,
         1.0/8,  3.0/8, 3.0/8, 1.0/8,
    };
    // clang-format on
    dp_rk_explicit_(state, system, 4, tableau);
}

void dp_rk_dopri(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/5,
        3.0/40,        9.0/40,
        44.0/45,      -56.0/15,      32.0/9,
        19372.0/6561, -25360.0/2187, 64448.0/6561, -212.0/729,
        9017.0/3168,  -355.0/33,     46732.0/5247, 49.0/176,  -5103.0/18656,
        35.0/384,      0.0,          500.0/1113,   125.0/192, -2187.0/6784,    11.0/84,
        35.0/384,      0.0,          500.0/1113,   125.0/192, -2187.0/6784,    11.0/84,    0,
        5179.0/57600,  0.0,          7571.0/16695, 393.0/640, -92097.0/339200, 187.0/2100, 1.0/40,
    };
    // clang-format on
    dp_rk_embedded_(state, system, 7, tableau);
}
