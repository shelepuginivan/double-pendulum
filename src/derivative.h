#ifndef DP_DERIVATIVE_H
#define DP_DERIVATIVE_H

#include "state.h"
#include "system.h"

typedef struct DpStateDerivative {
    double omega1;
    double omega2;
    double alpha1;
    double alpha2;
} DpStateDerivative;

DpStateDerivative *dp_state_derivative(DpState *state, DpSystem *system);
DpState *dp_state_add(DpState *state, DpStateDerivative *d);
DpStateDerivative *dp_derivative_scale(DpStateDerivative *d, double s);
void dp_derivative_scale_mut(DpStateDerivative *d, double s);
void dp_derivative_destroy(DpStateDerivative *d);

#endif /* DP_DERIVATIVE_H */
