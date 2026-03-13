#include "env.h"

void dp_env_load_into_str(const char *name, char **dst) {
    char *envvar = getenv(name);
    if (envvar == NULL) {
        return;
    }
    *dst = envvar;
}

void dp_env_load_into_ulong(const char *name, unsigned long *dst) {
    char *envvar = getenv(name);
    if (envvar == NULL) {
        return;
    }

    char *endptr;
    unsigned long value = strtoul(envvar, &endptr, 0);
    if (endptr == envvar) {
        return;
    }

    *dst = value;
}

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
