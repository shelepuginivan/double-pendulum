#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"
#include "system.h"

int main() {
    DpSystem *system = dp_system_new_from_env();
    DpSimulation *simulation = dp_simulation_new_from_env();

    int st = dp_simulation_run(simulation, system);
    if (st != 0) {
        perror("failed to run simulation:");
        return st;
    }

    dp_simulation_destroy(simulation);
    dp_system_destroy(system);

    return 0;
}
