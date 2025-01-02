#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

#define vec_iter(e, v, closure) \
for (size_t i = 0; i < v->len; i++) { \
    e = vec_get(v, i); \
    closure; \
}

typedef struct vec_s {
    void **buffer;
    size_t len;
    size_t capacity;
} vec;

vec *vec_new();

void *vec_get(vec *v, size_t i);

void vec_set(vec *v, size_t i, void *element);

void vec_push(vec *v, void *element);

bool vec_conatins(vec *v, void *element, int (*cmp)(void*, void*));

void vec_str_print(vec *v);

void vec_free(vec *v);

void vec_free_all(vec *v);

#endif //VEC_H
