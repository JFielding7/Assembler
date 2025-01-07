#include <stdio.h>
#include <string.h>

#include "assembly_generator.h"
#include "expression.h"
#include "types.h"
#include "pattern.h"
#include "util.h"

#define COMMON_PRECEDENCE_GROUPS 2
#define MAX_OPERATORS_PER_GROUP 3

typedef struct operator_s {
    char *operator_token;
    ast_node *(*parse_func)(expression_parser*);
} operator;

static operator operators[COMMON_PRECEDENCE_GROUPS][MAX_OPERATORS_PER_GROUP + 1] = {
    {{.operator_token = "=", .parse_func = &assignment}, {}},
    {{"+", &binary_operator}, {"-", &binary_operator}, {}},
};

static int match_parens(vec *tokenv, size_t start, size_t end, size_t *matches) {
    printf("Matching Parens\n");
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

    printf("Len: %lu\n", open_parens->len);
    return open_parens->len != 0;
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
    puts("Assignment");
    var_node *var = get_var(parser);
    if (var == NULL) {
        raise_compiler_error("Invalid Assignment", parser->line_num);
    }

    expression_parser val_parser = *parser;
    val_parser.start = parser->token_index + 1;
    ast_node *val = parse_sub_expression(&val_parser);

    assignment_node *node = assignment_node_new(var, val);
    return ast_node_new(node, &assignment_assembly);
}

static ast_node *compile_operator(expression_parser *parser) {
    for (operator *op_group = operators[parser->op_group_index]; op_group->operator_token != NULL; op_group++) {
        if (strcmp(parser->token, op_group->operator_token) == 0) {
            return (*op_group->parse_func)(parser);
        }
    }

    return NULL;
}

static ast_node *parse_parenthetical_expression(expression_parser *parser) {
    parser->start++;
    parser->end--;
    return parse_sub_expression(parser);
}

static ast_node *parse_value(expression_parser *parser) {
    char *token = vec_get(parser->tokenv, parser->start);
    type *literal_type = get_literal_type(token);
    if (literal_type != NULL) {
        printf("Literal Type %s\n", literal_type->name);
        return ast_node_new(literal_node_new(literal_type, token), &literal_assembly);
    }

    var_node *var = var_lookup(parser->ns, token);
    if (var != NULL) {
        return ast_node_new(var, &load_assembly);
    }

    return NULL;
}

static ast_node *parse_sub_expression(expression_parser *parser) {
    for (size_t i = parser->start; i < parser->end; i++) {
        printf("%s ", (char*) vec_get(parser->tokenv, i));
    }
    puts("");

    if (parser->start + 1 == parser->end) {
        return parse_value(parser);
    }

    vec *tokenv = parser->tokenv;
    if (strcmp(vec_get(tokenv, parser->start), PAREN_OPEN) == 0
        && strcmp(vec_get(tokenv, parser->end), PAREN_CLOSE) == 0) {
        return parse_parenthetical_expression(parser);
    }

    for (; parser->op_group_index < COMMON_PRECEDENCE_GROUPS; parser->op_group_index++) {

        parser->token_index = parser->end - 1;
        while (parser->token_index >= parser->start) {

            parser->token = vec_get(tokenv, parser->token_index);
            puts(parser->token);
            if (strcmp(parser->token, PAREN_CLOSE) == 0) {
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

ast_node *parse_expression(vec *tokenv, size_t start, size_t end, size_t line_num, namespace *ns) {
    size_t paren_matches[end - start];
    int ret = match_parens(tokenv, start, end, paren_matches);
    if (ret != 0) {
        raise_compiler_error("Mismatched Parentheses", line_num);
    }

    expression_parser parser;
    init_expression_parser(&parser, tokenv, start, end, line_num, ns);
    return parse_sub_expression(&parser);
}
