#include "ast.h"
#include "ast_node.h"
#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembly_generator.h"
#include "types.h"
#include "util.h"

#define MIN_DEF_LEN 4

#define ASSIGNMENT "="
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define MOD "%"

#define PAREN_OPEN "("
#define PAREN_CLOSE ")"

#define PARAM_START 3
#define PARAM_SEP ","
#define PARAM_MIN_TOKENS 3

#define COMMON_PRECEDENCE_GROUPS 2
#define MAX_OPERATORS_PER_GROUP 3

typedef struct expression_parser_s {
    vec *tokenv;
    char *token;
    size_t token_index;
    size_t expr_start;
    size_t start;
    size_t end;
    size_t op_group_index;
    size_t line_num;
    size_t *paren_matches;
    namespace *ns;

} expression_parser;

typedef struct operator_s {
    char *operator_token;
    ast_node *(*parse_func)(expression_parser*);
} operator;

static ast_node *assignment(expression_parser *parser);

static ast_node *binary_operator(expression_parser *parser);

static ast_node *parse_sub_expression(expression_parser *parser);

static operator operators[COMMON_PRECEDENCE_GROUPS][MAX_OPERATORS_PER_GROUP + 1] = {
    {{.operator_token = "=", .parse_func = &assignment}, 0},
    {{"+", &binary_operator}, {"-", &binary_operator}, 0},
};

static int match_parens(vec *tokenv, size_t start, size_t end, size_t *matches) {
    vec *open_parens = vec_new();

    for (size_t i = start; i < end; i++) {
        char *token = vec_get(tokenv, start);
        if (strcmp(token, PAREN_OPEN) == 0) {
            vec_push_val(open_parens, i);
        } else if (strcmp(token, PAREN_CLOSE) == 0) {
            if (open_parens->len == 0) {
                return 1;
            }
            matches[i - start] = vec_pop_val(open_parens, size_t);
        }
    }

    return open_parens->len == 0;
}

var_node *get_var(expression_parser *parser) {
    if (parser->token_index - parser->expr_start != 1) {
        return NULL;
    }
    return var_lookup(parser->ns, vec_get(parser->tokenv, parser->token_index - 1));
}

static ast_node *binary_operator(expression_parser *parser) {
    return NULL;
}

static ast_node *assignment(expression_parser *parser) {
    var_node *var = get_var(parser);
    if (var == NULL) {
        raise_compiler_error("Invalid Assginemnt", parser->line_num);
    }

    expression_parser val_parser = *parser;
    val_parser.start = parser->token_index + 1;
    ast_node *val = parse_sub_expression(&val_parser);

    assignment_node *node = assignment_node_new(var, val);
    return ast_node_new(node, &assignment_assembly);
}

static ast_node *compile_operator(expression_parser *parser) {
    for (operator *op_group = operators[parser->op_group_index]; op_group != NULL; op_group++) {
        if (strcmp(parser->token, op_group->operator_token)) {
            return (*op_group->parse_func)(parser);
        }
    }

    return NULL;
}

static ast_node *parse_sub_expression(expression_parser *parser) {
    vec *tokenv = parser->tokenv;
    if (strcmp(vec_get(tokenv, parser->start), PAREN_OPEN) == 0
        && strcmp(vec_get(tokenv, parser->end), PAREN_CLOSE) == 0) {
        parser->start++;
        parser->end--;
        return parse_sub_expression(parser);
    }

    for (; parser->op_group_index < COMMON_PRECEDENCE_GROUPS; parser->op_group_index++) {
        parser->token_index = parser->end - 1;
        while (parser->token_index >= parser->start) {
            parser->token = vec_get(tokenv, parser->token_index);
            if (strcmp(parser->token, PAREN_CLOSE)) {
                parser->token_index = parser->paren_matches[parser->token_index - parser->expr_start];
            }
            else {
                ast_node *operator_node = compile_operator(parser);
                if (operator_node != NULL) {
                    return operator_node;
                }
            }
            parser->token_index--;
        }
    }

    // figure out compiler error
    return NULL;
}

static void init_expression_parser(expression_parser *parser, vec *tokenv, size_t start, size_t end, size_t line_num, namespace *ns) {
    parser->tokenv = tokenv;
    parser->expr_start = start;
    parser->start = start;
    parser->end = end;
    parser->line_num = line_num;
    parser->op_group_index = 0;
    parser->ns = ns;
}

static ast_node *parse_expression(vec *tokenv, size_t start, size_t end, size_t line_num, namespace *ns) {
    size_t paren_matches[end - start];
    int ret = match_parens(tokenv, start, end, paren_matches);
    if (ret != 0) {
        raise_compiler_error("Mismatched Parentheses", line_num);
    }

    expression_parser parser;
    init_expression_parser(&parser, tokenv, start, end, line_num, ns);
    return parse_sub_expression(&parser);
}

static ast_node *var_def_node(vec *tokenv, line *curr_line, namespace *ns) {
    var_node *var_node = var_new(
        vec_get(tokenv, curr_line->start),
        vec_get(tokenv, curr_line->start + 1));

    assert_unique_var(var_node->name, ns, curr_line);
    vec_push(ns->vars, var_node);

    return ast_node_new(
        assignment_node_new(var_node,
        parse_expression(tokenv, curr_line->start + (MIN_DEF_LEN - 1), curr_line->end, curr_line->line_num, ns)),
        &assignment_assembly);
}

static ast_node *function_def_node(vec *tokenv, line *curr_line, vec *namespaces) {
    function_node *func_node = function_def_node_new(
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
        assert_unique_var(param_name, &func_node->ns, curr_line);

        vec_push(func_node->ns.vars, var_new(type, param_name));
    }

    vec_push(namespaces, &func_node->ns);
    return ast_node_new(func_node, &function_assembly);
}

static ast_node *symbol_definition(vec *tokenv, line *curr_line, vec *namespaces) {
    char *symbol = vec_get(tokenv, curr_line->start + 1);
    assert_valid_symbol(symbol, curr_line);

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

static ast_node *create_ast_node(vec *tokenv, line *curr_line, vec *ns) {
    char *token = vec_get(tokenv, curr_line->start);
    if (valid_type(token)) {
        assert_complete_definition(MIN_DEF_LEN, curr_line->start, curr_line);
        return symbol_definition(tokenv, curr_line, ns);
    }

    return NULL;
}

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec *tokenv) {
    vec *namespaces = vec_new();

    line_iterator iter;
    init_line_iterator(&iter, tokenv);

    line *curr_line = next_line(&iter);
    while (curr_line != NULL) {
        ast_node *node = create_ast_node(tokenv, curr_line, namespaces);

        curr_line = next_line(&iter);
    }

    return NULL;
}
