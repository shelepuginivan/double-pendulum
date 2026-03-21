#ifndef DP_UTIL_H
#define DP_UTIL_H

#include <math.h>

int dp_util_invert_2x2_(const double m[2][2], double inv[2][2]);
double dp_util_clamp_(const double v, const double min, const double max);

#endif /* DP_UTIL_H */
