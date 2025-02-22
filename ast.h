#ifndef AST_H
#define AST_H

#include "ast_node.h"
#include "vec.h"

ast_node *generate_ast(char *filename, vec tokenv);

#endif //AST_H
