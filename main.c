#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "vec.h"
#include "hashtable.h"
#include "util.h"

#define MIN_ARG_COUNT 2

int main(int argc, char *argv[]) {
    // if (argc < MIN_ARG_COUNT) {
    //     fprintf(stderr, "%s: fatal error: no input files\n", argv[0]);
    //     return 1;
    // }
    //
    // vec *tokenv = tokenize_source_code_files(argv + 1);
    // vec_print(tokenv, char*, "%s");
    //
    // vec_free_all(tokenv);

    hash_table *ht = hash_table_new(str_hash, str_cmp);
    char *str = "abcdefghijklmn";
    //
    for (char *c = str; *c != '\0'; c++) {
        char *l = malloc(2);
        l[0] = *c;
        l[1] = '\0';
        hash_table_insert(ht, l);
    }

    hash_table_print(ht, "%s");
    printf("%lu\n", ht->len);

    hash_table_free(ht);

    // printf("%d", hash_table_contains(ht, "dk won"));

    return 0;
}
