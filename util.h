#ifndef UTIL_H
#define UTIL_H

#define STR_HASH_PRIME_BASE 37
#define STR_HASH_MOD 1000000007
#include <stddef.h>

size_t str_hash(void *str);

int str_cmp(void *str0, void *str1);

#endif //UTIL_H
