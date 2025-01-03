#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstract_syntax_tree.h"
#include "pattern.h"
#include "tokenizer.h"
#include "types.h"
#include "vec.h"

#define MIN_ARG_COUNT 2

void allocate_resources() {
    compile_regexps();
    compile_types();
}

void deallocate_resources() {
    free_regexps();
}

int main(int argc, char *argv[]) {
    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "%s: fatal error: no input files\n", argv[0]);
        return 1;
    }

    allocate_resources();

    vec *tokenv = tokenize_source_code_files(argv + 1);

    generate_ast(tokenv);

    vec_free_all(tokenv);

    deallocate_resources();

    return 0;
}
