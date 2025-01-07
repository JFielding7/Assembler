#include "ast_node.h"

#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "util.h"

ast_node *ast_node_new(void *node, void (*generate_assembly)(void*)) {
    ast_node *n = malloc(sizeof(ast_node));
    n->node = node;
    n->generate_assembly = generate_assembly;
    return n;
}

var_node *var_new(char *type, char *name) {
    var_node *var = malloc(sizeof(var_node));
    var->name = name;
    return var;
}

void init_namespace(namespace *ns) {
    ns->vars = vec_new();
    ns->parent = NULL;
}

var_node *var_lookup(namespace *ns, char *name) {
    vec_iter(var_node *curr_var, ns->vars, {
        if (strcmp(name, curr_var->name) == 0) {
            return curr_var;
        }
    })

    return ns->parent == NULL ? NULL : var_lookup(ns->parent, name);
}

function_node *function_def_node_new(char *type, char *name) {
    function_node *func_node = malloc(sizeof(function_node));
    func_node->name = name;
    init_namespace(&func_node->ns);
    return func_node;
}

void add_func_statement(ast_node *func, ast_node *statement) {
    function_node *func_node = func->node;
    vec_push(func_node->statements, statement);
}

assignment_node *assignment_node_new(var_node *variable, ast_node *value) {
    assignment_node *assignment_node = malloc(sizeof(assignment_node));
    assignment_node->var = variable;
    assignment_node->value = value;
    return assignment_node;
}

literal_node *literal_node_new(type *literal_type, char *value) {
    literal_node *node = malloc(sizeof(literal_node));
    node->literal_type = literal_type;
    node->value = value;
    return node;
}
