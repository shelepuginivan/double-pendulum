#ifndef DP_SIMULATION_H
#define DP_SIMULATION_H

#include "state.h"
#include "system.h"

typedef struct DpSimulation {
    double duration;
    char *output;
    double (*stepfn)(DpState *state, DpSystem *system);
    int stepfn_order;
} DpSimulation;

DpSimulation *dp_simulation_new();
DpSimulation *dp_simulation_new_from_env();
int dp_simulation_run(DpSimulation *simulation, DpSystem *system);
void dp_simulation_destroy(DpSimulation *simulation);

#endif /* DP_SIMULATION_H */
