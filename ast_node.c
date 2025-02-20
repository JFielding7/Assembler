#include "ast_node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembly_generator.h"
#include "pattern.h"
#include "util.h"

#define NUM_BINARY_OPERATORS 2

void function_print(ast_node *node, size_t level);
void var_print(ast_node *node, size_t _);
void literal_print(ast_node *node, size_t _);
void binary_operation_print(ast_node *node, size_t level);
void ast_node_print(ast_node *node, size_t level);

ast_node *ast_node_new(type *expr_type, void *node, void (*generate_assembly)(void*),
    void (*free_func)(ast_node*), void (*print)(ast_node*, size_t)) {

    ast_node *n = malloc(sizeof(ast_node));
    n->expr_type = expr_type;
    n->node = node;
    n->generate_assembly = generate_assembly;
    n->free_func = free_func;
    n->print = print;
    return n;
}

void var_node_free(ast_node *node) { free(node); }

/**
 * Creates a new AST node for a variable
 * @param var_type Data type of the variable
 * @param var_name Name of the variable
 * @return ast_node*: AST node for the variable
 */
ast_node *var_node_new(type *var_type, char *var_name) {
    // TODO: free function
    return ast_node_new(var_type, var_name, &load_assembly, &var_node_free, &var_print);
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

void function_node_free(ast_node *node) {
    function_node *func_node = node->node;
    free(node);

    vec_iter(ast_node *var_node, func_node->func_namespace.vars, var_node->free_func(var_node))
    vec_free(func_node->func_namespace.vars);
    vec_iter(ast_node *statement, func_node->statements, statement->free_func(statement))
    vec_free(func_node->statements);

    free(func_node);
}

ast_node *function_node_new(type *ret_type, char *name) {
    function_node *func_node = malloc(sizeof(function_node));
    func_node->name = name;
    func_node->param_count = 0;
    func_node->statements = vec_new();
    init_namespace(&func_node->func_namespace);
    return ast_node_new(ret_type, func_node, &function_assembly, &function_node_free, &function_print);
}

void function_node_add_var(function_node *func_node, ast_node *var_node) {
    vec_push(func_node->func_namespace.vars, var_node);
}

ast_node *literal_node_new(type *literal_type, char *value) {
    // TODO: free
    return ast_node_new(literal_type, value, &literal_assembly, NULL, &literal_print);
}

ast_node *binary_operation_new(type *operation_type, ast_node *left, ast_node *right, void (*generate_assembly)(void*)) {
    binary_operation_node *node = malloc(sizeof(binary_operation_node));
    node->left = left;
    node->right = right;
    // TODO: free
    return ast_node_new(operation_type, node, generate_assembly, NULL, &binary_operation_print);
}

void function_print(ast_node *node, size_t level) {
    function_node *func_node = node->node;
    printf("fn %s(", func_node->name);

    vec vars = func_node->func_namespace.vars;
    for (size_t i = 0; i < func_node->param_count; i++) {
        ast_node *param = vec_get(vars, i);
        if (i != 0) {
            printf(", ");
        }
        printf("%s %s", (char*) param->expr_type->name, (char*) param->node);
    }
    printf(") -> %s\n", node->expr_type->name);

    vec_iter(ast_node *statement, func_node->statements, ast_node_print(statement, level + 1))
}

void var_print(ast_node *node, size_t _) {
    printf("%s %s (%lu bytes)\n", node->expr_type->name, (char*) node->node, node->expr_type->size);
}

void literal_print(ast_node *node, size_t _) {
    puts(node->node);
}

void binary_operation_print(ast_node *node, size_t level) {
    void *assembly_functions[NUM_BINARY_OPERATORS << 1] = {
        &assignment_assembly, "=",
        &add_assembly, "+",
    };

    void (*assembly)(void*) = node->generate_assembly;
    for (size_t i = 0; i < NUM_BINARY_OPERATORS << 1; i+=2) {
        if (assembly == assembly_functions[i]) {
            puts(assembly_functions[i + 1]);
            break;
        }
    }

    binary_operation_node *op_node = node->node;
    ast_node_print(op_node->left, level + 1);
    ast_node_print(op_node->right, level + 1);
}

/**
 * Prints out an abstract syntax node and its children
 * @param node ast node
 * @param level current depth level in the ast tree
 */
void ast_node_print(ast_node *node, size_t level) {
    char *dash = "- ";
    char indent_str[level + strlen(dash) + 1];
    memset(indent_str, ' ', level);
    strcpy(indent_str + level, dash);
    printf("%s", indent_str);

    (*node->print)(node, level);
}

/**
 * Prints out the abstract syntax tree
 * @param node root of AST tree
 */
void ast_tree_print(ast_node *node) {
    ast_node_print(node, 0);
}
