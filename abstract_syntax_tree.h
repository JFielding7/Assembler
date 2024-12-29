#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include "vec.h"

#define OPERATOR_PRECEDENCE { \
    "=", NULL, \
    "+", "-", NULL, \
    "*", "/", NULL  \
}

typedef struct ast_node_s {
    unsigned char node_type;
    void *node;
} ast_node;

typedef struct body_s {
    vec *statements;
} body;

typedef struct function_s {
    vec *params;
    body *func_body;
} function;

typedef struct if_statement_s {
    ast_node *condition;
    body *if_body;
    body *else_body;
} if_statement;

typedef struct binary_operation_s {
    unsigned char operation;
    ast_node *left;
    ast_node *right;
} binary_operation;

typedef struct unary_operation_s {
    unsigned char operation;
    ast_node *operand;
} unary_operation;

typedef struct var_s {
    char *name;
    unsigned char type;
} var;

typedef struct literal_s {
    void *value;
    unsigned char type;
} literal;

#endif //ABSTRACT_SYNTAX_TREE_H
