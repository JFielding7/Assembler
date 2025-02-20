#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

#define vec_iter(e, v, closure) \
for (size_t i = 0; i < vec_len(v); i++) { \
    e = vec_get(v, i); \
    closure; \
}

#define vec_push_val(v, e) vec_push(v, (void*) e)

#define vec_pop_val(v, type) ((type) vec_pop(v))

typedef struct vec_s* vec;

vec vec_new();

size_t vec_len(vec v);

void *vec_get(vec v, size_t i);

void vec_set(vec v, size_t i, void *element);

void vec_push(vec v, void *element);

void *vec_pop(vec v);

void *vec_peek_end(vec v);

bool vec_conatins(vec v, void *element, int (*cmp)(void*, void*));

void vec_str_print(vec v);

void vec_free(vec v);

void free_vec_and_elements(vec v);

#endif //VEC_H
