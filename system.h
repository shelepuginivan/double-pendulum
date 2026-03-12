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

    double dissipation_factor;

    double dt;
} DpSystem;

DpSystem *dp_system_new();
void dp_system_destroy(DpSystem *dp);

#endif /* DP_SYSTEM_H */
