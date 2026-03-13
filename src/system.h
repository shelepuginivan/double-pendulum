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
} DpSystem;

DpSystem *dp_system_new();
DpSystem *dp_system_new_from_env();
void dp_system_destroy(DpSystem *system);

#endif /* DP_SYSTEM_H */
