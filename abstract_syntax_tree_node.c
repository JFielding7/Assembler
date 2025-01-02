#include "abstract_syntax_tree_node.h"

#include <stdlib.h>

var *var_new(char *type, char *name) {
    var *v = malloc(sizeof(var));
    v->type = type;
    v->name = name;
    return v;
}


