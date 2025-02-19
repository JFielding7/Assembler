#include "vec.h"

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16
#define ALLOCATION_SHIFT 3

struct vec_s {
    void **buffer;
    size_t len;
    size_t capacity;
};

vec vec_new() {
    vec v = malloc(sizeof(vec));
    v->buffer = malloc(INITIAL_CAPACITY << ALLOCATION_SHIFT);
    v->capacity = INITIAL_CAPACITY;
    v->len = 0;
    return v;
}

size_t vec_len(vec v) {
    return v->len;
}

void vec_double_capacity(vec v) {
    v->capacity <<= 1;
    v->buffer = realloc(v->buffer, v->capacity << ALLOCATION_SHIFT);
}

void *vec_get(vec v, size_t i) {
    return v->buffer[i];
}

void vec_set(vec v, size_t i, void *element) {
    v->buffer[i] = element;
}

void vec_push(vec v, void *element) {
    if (v->len == v->capacity) {
        vec_double_capacity(v);
    }

    vec_set(v, v->len, element);
    v->len++;
}

void *vec_pop(vec v) {
    return v->buffer[--v->len];
}

void *vec_peek_end(vec v) {
    return v->buffer[v->len - 1];
}

bool vec_conatins(vec v, void *element, int (*cmp)(void*, void*)) {
    vec_iter(void *curr, v, {
        if (cmp(element, curr) == 0)
            return true;
    })

    return false;
}

void vec_str_print(vec v) {
    char **strings = (char**) v->buffer;

    printf("[");
    if (v->len > 0) {
        for (size_t i = 0; i < v->len - 1; i++) {
            printf("%s, ", strings[i]);
        }
        printf("%s", strings[v->len - 1]);
    }
    printf("]\n");
}

void vec_free(vec v) {
    free(v->buffer);
    free(v);
}

void free_vec_and_elements(vec v) {
    vec_iter(void *element, v, free(element))
    vec_free(v);
}
