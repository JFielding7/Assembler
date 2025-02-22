#include <stdio.h>
#include <string.h>

#include "assembly_generator.h"
#include "expression.h"
#include "types.h"
#include "pattern.h"
#include "util.h"

#define COMMON_PRECEDENCE_GROUPS 3
#define MAX_OPERATORS_PER_GROUP 3

#define ASSIGNMENT "="
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define MOD "%"

#define PAREN_OPEN "("
#define PAREN_CLOSE ")"

typedef struct expression_parser_s {
    vec tokenv;
    line *line;
    char *token;
    size_t token_index;
    size_t expr_start;
    size_t start;
    size_t end;
    size_t op_group_index;
    size_t *paren_matches;
    namespace *ns;
} expression_parser;

typedef struct operator_s {
    char *operator_token;
    ast_node *(*parse_func)(expression_parser*);
} operator;

static ast_node *assignment_parser(expression_parser *parser);
static ast_node *mul_parser(expression_parser *parser);
static ast_node *div_parser(expression_parser *parser);
static ast_node *mod_parser(expression_parser *parser);
static ast_node *add_parser(expression_parser *parser);
static ast_node *sub_parser(expression_parser *parser);
static ast_node *parse_sub_expression(expression_parser *parser);

operator operators[COMMON_PRECEDENCE_GROUPS][MAX_OPERATORS_PER_GROUP + 1] = {
    {
        {.operator_token = "=", .parse_func = &assignment_parser},
        {}
    },
    {
        {"+", &add_parser},
        {"-", &sub_parser},
        {}
    },
    {
        {.operator_token = "*", .parse_func = &mul_parser},
        {.operator_token = "/", .parse_func = &div_parser},
        {.operator_token = "%", .parse_func = &mod_parser},
        {}
    },
};

static ast_node *binary_operation_parser(expression_parser *parser, void (*assembly_generator)(ast_node*)) {
    expression_parser left_parser = *parser;
    left_parser.end = parser->token_index;
    ast_node *left = parse_sub_expression(&left_parser);

    expression_parser right_parser = *parser;
    right_parser.start = parser->token_index + 1;
    ast_node *right = parse_sub_expression(&right_parser);

    return binary_operation_new(right->expr_type, left, right, assembly_generator);
}

static ast_node *mul_parser(expression_parser *parser) {
    return binary_operation_parser(parser, &mul_assembly);
}

static ast_node *div_parser(expression_parser *parser) {
    return binary_operation_parser(parser, &div_assembly);
}

static ast_node *mod_parser(expression_parser *parser) {
    return binary_operation_parser(parser, &mod_assembly);
}

static ast_node *add_parser(expression_parser *parser) {
    return binary_operation_parser(parser, &add_assembly);
}

static ast_node *sub_parser(expression_parser *parser) {
    return binary_operation_parser(parser, &sub_assembly);
}

static ast_node *assignment_parser(expression_parser *parser) {
    puts("Assignment");
    if (parser->token_index - parser->expr_start != 1) {
        raise_compiler_error("Invalid Assignment", parser->line);
    }
    ast_node *var_node = var_lookup(parser->ns, vec_get(parser->tokenv, parser->token_index - 1));

    expression_parser val_parser = *parser;
    val_parser.start = parser->token_index + 1;
    ast_node *value = parse_sub_expression(&val_parser);

    return binary_operation_new(value->expr_type, var_node, value, &assignment_assembly);
}

static ast_node *compile_operator(expression_parser *parser) {
    for (operator *op = operators[parser->op_group_index]; op->operator_token != NULL; op++) {
        if (strcmp(parser->token, op->operator_token) == 0) {
            return (*op->parse_func)(parser);
        }
    }

    return NULL;
}

static ast_node *parse_parenthetical_expression(expression_parser *parser) {
    printf("Parsing parens %lu %lu", parser->start, parser->end);
    parser->start++;
    parser->end--;
    parser->op_group_index = 0;
    return parse_sub_expression(parser);
}

static ast_node *parse_value(expression_parser *parser) {
    char *token = vec_get(parser->tokenv, parser->start);
    type *literal_type = get_literal_type(token);
    if (literal_type != NULL) {
        // printf("Literal Type %s\n", literal_type->name);
        return literal_node_new(literal_type, token);
    }

    ast_node *var = var_lookup(parser->ns, token);
    if (var != NULL) {
        return var_node_new(var->expr_type, token);
    }

    raise_compiler_error("Invalid Value", parser->line);
    return NULL;
}

static ast_node *parse_sub_expression(expression_parser *parser) {

    printf("Expr: ");
    for (size_t i = parser->start; i < parser->end; i++) {
        printf("%s ", (char*) vec_get(parser->tokenv, i));
    }
    puts("");

    if (parser->start + 1 == parser->end) {
        return parse_value(parser);
    }

    vec tokenv = parser->tokenv;
    puts(vec_get(tokenv, parser->start));
    puts(vec_get(tokenv, parser->end));
    printf("Group: %lu\n", parser->op_group_index);

    if (strcmp(vec_get(tokenv, parser->start), PAREN_OPEN) == 0
        && strcmp(vec_get(tokenv, parser->end - 1), PAREN_CLOSE) == 0) {
        return parse_parenthetical_expression(parser);
    }

    for (; parser->op_group_index < COMMON_PRECEDENCE_GROUPS; parser->op_group_index++) {

        parser->token_index = parser->end - 1;
        while (parser->token_index >= parser->start) {
            parser->token = vec_get(tokenv, parser->token_index);
            printf("Token: %s\n", parser->token);

            if (strcmp(parser->token, PAREN_CLOSE) == 0) {
                for (size_t i = 0; i < parser->end - parser->start; i++) {
                    printf("%lu, ", parser->paren_matches[i]);
                }
                parser->token_index = parser->paren_matches[parser->token_index - parser->expr_start];
            }
            else {
                ast_node *operator_node = compile_operator(parser);
                if (operator_node != NULL) {
                    return operator_node;
                }
            }
            printf("Index: %lu\n", parser->token_index);
            parser->token_index--;
        }
    }

    // figure out compiler error
    return NULL;
}

/**
 * Finds and stores the matching pairs of parentheses in an array, raises fatal error if mismatched parens
 * @param parser Expression parser
 */
static void match_parens(expression_parser *parser) {
    vec open_parens = vec_new();

    for (size_t i = parser->start; i < parser->end; i++) {
        char *token = vec_get(parser->tokenv, i);

        if (strcmp(token, PAREN_OPEN) == 0) {
            vec_push_val(open_parens, i);
        } else if (strcmp(token, PAREN_CLOSE) == 0) {
            if (vec_len(open_parens) == 0) {
                raise_compiler_error("Mismatched Parentheses", parser->line);;
            }
            parser->paren_matches[i - parser->start] = vec_pop_val(open_parens, size_t);
        }
    }

    // printf("Len: %lu\n", vec_len(open_parens));
    if (vec_len(open_parens) != 0) {
        raise_compiler_error("Mismatched Parentheses", parser->line);
    }
}

ast_node *parse_expression(vec tokenv, line *curr_line, size_t start, size_t end, namespace *ns) {
    expression_parser parser;
    parser.tokenv = tokenv;
    parser.expr_start = start;
    parser.start = start;
    parser.end = end;
    parser.line = curr_line;
    parser.op_group_index = 0;
    parser.ns = ns;

    size_t paren_matches[end - start];
    parser.paren_matches = paren_matches;
    match_parens(&parser);

    return parse_sub_expression(&parser);
}
