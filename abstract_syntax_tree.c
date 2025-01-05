#include "abstract_syntax_tree.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"

#define MIN_DEF_LEN 4

#define ASSIGNMENT_TOKEN "="

#define PAREN_OPEN "("
#define PAREN_CLOSE ")"

#define PARAM_START 3
#define PARAM_SEP ","
#define PARAM_MIN_TOKENS 3

#define COMMON_PRECEDENCE_GROUPS 3
#define MAX_OPERATORS_PER_GROUP 3
#define MAX_OPERATOR_LEN 2

#define operators_iter(op, group_index, closure) \
for (char **curr_op = operators[group_index]; *curr_op != NULL; curr_op++) { \
    op = *curr_op; \
    closure; \
}

static char operators[COMMON_PRECEDENCE_GROUPS][MAX_OPERATORS_PER_GROUP + 1][MAX_OPERATOR_LEN] = {
    {"=", 0},
    {"+", "-", 0},
    {"*", "/", "%", 0},
};



int match_parens(vec *tokenv, size_t start, size_t end, size_t *matches) {
    vec *open_parens = vec_new();

    for (size_t i = start; i < end; i++) {
        char *token = vec_get(tokenv, start);
        if (strcmp(token, PAREN_OPEN) == 0) {
            vec_push_val(open_parens, i);
        } else if (strcmp(token, PAREN_CLOSE) == 0) {
            if (open_parens->len == 0) {
                return 1;
            }
            matches[vec_pop_val(open_parens, size_t) - start] = i;
        }
    }

    return open_parens->len == 0;
}

ast_node *parse_expression(vec *tokenv, size_t start, size_t end, size_t line_num) {
    size_t parens_match[end - start];
    int ret = match_parens(tokenv, start, end, parens_match);
    if (ret != 0) {
        raise_compiler_error("Mismatched Parentheses", line_num);
    }

    for (size_t precedence_group = 0; precedence_group < COMMON_PRECEDENCE_GROUPS; precedence_group++) {
        operators_iter(char *op, precedence_group, {

        })
    }
}

ast_node *var_def_node(vec *tokenv, line *curr_line) {
    var *var_node = var_new(
        vec_get(tokenv, curr_line->start),
        vec_get(tokenv, curr_line->start + 1));

    ast_node *node = malloc(sizeof(ast_node));
    node->node = var_node;

    return node;
}

ast_node *create_function_node(vec *tokenv, line *curr_line) {
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

        assert_token_equals(vec_get(tokenv, i), i + 1 == curr_line->end ? PAREN_CLOSE : PARAM_SEP, curr_line);
        i++;
        namespace *x = &func_node->ns;
        assert_unique_var(param_name, x, curr_line);

        vec_push(func_node->ns.vars, var_new(type, param_name));
    }

    ast_node *node = malloc(sizeof(ast_node));
    node->node = func_node;

    return node;
}

ast_node *symbol_definition(vec *tokenv, line *curr_line) {
    char *symbol = vec_get(tokenv, curr_line->start + 1);
    assert_valid_symbol(symbol, curr_line);

    char *token = vec_get(tokenv, curr_line->start + 2);
    if (strcmp(token, ASSIGNMENT_TOKEN) == 0) {
        // TODO
    } else if (strcmp(token, PAREN_OPEN) == 0) {
        return create_function_node(tokenv, curr_line);
    } else {
        raise_compiler_error("Invalid Definition", curr_line->line_num);
    }

    return NULL;
}

ast_node *create_ast_node(vec *tokenv, line *curr_line) {
    char *token = vec_get(tokenv, curr_line->start);
    if (valid_type(token)) {
        assert_complete_definition(MIN_DEF_LEN, curr_line->start, curr_line);
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

    // int expected_indent = 0;
    line *curr_line = next_line(&iter);

    while (curr_line != NULL) {
        // if (curr_line->indent != expected_indent) {
        //     raise_compiler_error("Unexpected Indent Size", curr_line->line_num);
        // }

        ast_node *node = create_ast_node(tokenv, curr_line);

        curr_line = next_line(&iter);
    }

    return NULL;
}
