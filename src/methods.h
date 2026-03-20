#ifndef DP_METHODS_H
#define DP_METHODS_H

#include "derivative.h"
#include "state.h"
#include "system.h"

double dp_rk_ralston(DpState *state, DpSystem *system);
double dp_rk4(DpState *state, DpSystem *system);
double dp_rk38(DpState *state, DpSystem *system);
double dp_rk_dopri5(DpState *state, DpSystem *system);
double dp_rk_dopri8(DpState *state, DpSystem *system);

#endif /* DP_METHODS_H */
