#include "ast_node.h"

#include <stdlib.h>
#include <string.h>

#include "assembly_generator.h"
#include "pattern.h"
#include "util.h"

ast_node *ast_node_new(type *expr_type, void *node, void (*generate_assembly)(void*)) {
    ast_node *n = malloc(sizeof(ast_node));
    n->expr_type = expr_type;
    n->node = node;
    n->generate_assembly = generate_assembly;
    return n;
}

// TODO: fix types
ast_node *var_node_new(type *var_type, char *var_name) {
    return ast_node_new(var_type, var_name, &load_assembly);
}

void init_namespace(namespace *ns) {
    ns->vars = vec_new();
    ns->parent = NULL;
}

ast_node *var_lookup(namespace *ns, char *name) {
    while (ns != NULL) {
        vec_iter(ast_node *curr_var, ns->vars, {
            if (strcmp(name, curr_var->node) == 0) {
                return curr_var;
            }
        })

        ns = ns->parent;
    }

    return NULL;
}

ast_node *function_def_node_new(type *ret_type, char *name) {
    function_node *func_node = malloc(sizeof(function_node));
    func_node->name = name;
    init_namespace(&func_node->func_namespace);
    return ast_node_new(ret_type, func_node, &function_assembly);
}

ast_node *assignment_node_new(ast_node *var, ast_node *value) {
    assignment_node *assignment_node = malloc(sizeof(assignment_node));
    assignment_node->var = var;
    assignment_node->value = value;
    return ast_node_new(var->expr_type, assignment_node, &assignment_assembly);
}

ast_node *literal_node_new(type *literal_type, char *value) {
    return ast_node_new(literal_type, value, &literal_assembly);
}

ast_node *binary_operation_new(type *operation_type, ast_node *left, ast_node *right, void (*generate_assembly)(void*)) {
    binary_operation *node = malloc(sizeof(binary_operation));
    node->operation_type = operation_type;
    node->left = left;
    node->right = right;
    // TODO: Assembly function
    return ast_node_new(operation_type, node, generate_assembly);
}
