#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>

typedef struct type_s {
    char *name;
    size_t size;
    bool (*validate_literal)(char*);
} type;

void compile_native_types();

void free_types();

bool valid_type(char *type);

type *get_literal_type(char *literal);

#endif //TYPES_H
