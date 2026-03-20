#ifndef DP_SYSTEM_H
#define DP_SYSTEM_H

#include <math.h>
#include <stdlib.h>

typedef struct DpSystem {
    double m1;
    double m2;
    double l1;
    double l2;
    double phi1;
    double phi2;
    double g;
    double dt;
    double atol_phi1;
    double rtol_phi1;
    double atol_phi2;
    double rtol_phi2;
    double atol_omega1;
    double rtol_omega1;
    double atol_omega2;
    double rtol_omega2;
} DpSystem;

DpSystem *dp_system_new();
DpSystem *dp_system_new_from_env();
void dp_system_destroy(DpSystem *system);

#endif /* DP_SYSTEM_H */
