#ifndef DP_METHODS_H
#define DP_METHODS_H

#include "derivative.h"
#include "state.h"
#include "system.h"

void dp_rk4(DpState *state, DpSystem *system);
void dp_rk38(DpState *state, DpSystem *system);

#endif /* DP_METHODS_H */
