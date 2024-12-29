#include "vec.h"
#include <stdlib.h>
#include <stdio.h>

vec *vec_alloc(size_t element_size) {
    vec *v = malloc(sizeof(vec));
    v->buffer = malloc(INITIAL_CAPACITY * element_size);
    v->capacity = INITIAL_CAPACITY;
    v->element_size = element_size;
    v->len = 0;
    return v;
}

void resize(vec *v) {
    v->capacity <<= 1;
    v->buffer = realloc(v->buffer, v->capacity * v->element_size);
}

void vec_append(vec *v, void *element) {
    if (v->len == v->capacity) {
        resize(v);
    }

    unsigned char *curr_element_byte = element;
    unsigned char *curr_byte = v->buffer + v->len * v->element_size;
    const unsigned char *end_byte = curr_byte + v->element_size;

    while (curr_byte < end_byte) {
        *curr_byte++ = *curr_element_byte++;
    }

    v->len++;
}

void vec_free(vec *v) {
    free(v->buffer);
    free(v);
}

void free_vec_and_elements(vec *v) {
    for (size_t i = 0; i < v->len; i++) {
        free(*(char**)(v->buffer + i * v->element_size));
    }

    vec_free(v);
}
