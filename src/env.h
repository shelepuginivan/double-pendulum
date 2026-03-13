#ifndef DP_ENV_H
#define DP_ENV_H

#include <stdlib.h>

void dp_env_load_into_str(const char *name, char **dst);
void dp_env_load_into_ulong(const char *name, unsigned long *dst);
void dp_env_load_into_double(const char *name, double *dst);

#endif /* DP_ENV_H */
