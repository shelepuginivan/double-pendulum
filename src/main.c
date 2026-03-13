#include <stdio.h>
#include <stdlib.h>

#include "methods.h"
#include "state.h"
#include "system.h"

int main() {
    DpSystem *system = dp_system_new_from_env();
    DpState *state = dp_state_new_from_system(system);

    FILE *out = fopen("data.csv", "w");
    if (out == NULL) {
        perror("cannot open file:");
        return 1;
    }

    for (int i = 0; i < 200000; i++) {
        dp_state_write(state, system, out);
        dp_rk4(state, system);
    }

    fclose(out);
    dp_state_destroy(state);
    dp_system_destroy(system);

    return 0;
}
