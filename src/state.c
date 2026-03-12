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

DpState *dp_state_copy(DpState *src) {
    DpState *dst = (DpState *)malloc(sizeof(DpState));
    if (dst == NULL) {
        return NULL;
    }

    dst->phi1 = src->phi1;
    dst->phi2 = src->phi2;
    dst->omega1 = src->omega1;
    dst->omega2 = src->omega2;

    return dst;
}

void dp_state_destroy(DpState *state) {
    free(state);
}
