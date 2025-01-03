#include "abstract_syntax_tree.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "types.h"
#include "util.h"

#define MIN_DEF_LEN 4

#define ASSIGNMENT_TOKEN "="

#define PARAM_START_TOKEN "("
#define PARAM_END_TOKEN ")"
#define PARAM_START 3
#define PARAM_SEP ","
#define PARAM_MIN_TOKENS 3

var *create_var_node(vec *tokenv, line *curr_line) {
    return NULL;
}

function_node *create_function_node(vec *tokenv, line *curr_line) {
    function_node *func_node = function_node_new(
        vec_get(tokenv, curr_line->start),
        vec_get(tokenv, curr_line->start + 1));

    size_t i = curr_line->start + PARAM_START;
    while (i < curr_line->end) {
        assert_complete_definition(PARAM_MIN_TOKENS, i, curr_line);

        char *type = vec_get(tokenv, i++);
        assert_valid_type(type, curr_line);

        char *param_name = vec_get(tokenv, i++);
        assert_valid_symbol(param_name, curr_line);

        assert_token_equals(vec_get(tokenv, i), i + 1 == curr_line->end ? PARAM_END_TOKEN : PARAM_SEP, curr_line);
        i++;
        namespace *x = &func_node->ns;
        assert_unique_var(param_name, x, curr_line);

        vec_push(func_node->ns.vars, var_new(type, param_name));
    }

    vec_iter(var *v, func_node->ns.vars, printf("%s %s\n", v->type, v->name));
    return func_node;
}

ast_node *symbol_definition(vec *tokenv, line *curr_line, int *expected_indent) {
    char *symbol = vec_get(tokenv, curr_line->start + 1);
    assert_valid_symbol(symbol, curr_line);

    ast_node *node = malloc(sizeof(ast_node));

    char *token = vec_get(tokenv, curr_line->start + 2);
    if (strcmp(token, ASSIGNMENT_TOKEN) == 0) {
        // TODO
    } else if (strcmp(token, PARAM_START_TOKEN) == 0) {
        node->node = create_function_node(tokenv, curr_line);
        *expected_indent++;
    } else {
        raise_compiler_error("Invalid Definition", curr_line->line_num);
    }

    return node;
}

ast_node *create_ast_node(vec *tokenv, line *curr_line, int *expected_indent) {
    char *token = vec_get(tokenv, curr_line->start);
    if (valid_type(token)) {
        assert_complete_definition(MIN_DEF_LEN, curr_line->start, curr_line);
        return symbol_definition(tokenv, curr_line, expected_indent);
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

        ast_node *node = create_ast_node(tokenv, curr_line, &expected_indent);
        exit(0);

        curr_line = next_line(&iter);
    }

    return NULL;
}
