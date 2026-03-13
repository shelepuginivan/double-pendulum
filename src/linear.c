#include "linear.h"

int dp_invert_2x2_(const double m[2][2], double inv[2][2]) {
    double det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    if (fabs(det) < 1e-12) {
        return -1;
    }

    double inv_det = 1.0 / det;
    inv[0][0] = m[1][1] * inv_det;
    inv[0][1] = -m[0][1] * inv_det;
    inv[1][0] = -m[1][0] * inv_det;
    inv[1][1] = m[0][0] * inv_det;

    return 0;
}
