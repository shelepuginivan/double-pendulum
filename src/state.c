#include "state.h"
#include "linear.h"
#include "system.h"

DpState *dp_state_new(double phi1, double phi2) {
    DpState *state = (DpState *)malloc(sizeof(DpState));
    if (state == NULL) {
        return NULL;
    }

    state->phi1 = phi1;
    state->phi2 = phi2;
    state->omega1 = 0;
    state->omega2 = 0;

    return state;
}

void dp_state_destroy(DpState *state) {
    free(state);
}

