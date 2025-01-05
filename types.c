#include "types.h"

#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define NATIVE_TYPES_COUNT 2

static vec *types;

type *new_type(char *name, size_t size) {
    type *data_type = malloc(sizeof(type));
    data_type->name = name;
    data_type->size = size;
    return data_type;
}

void compile_native_types() {
    types = vec_new();
    char *native_types[NATIVE_TYPES_COUNT] = {"i32", "i64"};
    int native_sizes[NATIVE_TYPES_COUNT] = {4, 8};

    for (size_t i = 0; i < NATIVE_TYPES_COUNT; i++) {
        vec_push(types, new_type(native_types[i], native_sizes[i]));
    }
}

void free_types() {
    free_vec_and_elements(types);
}

bool valid_type(char *type_name) {
    vec_iter(type *curr, types, {
        if (strcmp(type_name, curr->name) == 0) {
            return true;
        }
    })

    return false;
}
