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
DpState *dp_state_copy(DpState *src);
void dp_state_destroy(DpState *state);

#endif /* DP_STATE_H */
