#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>

typedef struct type_s {
    char *name;
    size_t size;
} type;

void compile_native_types();

void free_types();

type *new_type(char *name, size_t size);

bool valid_type(char *type);

#endif //TYPES_H
