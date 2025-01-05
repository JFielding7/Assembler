#ifndef ABSTRACT_SYNTAX_TREE_NODE_H
#define ABSTRACT_SYNTAX_TREE_NODE_H

#include "vec.h"

typedef struct ast_node_s {
    void *node;
    void (*add_child)(struct ast_node_s*, struct ast_node_s*);
} ast_node;

typedef struct if_statement_s {
    ast_node *condition;
    vec *if_body;
    vec *else_body;
} if_statement;

typedef struct binary_operation_s {
    void (*operation)(ast_node*, ast_node*);
    ast_node *left;
    ast_node *right;
} binary_operation;

typedef struct unary_operation_s {
    void (*operation)(ast_node*);
    ast_node *operand;
} unary_operation;

typedef struct var_s {
    char *type;
    char *name;
} var;

typedef struct literal_s {
    unsigned long type;
    void *value;
} literal;

typedef struct namespace_s {
    vec *vars;
    struct namespace_s *parent;
} namespace;

typedef struct function_s {
    char *type;
    char *name;
    vec *statements;
    namespace ns;
} function_node;

var *var_new(char *type, char *name);

bool var_exists(char *var_name, namespace *ns);

function_node *function_node_new(char *type, char *name);

#endif //ABSTRACT_SYNTAX_TREE_NODE_H
