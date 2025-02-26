#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "ast_node.h"
#include "pattern.h"
#include "tokenizer.h"
#include "types.h"
#include "vec.h"

#define MIN_ARG_COUNT 2

void allocate_resources() {
    compile_regexps();
    compile_native_types();
}

void deallocate_resources() {
    free_regexps();
    free_types();
}

int main(int argc, char *argv[]) {
    if (argc < MIN_ARG_COUNT) {
        fprintf(stderr, "%s: fatal error: no input files\n", argv[0]);
        return 1;
    }

    allocate_resources();

    vec tokenv = tokenize_file(argv[1]);
    generate_ast(argv[1], tokenv);
    free_vec_and_elements(tokenv);

    deallocate_resources();

    return 0;
}
