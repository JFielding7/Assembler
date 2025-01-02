#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

#include "abstract_syntax_tree_node.h"
#include "vec.h"

#define OPERATOR_PRECEDENCE { \
    "=", NULL, \
    "+", "-", NULL, \
    "*", "/", NULL  \
}

ast_node *generate_ast(vec *tokenv);

#endif //ABSTRACT_SYNTAX_TREE_H
