#include "derivative.h"
#include "linear.h"

DpStateDerivative *dp_derivative_scale(DpStateDerivative *d, double s) {
    DpStateDerivative *r = (DpStateDerivative *)malloc(sizeof(DpStateDerivative));
    if (r == NULL) {
        return NULL;
    }

    r->omega1 = s * d->omega1;
    r->omega2 = s * d->omega2;
    r->alpha1 = s * d->alpha1;
    r->alpha2 = s * d->alpha2;

    return r;
}

void dp_derivative_scale_mut(DpStateDerivative *d, double s) {
    d->omega1 *= s;
    d->omega2 *= s;
    d->alpha1 *= s;
    d->alpha2 *= s;
}

DpStateDerivative *dp_state_derivative(DpState *state, DpSystem *system) {
    DpStateDerivative *d = (DpStateDerivative *)malloc(sizeof(DpStateDerivative));
    if (d == NULL) {
        return NULL;
    }

    double a1 = state->phi1;
    double a2 = state->phi2;
    double v1 = state->omega1;
    double v2 = state->omega2;

    double m1 = system->m1;
    double m2 = system->m2;
    double l1 = system->l1;
    double l2 = system->l2;
    double g = system->g;

    double m[2][2];
    m[0][0] = (m1 + m2) * l1;
    m[0][1] = m2 * l2 * cos(a1 - a2);
    m[1][0] = l1 * cos(a1 - a2);
    m[1][1] = l2;

    double f[2];
    f[0] = -m2 * l2 * v2 * v2 * sin(a1 - a2) - (m1 + m2) * g * sin(a1);
    f[1] = l1 * v1 * v1 * sin(a1 - a2) - g * sin(a2);

    double inv_m[2][2];
    if (dp_invert_2x2_(m, inv_m) != 0) {
        d->omega1 = v1;
        d->omega2 = v2;
        d->alpha1 = 0.0;
        d->alpha2 = 0.0;
        return d;
    }

    double alpha1 = inv_m[0][0] * f[0] + inv_m[0][1] * f[1];
    double alpha2 = inv_m[1][0] * f[0] + inv_m[1][1] * f[1];

    d->omega1 = v1;
    d->omega2 = v2;
    d->alpha1 = alpha1;
    d->alpha2 = alpha2;

    return d;
}

void dp_derivative_destroy(DpStateDerivative *d) {
    free(d);
}

DpState *dp_state_add(DpState *state, DpStateDerivative *d) {
    DpState *r = dp_state_copy(state);
    if (r == NULL) {
        return state;
    }

    r->phi1 += d->omega1;
    r->phi2 += d->omega2;
    r->omega1 += d->alpha1;
    r->omega2 += d->alpha2;

    return r;
}

void dp_state_add_mut(DpState *state, DpStateDerivative *d) {
    state->phi1 += d->omega1;
    state->phi2 += d->omega2;
    state->omega1 += d->alpha1;
    state->omega2 += d->alpha2;
}
