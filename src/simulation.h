#ifndef DP_SIMULATION_H
#define DP_SIMULATION_H

#include "state.h"
#include "system.h"

typedef struct DpSimulation {
    int steps;
    char *output;
    void (*stepfn)(DpState *state, DpSystem *system);
} DpSimulation;

DpSimulation *dp_simulation_new();
int dp_simulation_run(DpSimulation *simulation, DpSystem *system);
void dp_simulation_destroy(DpSimulation *simulation);

#endif /* DP_SIMULATION_H */
