#include "ast.h"
#include "ast_node.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "types.h"
#include "util.h"

#define MIN_SYMBOL_DEF_LEN 4
#define PARAM_START 3
#define PARAM_SEP ","
#define PARAM_MIN_TOKENS 3

#define ASSIGNMENT "="
#define PAREN_OPEN "("
#define PAREN_CLOSE ")"

static ast_node *var_def_node(vec tokenv, line *curr_line, namespace *ns) {
    puts("Defining var");
    // TODO: fix type
    ast_node *var_node = var_node_new(
        vec_get(tokenv, curr_line->start),
        vec_get(tokenv, curr_line->start + 1)
    );

    assert_unique_var(var_node->node, ns, curr_line);
    vec_push(ns->vars, var_node);

    return assignment_node_new(var_node,
        parse_expression(tokenv, curr_line->start + 3, curr_line->end, curr_line->line_num, ns));
}

/**
 * Creates AST Node for a function definition
 * @param tokenv Tokens
 * @param curr_line Current Line
 * @param namespaces
 * @return
 */
static ast_node *function_def_node(vec tokenv, line *curr_line, vec namespaces) {
    ast_node *node = function_def_node_new(
        vec_get(tokenv, curr_line->start),
        vec_get(tokenv, curr_line->start + 1)
    );
    function_node *func_node = node->node;

    size_t i = curr_line->start + PARAM_START;
    i += i + 1 == curr_line->end && strcmp(vec_get(tokenv, i), PAREN_CLOSE) == 0;

    while (i < curr_line->end) {
        assert_has_min_tokens(PARAM_MIN_TOKENS, i, curr_line);

        char *type_name = vec_get(tokenv, i++);
        type *param_type = get_type(type_name);
        assert_valid_type(param_type, curr_line);

        char *param_name = vec_get(tokenv, i++);
        assert_valid_symbol(param_name, curr_line);

        assert_token_equals(vec_get(tokenv, i), i + 1 == curr_line->end ? PAREN_CLOSE : PARAM_SEP, curr_line);
        i++;
        assert_unique_var(param_name, &func_node->func_namespace, curr_line);

        // fix type, link with actual type structure
        vec_push(func_node->func_namespace.vars, var_node_new(param_type, param_name));
    }

    vec_push(namespaces, &func_node->func_namespace);
    return node;
}

static ast_node *symbol_definition(vec tokenv, line *curr_line, vec namespaces) {
    char *symbol = vec_get(tokenv, curr_line->start + 1);
    assert_valid_symbol(symbol, curr_line);

    // TODO, link actual type struct
    char *token = vec_get(tokenv, curr_line->start + 2);
    if (strcmp(token, ASSIGNMENT) == 0) {
        return var_def_node(tokenv, curr_line, vec_peek_end(namespaces));
    }
    if (strcmp(token, PAREN_OPEN) == 0) {
        return function_def_node(tokenv, curr_line, namespaces);
    }

    raise_compiler_error("Invalid Definition", curr_line->line_num);
    return NULL;
}

/**
 * Creates an abstract syntax tree node for the current line
 * @param tokenv Tokens
 * @param curr_line Current line
 * @param namespaces
 * @return ast_node: AST node for the current line
 */
static ast_node *create_ast_node(vec tokenv, line *curr_line, vec namespaces) {
    char *token = vec_get(tokenv, curr_line->start);

    if (valid_type(token)) {
        assert_has_min_tokens(MIN_SYMBOL_DEF_LEN, curr_line->start, curr_line);
        return symbol_definition(tokenv, curr_line, namespaces);
    }

    return parse_expression(tokenv, curr_line->start, curr_line->end, curr_line->line_num, vec_peek_end(namespaces));
}

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec tokenv) {
    vec namespaces = vec_new();

    line_iterator iter;
    init_line_iterator(&iter, tokenv);

    line *curr_line = next_line(&iter);
    while (curr_line != NULL) {
        ast_node *node = create_ast_node(tokenv, curr_line, namespaces);

        curr_line = next_line(&iter);
    }

    return NULL;
}
