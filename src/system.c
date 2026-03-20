#include "system.h"
#include "env.h"

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
    system->dt = 1e-4;
    system->atol_phi1 = 1e-6;
    system->rtol_phi1 = 1e-4;
    system->atol_phi2 = 1e-6;
    system->rtol_phi2 = 1e-4;
    system->atol_omega1 = 1e-6;
    system->rtol_omega1 = 1e-4;
    system->atol_omega2 = 1e-6;
    system->rtol_omega2 = 1e-4;
    system->err_min_factor = 4.0;
    system->err_max_factor = 0.25;

    return system;
}

DpSystem *dp_system_new_from_env() {
    DpSystem *system = dp_system_new();

    dp_env_load_into_double("DP_SYSTEM_M1", &system->m1);
    dp_env_load_into_double("DP_SYSTEM_M2", &system->m2);
    dp_env_load_into_double("DP_SYSTEM_L1", &system->l1);
    dp_env_load_into_double("DP_SYSTEM_L2", &system->l2);
    dp_env_load_into_double("DP_SYSTEM_PHI1", &system->phi1);
    dp_env_load_into_double("DP_SYSTEM_PHI2", &system->phi2);
    dp_env_load_into_double("DP_SYSTEM_G", &system->g);
    dp_env_load_into_double("DP_SYSTEM_DT", &system->dt);
    dp_env_load_into_double("DP_SYSTEM_ATOL_PHI1", &system->atol_phi1);
    dp_env_load_into_double("DP_SYSTEM_RTOL_PHI1", &system->rtol_phi1);
    dp_env_load_into_double("DP_SYSTEM_ATOL_PHI2", &system->atol_phi2);
    dp_env_load_into_double("DP_SYSTEM_RTOL_PHI2", &system->rtol_phi2);
    dp_env_load_into_double("DP_SYSTEM_ATOL_OMEGA1", &system->atol_omega1);
    dp_env_load_into_double("DP_SYSTEM_RTOL_OMEGA1", &system->rtol_omega1);
    dp_env_load_into_double("DP_SYSTEM_ATOL_OMEGA2", &system->atol_omega2);
    dp_env_load_into_double("DP_SYSTEM_RTOL_OMEGA2", &system->rtol_omega2);
    dp_env_load_into_double("DP_SYSTEM_ERR_MIN_FACTOR", &system->err_min_factor);
    dp_env_load_into_double("DP_SYSTEM_ERR_MAX_FACTOR", &system->err_max_factor);

    return system;
}

void dp_system_destroy(DpSystem *system) {
    free(system);
}
