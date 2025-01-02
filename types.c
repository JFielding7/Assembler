#include "types.h"

#include <string.h>

#include "vec.h"

static vec *types;

bool valid_type(char *type) {
    vec_iter(char *curr, types, {
        if (strcmp(type, curr) == 0) {
            return true;
        }
    })

    return false;
}

