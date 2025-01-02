#include "abstract_syntax_tree.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#define FUNCTION_DEFINITION "fn"

function *create_function_node(vec *tokenv, size_t *token_index, size_t line_num) {
    *token_index++;

    char *function_name = vec_get(tokenv, *token_index);
    if (invalid_symbol(function_name)) {
        fprintf(stderr, "ERROR on line %lu: Invalid function name `%s`", line_num, function_name);
    }


}

ast_node *create_ast_node(vec *tokenv, size_t *token_index) {
    char *token = vec_get(tokenv, *token_index);

    if (strcmp(token, FUNCTION_DEFINITION) == 0) {

    }
}

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec *tokenv) {
    line_iterator iter;
    init_line_iterator(&iter, tokenv);

    line *curr_line = next_line(&iter);
    while (curr_line != NULL) {
        printf("%lu. %lu.", curr_line->line_num, curr_line->indent);
        for (size_t i = curr_line->start; i < curr_line->end; i++) {
            printf("%s ", (char*) vec_get(tokenv, i));
        }
        printf("\n");


        curr_line = next_line(&iter);
    }
}
