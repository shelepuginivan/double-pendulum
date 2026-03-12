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

void dp_state_write(DpState *state, DpSystem *system, FILE *stream) {
    double x1 = system->l1 * sin(state->phi1);
    double y1 = -system->l1 * cos(state->phi1);
    double x2 = x1 + system->l2 * sin(state->phi2);
    double y2 = y1 - system->l2 * cos(state->phi2);

    fprintf(stream, "%.6f,%.6f,%.6f,%.6f\n", x1, y1, x2, y2);
}

void dp_state_destroy(DpState *state) {
    free(state);
}
