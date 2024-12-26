#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "source_code_files.h"
#include "tokenizer.h"
#include "vec.h"

#define MIN_ARG_COUNT 2

int main(int argc, char *argv[]) {
    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "%s: fatal error: no input files\n", argv[0]);
        return 1;
    }

    vec *source_code_files = read_source_code(argv + 1);
    if (source_code_files == NULL) {
        return 1;
    }

    vec *tokenv = tokenize_source_code_files(source_code_files);
    vec_print(tokenv, char*, "%s");

    free_vec_and_elements(source_code_files);
    vec_free(tokenv);

    return 0;
}
