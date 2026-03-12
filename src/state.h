#ifndef DP_STATE_H
#define DP_STATE_H

#include <stdlib.h>

#include "system.h"

typedef struct DpState {
    double phi1;
    double phi2;
    double omega1;
    double omega2;
} DpState;

DpState *dp_state_new(double phi1, double phi2);
void dp_state_destroy(DpState *state);

typedef struct DpStateDerivative {
    double omega1;
    double omega2;
    double alpha1;
    double alpha2;
} DpStateDerivative;

DpStateDerivative *dp_state_derivative(DpSystem *system, DpState *state);

#endif /* DP_STATE_H */
