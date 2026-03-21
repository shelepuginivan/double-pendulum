#include "methods.h"

void dp_rk_base_(DpState *state, DpSystem *system, int size, const double tableau[],
                 DpStateDerivative *k[size]) {
    double h = system->dt;
    int t_ptr = 0;

    // k_1
    k[0] = dp_state_derivative(state, system);

    // k_2, ..., k_s
    for (int s = 1; s < size; s++) {
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

double dp_rk_explicit_(DpState *state, DpSystem *system, int size, const double tableau[]) {
    double h = system->dt;
    int t_ptr = size * (size - 1) / 2;

    DpStateDerivative *k[size];
    dp_rk_base_(state, system, size, tableau, k);

    // b_1, ..., b_s
    for (int s = 0; s < size; s++) {
        double b_i = tableau[t_ptr++];
        dp_derivative_scale_mut(k[s], h * b_i);

        state->phi1 += k[s]->omega1;
        state->phi2 += k[s]->omega2;
        state->omega1 += k[s]->alpha1;
        state->omega2 += k[s]->alpha2;

        dp_derivative_destroy(k[s]);
    }

    return 1.0;
}

double dp_rk_embedded_(DpState *state, DpSystem *system, int size, const double tableau[]) {
    double h = system->dt;
    int t_ptr = size * (size - 1) / 2;

    DpStateDerivative *k[size];
    dp_rk_base_(state, system, size, tableau, k);

    DpState *state_hat = dp_state_copy(state);

    // b_1, ..., b_s
    // b*_1, ..., b*_s
    for (int s = 0; s < size; s++) {
        double b_i = tableau[t_ptr];
        double bhat_i = tableau[size + t_ptr++];

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

    dp_state_destroy(state_hat);

    return fmax(fmax(err_phi_1, err_phi_2), fmax(err_omega_1, err_omega_2));
}

double dp_rk_ralston(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/2,
        0.0,   3.0/4,
        2.0/9, 1.0/3, 4.0/9,
    };
    // clang-format on
    return dp_rk_explicit_(state, system, 3, tableau);
}

double dp_rk4(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
        1.0/2,
        0.0,   1.0/2,
        0.0,   0.0,   1.0,
        1.0/6, 1.0/3, 1.0/3, 1.0/6,
    };
    // clang-format on
    return dp_rk_explicit_(state, system, 4, tableau);
}

double dp_rk38(DpState *state, DpSystem *system) {
    // clang-format off
    double tableau[] = {
         1.0/3,
        -1.0/3,  1.0,
         1.0,   -1.0,   1.0,
         1.0/8,  3.0/8, 3.0/8, 1.0/8,
    };
    // clang-format on
    return dp_rk_explicit_(state, system, 4, tableau);
}

double dp_rk_dopri5(DpState *state, DpSystem *system) {
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
    return dp_rk_embedded_(state, system, 7, tableau);
}

double dp_rk_dopri8(DpState *state, DpSystem *system) {
    // clang-format off
    // Source: https://github.com/rtqichen/torchdiffeq/blob/master/torchdiffeq/_impl/dopri8.py
    double tableau[] = {
        // a
        1.0/18,
        1.0/48, 1.0/16,
        1.0/32, 0.0, 3.0/32,
        5.0/16, 0.0, -75.0/64, 75.0/64,
        3.0/80, 0.0, 0.0, 3.0/16, 3.0/20,
        29443841.0/614563906, 0.0, 0.0, 77736538.0/692538347, -28693883.0/1125000000, 23124283.0/1800000000,
        16016141.0/946692911, 0.0, 0.0, 61564180.0/158732637, 22789713.0/633445777, 545815736.0/2771057229, -180193667.0/1043307555,
        39632708.0/573591083, 0.0, 0.0, -433636366.0/683701615, -421739975.0/2616292301, 100302831.0/723423059, 790204164.0/839813087, 800635310.0/3783071287,
        246121993.0/1340847787, 0.0, 0.0, -37695042795.0/15268766246, -309121744.0/1061227803, -12992083.0/490766935, 6005943493.0/2108947869, 393006217.0/1396673457, 123872331.0/1001029789,
        -1028468189.0/846180014, 0.0, 0.0, 8478235783.0/508512852, 1311729495.0/1432422823, -10304129995.0/1701304382, -48777925059.0/3047939560, 15336726248.0/1032824649, -45442868181.0/3398467696, 3065993473.0/597172653,
        185892177.0/718116043, 0.0, 0.0, -3185094517.0/667107341, -477755414.0/1098053517, -703635378.0/230739211, 5731566787.0/1027545527, 5232866602.0/850066563, -4093664535.0/808688257, 3962137247.0/1805957418, 65686358.0/487910083,
        403863854.0/491063109, 0.0, 0.0, -5068492393.0/434740067, -411421997.0/543043805, 652783627.0/914296604, 11173962825.0/925320556, -13158990841.0/6184727034, 3936647629.0/1978049680, -160528059.0/685178525, 248638103.0/1413531060, 0.0,
        14005451.0/335480064, 0.0, 0.0, 0.0, 0.0, -59238493.0/1068277825, 181606767.0/758867731, 561292985.0/797845732, -1041891430.0/1371343529, 760417239.0/1151165299, 118820643.0/751138087, -528747749.0/2220607170, 1.0/4,
        // b
        14005451.0/335480064, 0.0, 0.0, 0.0, 0.0, -59238493.0/1068277825, 181606767.0/758867731, 561292985.0/797845732, -1041891430.0/1371343529, 760417239.0/1151165299, 118820643.0/751138087, -528747749.0/2220607170, 1.0/4, 0.0,
        // b*
        14005451.0/335480064 - 13451932.0/455176623, 0.0, 0.0, 0.0, 0.0, -59238493.0/1068277825 - -808719846.0/976000145, 181606767.0/758867731 - 1757004468.0/5645159321, 561292985.0/797845732 - 656045339.0/265891186, -1041891430.0/1371343529 - -3867574721.0/1518517206, 760417239.0/1151165299 - 465885868.0/322736535, 118820643.0/751138087 - 53011238.0/667516719, -528747749.0/2220607170 - 2.0/45, 1.0/4, 0.0,
    };
    // clang-format on
    return dp_rk_embedded_(state, system, 13, tableau);
}
