#include "vec.h"

#include <stdio.h>
#include <stdlib.h>

#define ALLOCATION_SHIFT 3

vec *vec_new() {
    vec *v = malloc(sizeof(vec));
    v->buffer = malloc(INITIAL_CAPACITY << ALLOCATION_SHIFT);
    v->capacity = INITIAL_CAPACITY;
    v->len = 0;
    return v;
}

void vec_double_capacity(vec *v) {
    v->capacity <<= 1;
    v->buffer = realloc(v->buffer, v->capacity << ALLOCATION_SHIFT);
}

void vec_set(vec *v, size_t i, void *element) {
    ((void**) v->buffer)[i] = element;
}

void vec_push(vec *v, void *element) {
    if (v->len == v->capacity) {
        vec_double_capacity(v);
    }

    vec_set(v, v->len, element);
    v->len++;
}

bool vec_conatins(vec *v, void *element, int (*cmp)(void*, void*)) {
    for (size_t i = 0; i < v->len; i++) {
    }
}

void vec_str_print(vec *v) {
    char **strings = v->buffer;

    printf("[");
    if (v->len > 0) {
        for (size_t i = 0; i < v->len - 1; i++) {
            printf("%s, ", strings[i]);
        }
        printf("%s", strings[v->len - 1]);
    }
    printf("]\n");
}

void vec_free(vec *v) {
    free(v->buffer);
    free(v);
}

void vec_free_all(vec *v) {
    for (size_t i = 0; i < v->len; i++) {
        free(*(void**)(v->buffer + (i << ALLOCATION_SHIFT)));
    }

    vec_free(v);
}
