#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#define INITIAL_CAPACITY 16

#define new_vec(type) (vec_alloc(sizeof(type)))

#define vec_get(v, type, i) (((type*) (v->buffer))[i])

#define vec_add(v, type, e) { \
    type element##__COUNTER__ = e; \
    vec_append((v), &element##__COUNTER__); \
}

#define vec_print(v, type, format) { \
    printf("["); \
    for (size_t i = 0; i < v->len - 1; i++) { \
        printf(format, ((type*) v->buffer)[i]); \
        printf(", "); \
    } \
    if (v->len > 0) { \
        printf(format, ((type*) v->buffer)[v->len - 1]); \
    } \
    printf("]\n"); \
}

typedef struct vec_s {
    void *buffer;
    size_t len;
    size_t capacity;
    size_t element_size;
} vec;

vec *vec_alloc(size_t element_size);

void vec_append(vec *v, void *element);

void vec_free(vec *v);

#endif //VEC_H
