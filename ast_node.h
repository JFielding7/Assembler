#ifndef AST_NODE_H
#define AST_NODE_H

#include "types.h"
#include "vec.h"

typedef struct ast_node_s {
    type *expr_type;
    void *node;
    void (*generate_assembly)(struct ast_node_s*);
    void (*free_func)(struct ast_node_s*);
    void (*print)(struct ast_node_s*, size_t);
} ast_node;

typedef struct binary_operation_s {
    ast_node *left;
    ast_node *right;
} binary_operation_node;

typedef struct assignment_s {
    ast_node *var;
    ast_node *value;
} assignment_node;

typedef struct namespace_s {
    vec vars;
    struct namespace_s *parent;
} namespace;

typedef struct function_s {
    char *name;
    size_t param_count;
    vec statements;
    namespace func_namespace;
} function_node;

ast_node *var_node_new(type *var_type, char *var_name);

ast_node *var_lookup(namespace *ns, char *name);

ast_node *function_node_new(type *ret_type, char *name);

void function_node_add_var(function_node *func_node, ast_node *var_node);

ast_node *assignment_node_new(ast_node *var, ast_node *value);

ast_node *literal_node_new(type *literal_type, char *value);

ast_node *binary_operation_new(type *operation_type, ast_node *left, ast_node *right, void (*generate_assembly)(ast_node*));

void ast_tree_print(ast_node *node);

#endif //AST_NODE_H
