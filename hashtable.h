#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "vec.h"

typedef struct hashtable_s {
  size_t len;
  vec *ht;
  size_t (*hash_func)(void *);
} hash_table;

#endif //HASHTABLE_H
