#include "util.h"
#include <stddef.h>
#include <string.h>

#include <stdio.h>

size_t str_hash(void *str) {
    unsigned long hash = 0;
    unsigned long pow = 1;

    // printf("Str: %s\n", (char*) str);
    for (char *curr = str; *curr != '\0'; curr++) {
        hash = (hash + *curr * pow) % STR_HASH_MOD;
        pow = pow * STR_HASH_PRIME_BASE % STR_HASH_MOD;
    }

    return hash;
}

int str_cmp(void *str0, void *str1) {
    return strcmp(str0, str1);
}