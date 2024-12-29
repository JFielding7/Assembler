#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "vec.h"

typedef struct hashtable_s {
  vec *ht;
  size_t (*hash_func)(void *);
} hash_table;

#endif //HASHTABLE_H
