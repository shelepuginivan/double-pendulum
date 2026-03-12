#include <math.h>
#include <stdlib.h>

#include "system.h"

DpSystem *dp_system_new() {
    DpSystem *dp = (DpSystem *)malloc(sizeof(DpSystem));

    dp->m1 = 3;
    dp->m2 = 3;
    dp->l1 = 2;
    dp->l1 = 1;
    dp->phi1 = M_PI;
    dp->phi2 = M_PI_2;
    dp->g = 9.81;
    dp->dissipation_factor = 1;

    return dp;
}

void dp_system_destroy(DpSystem *dp) {
    free(dp);
}
