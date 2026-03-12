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

DpStateDerivative *dp_state_derivative(DpSystem *system, DpState *state);

#endif /* DP_DERIVATIVE_H */
