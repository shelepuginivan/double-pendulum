#include "methods.h"

static void dp_rk_generic_(DpState *state, DpSystem *system, int order, const double tableau[]) {
    double h = system->dt;
    int t_ptr = 0;

    DpStateDerivative *k[order];

    // k1
    k[0] = dp_state_derivative(state, system);

    // k1, k2, ..., k_s
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

    // b1, ..., b_s
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

void dp_rk4(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/2,
        0.0,   1.0/2,
        0.0,   0.0,   1.0,
        1.0/6, 1.0/3, 1.0/3, 1.0/6,
    };
    // clang-format on
    dp_rk_generic_(state, system, 4, tableau);
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
    dp_rk_generic_(state, system, 4, tableau);
}
