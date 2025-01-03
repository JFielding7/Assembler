#include "abstract_syntax_tree_node.h"

#include <stdlib.h>
#include <string.h>

var *var_new(char *type, char *name) {
    var *v = malloc(sizeof(var));
    v->type = type;
    v->name = name;
    return v;
}

bool var_exists(char *var_name, namespace *ns) {
    vec_iter(char *var, ns->vars, {
        if (strcmp(var_name, var) == 0) {
            return true;
        }
    })
    vec *v = ns->vars;
    for (size_t i = 0; i < v->len; i++) {
        if (strcmp(var_name, vec_get(ns->vars, i)) == 0) {
            return true;
        }
    }

    return ns->parent == NULL ? false : var_exists(var_name, ns->parent);
}

void init_namespace(namespace *ns) {
    ns->vars = vec_new();
    ns->parent = NULL;
}

function_node *function_node_new(char *type, char *name) {
    function_node *func_node = malloc(sizeof(function_node));
    func_node->type = type;
    func_node->name = name;
    init_namespace(&func_node->ns);
    return func_node;
}
