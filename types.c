#include "types.h"

#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "vec.h"

#define i64_SIZE 8
#define STR_SIZE 8

static vec *types;

type *new_native_type(char *name, size_t size, bool (*validate_literal)(char*)) {
    type *data_type = malloc(sizeof(type));
    data_type->name = name;
    data_type->size = size;
    data_type->validate_literal = validate_literal;
    return data_type;
}

void compile_native_types() {
    types = vec_new();
    vec_push(types, new_native_type("i64", i64_SIZE, &valid_i64_literal));
    vec_push(types, new_native_type("str", STR_SIZE, &valid_string_literal));
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

type *get_literal_type(char *literal) {
    vec_iter(type *curr_type, types, {
        if ((*curr_type->validate_literal)(literal)) {
            return curr_type;
        }
    })

    return NULL;
}
