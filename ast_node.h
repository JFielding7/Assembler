#ifndef AST_NODE_H
#define AST_NODE_H

#include "types.h"
#include "vec.h"

typedef struct ast_node_s {
    void *node;
    void (*generate_assembly)(void*);
} ast_node;

typedef struct binary_operation_s {
    void (*operation)(ast_node*, ast_node*);
    ast_node *left;
    ast_node *right;
} binary_operation;

typedef struct literal_s {
    type *literal_type;
    char *value;
} literal_node;

typedef struct var_s {
    type *var_type;
    char *name;
} var_node;

typedef struct assignment_s {
    var_node *var;
    ast_node *value;
} assignment_node;

typedef struct namespace_s {
    vec *vars;
    struct namespace_s *parent;
} namespace;

typedef struct function_s {
    type *return_type;
    char *name;
    vec *statements;
    namespace ns;
} function_node;

ast_node *ast_node_new(void *node, void (*generate_assembly)(void*));

var_node *var_new(char*, char*);

var_node *var_lookup(namespace *ns, char *name);

function_node *function_def_node_new(char *type, char *name);

assignment_node *assignment_node_new(var_node *variable, ast_node *value);

literal_node *literal_node_new(type *literal_type, char *value);

#endif //AST_NODE_H
