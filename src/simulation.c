#include "simulation.h"

#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "methods.h"
#include "state.h"
#include "system.h"

DpSimulation *dp_simulation_new() {
    DpSimulation *s = (DpSimulation *)malloc(sizeof(DpSimulation));
    if (s == NULL) {
        return NULL;
    }

    s->steps = 1000000;
    s->stepfn = dp_rk4;
    s->output = "-";

    return s;
}

DpSimulation *dp_simulation_new_from_env() {
    DpSimulation *s = dp_simulation_new();

    dp_env_load_into_ulong("DP_SIMULATION_STEPS", &s->steps);
    dp_env_load_into_str("DP_SIMULATION_OUTPUT", &s->output);

    char *stepfn = getenv("DP_SIMULATION_METHOD");
    if (stepfn == NULL) {
        return s;
    }

    if (strcmp(stepfn, "ralston") == 0) {
        s->stepfn = dp_rk_ralston;
    } else if (strcmp(stepfn, "RK4") == 0) {
        s->stepfn = dp_rk4;
    } else if (strcmp(stepfn, "RK3/8") == 0) {
        s->stepfn = dp_rk38;
    }

    return s;
}

int dp_simulation_run(DpSimulation *simulation, DpSystem *system) {
    int output_is_file = strcmp(simulation->output, "-") != 0;

    FILE *output = stdout;
    if (output_is_file) {
        output = fopen(simulation->output, "w");
    }

    if (output == NULL) {
        return 1;
    }

    DpState *state = dp_state_new_from_system(system);

    for (unsigned long i = 0; i < simulation->steps; i++) {
        dp_state_write(state, system, output);
        simulation->stepfn(state, system);
    }

    if (output_is_file) {
        fclose(output);
    }

    return 0;
}

void dp_simulation_destroy(DpSimulation *simulation) {
    free(simulation);
}
