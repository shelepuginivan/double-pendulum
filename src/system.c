#include "system.h"

DpSystem *dp_system_new() {
    DpSystem *system = (DpSystem *)malloc(sizeof(DpSystem));
    if (system == NULL) {
        return NULL;
    }

    system->m1 = 3;
    system->m2 = 3;
    system->l1 = 2;
    system->l2 = 1;
    system->phi1 = M_PI;
    system->phi2 = M_PI_2;
    system->g = 9.81;
    system->dt = 0.0001;

    return system;
}

static void __getenv_into_double(const char *name, double *dst) {
    char *envvar = getenv(name);
    if (envvar == NULL) {
        return;
    }

    char *endptr;
    double value = strtod(envvar, &endptr);
    if (endptr == envvar) {
        return;
    }

    *dst = value;
}

DpSystem *dp_system_new_from_env() {
    DpSystem *system = dp_system_new();

    __getenv_into_double("DP_SYSTEM_M1", &system->m1);
    __getenv_into_double("DP_SYSTEM_M2", &system->m2);
    __getenv_into_double("DP_SYSTEM_L1", &system->l1);
    __getenv_into_double("DP_SYSTEM_L2", &system->l2);
    __getenv_into_double("DP_SYSTEM_PHI1", &system->phi1);
    __getenv_into_double("DP_SYSTEM_PHI2", &system->phi2);
    __getenv_into_double("DP_SYSTEM_G", &system->g);
    __getenv_into_double("DP_SYSTEM_DT", &system->dt);

    return system;
}

void dp_system_destroy(DpSystem *system) {
    free(system);
}
