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

    s->duration = 30;
    s->stepfn = dp_rk4;
    s->stepfn_order = 4;
    s->output = "-";

    return s;
}

DpSimulation *dp_simulation_new_from_env() {
    DpSimulation *s = dp_simulation_new();

    dp_env_load_into_double("DP_SIMULATION_DURATION", &s->duration);
    dp_env_load_into_str("DP_SIMULATION_OUTPUT", &s->output);

    char *stepfn = getenv("DP_SIMULATION_METHOD");
    if (stepfn == NULL) {
        return s;
    }

    if (strcmp(stepfn, "ralston") == 0) {
        s->stepfn = dp_rk_ralston;
        s->stepfn_order = 3;
    } else if (strcmp(stepfn, "RK4") == 0) {
        s->stepfn = dp_rk4;
        s->stepfn_order = 4;
    } else if (strcmp(stepfn, "RK3/8") == 0) {
        s->stepfn = dp_rk38;
        s->stepfn_order = 4;
    } else if (strcmp(stepfn, "DOPRI5") == 0) {
        s->stepfn = dp_rk_dopri5;
        s->stepfn_order = 5;
    } else if (strcmp(stepfn, "DOPRI8") == 0) {
        s->stepfn = dp_rk_dopri8;
        s->stepfn_order = 8;
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
    dp_state_write(state, system, output);
    double elapsed = 0.0;
    double exp = 1.0 / simulation->stepfn_order;

    while (elapsed < simulation->duration) {
        DpState *prev = dp_state_copy(state);
        double err = simulation->stepfn(state, system);

        if (err > 1) {
            dp_state_destroy(state);
            state = prev;
        } else {
            dp_state_destroy(prev);
            dp_state_write(state, system, output);
            elapsed += system->dt;
        }

        double factor = pow(1.0 / err, exp);
        factor = fmax(system->err_min_factor, fmin(system->err_max_factor, factor));
        system->dt *= factor;
    }

    if (output_is_file) {
        fclose(output);
    }

    return 0;
}

void dp_simulation_destroy(DpSimulation *simulation) {
    free(simulation);
}
