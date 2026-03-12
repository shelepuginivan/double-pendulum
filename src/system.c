#include "system.h"

DpSystem *dp_system_new() {
    DpSystem *system = (DpSystem *)malloc(sizeof(DpSystem));

    system->m1 = 3;
    system->m2 = 3;
    system->l1 = 2;
    system->l1 = 1;
    system->phi1 = M_PI;
    system->phi2 = M_PI_2;
    system->g = 9.81;
    system->dissipation_factor = 1;

    return system;
}

void dp_system_destroy(DpSystem *system) {
    free(system);
}
