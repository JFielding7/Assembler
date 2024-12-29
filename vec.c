#include "vec.h"
#include <stdlib.h>

vec *vec_alloc(size_t element_size) {
    vec *v = malloc(sizeof(vec));
    v->buffer = malloc(INITIAL_CAPACITY * element_size);
    v->capacity = INITIAL_CAPACITY;
    v->element_size = element_size;
    v->len = 0;
    return v;
}

vec *vec_zero(size_t len, size_t element_size) {
    vec *v = malloc(sizeof(vec));
    v->buffer = calloc(len, element_size);
    v->capacity = len;
    v->element_size = element_size;
    v->len = len;
    return v;
}

void vec_double_capacity(vec *v) {
    v->capacity <<= 1;
    v->buffer = realloc(v->buffer, v->capacity * v->element_size);
}

void vec_set_element(vec *v, size_t i, void *element) {
    unsigned char *curr_element_byte = element;
    unsigned char *curr_byte = v->buffer + i * v->element_size;
    const unsigned char *end_byte = curr_byte + v->element_size;

    while (curr_byte < end_byte) {
        *curr_byte++ = *curr_element_byte++;
    }
}

void vec_append(vec *v, void *element) {
    if (v->len == v->capacity) {
        vec_double_capacity(v);
    }

    vec_set_element(v, v->len, element);
    v->len++;
}

void vec_free(vec *v) {
    free(v->buffer);
    free(v);
}

void vec_free_all(vec *v) {
    for (size_t i = 0; i < v->len; i++) {
        free(*(char**)(v->buffer + i * v->element_size));
    }

    vec_free(v);
}
