#include "abstract_syntax_tree.h"

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec *tokenv) {
    for (size_t i = 0; i < tokenv->len; i++) {
        char *token = vec_get(tokenv, char*, i);

    }
}
