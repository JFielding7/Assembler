#ifndef ABSTRACT_SYNTAX_TREE_H
#define ABSTRACT_SYNTAX_TREE_H

typedef struct ast_s {
    unsigned char node_type;
    ast *left;
    ast *right;
}; ast

#endif //ABSTRACT_SYNTAX_TREE_H
