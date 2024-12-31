#include "abstract_syntax_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#define SUSPICIOUS_INDENT -1
#define SPACE ' '
#define TAB '\t'
#define TAB_SHIFT 2
#define TAB_WIDTH (1 << TAB_SHIFT)

#define FUNCTION_DEFINITION "fn"

function *create_function_node(vec *tokenv, size_t *token_index, size_t line_num) {
    *token_index++;

    char *function_name = vec_get(tokenv, char*, *token_index);
    if (invalid_symbol(function_name)) {
        fprintf(stderr, "ERROR on line %lu: Invalid function name `%s`", line_num, function_name);
    }


}

ast_node *create_ast_node(vec *tokenv, size_t *token_index) {
    char *token = vec_get(tokenv, char*, *token_index);

    if (strcmp(token, FUNCTION_DEFINITION) == 0) {

    }
}

int get_indent_level(char *indent_token) {
    int count = 0;
    for (char *curr = indent_token; *curr != '\0'; curr++) {
        switch (*curr) {
            case TAB:
                count += TAB_WIDTH;
                break;
            case SPACE:
                count++;
                break;
            default:
                break;
        }
    }

    return count & TAB_WIDTH - 1 ? SUSPICIOUS_INDENT : count >> TAB_SHIFT;
}

/**
 * Generate an abstract syntax tree for the source code
 * @param tokenv Vector of tokens in the source code
 * @return ast_node Root of the code's abstarct syntax tree
 */
ast_node *generate_ast(vec *tokenv) {
    // ast_node *

    size_t line_num = 1;
    size_t token_index = 0;
    while (token_index < tokenv->len) {
        char *indent_token = vec_get(tokenv, char*, token_index);

        int indent = get_indent_level(indent_token);
        if (indent == SUSPICIOUS_INDENT) {
            fprintf(stderr, "ERROR on line %lu: Suspicious indent\n", line_num);
            exit(-1);
        }

        token_index++;
        ast_node *node = create_ast_node(tokenv, &token_index);
    }
}
