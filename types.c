#include "types.h"

#include <string.h>

#include "vec.h"

static vec *types;

void compile_types() {
    types = vec_new();
    char* native_types[3] = {"i32", "i64", NULL};

    for (char **type = native_types; *type != NULL; type++) {
        vec_push(types, *type);
    }
}

bool valid_type(char *type) {
    vec_iter(char *curr, types, {
        if (strcmp(type, curr) == 0) {
            return true;
        }
    })

    return false;
}
