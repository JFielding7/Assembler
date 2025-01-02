#include "abstract_syntax_tree.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "util.h"

#define MIN_DEF_LEN 4

#define ASSIGNMENT_TOKEN "="
#define FUNC_PARAM_START_TOKEN "("

function *create_function_node(vec *tokenv, line *curr_line) {

}



ast_node *symbol_definition(vec *tokenv, line *curr_line) {
    char *symbol = vec_get(tokenv, curr_line->start + 1);
    if (!valid_symbol(symbol)) {
        raise_compiler_error("Invalid symbol `%s`", curr_line->line_num, symbol);
    }

    ast_node *node = malloc(sizeof(ast_node));

    char *token = vec_get(tokenv, curr_line->start + 2);
    if (strcmp(token, ASSIGNMENT_TOKEN) == 0) {

    } else if (strcmp(token, FUNC_PARAM_START_TOKEN) == 0) {
        create_function_node(tokenv, curr_line);
    } else {
        raise_compiler_error("Invalid Definition", curr_line->line_num);
    }

    return node;
}

ast_node *create_ast_node(vec *tokenv, line *curr_line) {
    char *token = vec_get(tokenv, curr_line->start);
    if (valid_type(token)) {
        if (curr_line->end - curr_line->start < MIN_DEF_LEN) {
            raise_compiler_error("Incomplete Defintion", curr_line->line_num);
        }
        return symbol_definition(tokenv, curr_line);
    }
    return NULL;
}

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec *tokenv) {
    line_iterator iter;
    init_line_iterator(&iter, tokenv);

    int expected_indent = 0;
    line *curr_line = next_line(&iter);
    while (curr_line != NULL) {
        if (curr_line->indent != expected_indent) {
            raise_compiler_error("Unexpected Indent Size", curr_line->line_num);
        }

        curr_line = next_line(&iter);
    }
}
