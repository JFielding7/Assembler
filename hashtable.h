#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "vec.h"

#include <stdbool.h>

#define hash_table_new(type, hash_func, cmp) (hash_table_alloc(sizeof(type), hash_func, cmp))

typedef struct hashtable_s {
  size_t len;
  vec *ht;
  size_t (*hash_func)(void*);
  int (*cmp)(void*, void*);
} hash_table;

hash_table *hash_table_alloc(size_t element_size, size_t (*hash_func)(void*), int (*cmp)(void*, void*));

void hash_table_insert(hash_table *ht, void *value);

bool hash_table_contains(hash_table *ht, void *value);

void hash_table_print(hash_table *ht, char *format);

#endif //HASHTABLE_H
