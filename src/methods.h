#ifndef DP_METHODS_H
#define DP_METHODS_H

#include "derivative.h"
#include "state.h"
#include "system.h"

void dp_rk4(DpSystem *system, DpState *state);

#endif /* DP_METHODS_H */
