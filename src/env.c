#include "env.h"

void dp_env_load_into_double(const char *name, double *dst) {
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
