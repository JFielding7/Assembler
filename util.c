#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_node.h"
#include "pattern.h"
#include "types.h"

#define ERROR_LINE_MESSAGE "ERROR: %s: Line %lu: "

void assert_valid_type(char *type_name, type *expr_type, line *curr_line) {
    if (expr_type == NULL)
        raise_compiler_error("Invalid type `%s`", curr_line, type_name);
}

void assert_valid_symbol(char *symbol, line *curr_line) {
    if (!valid_symbol(symbol))
        raise_compiler_error("Invalid symbol `%s`", curr_line, symbol);
}

void assert_token_equals(char *token, char *expected_token, line *curr_line) {
    if (strcmp(token, expected_token) != 0)
        raise_compiler_error("Expected `%s`", curr_line, expected_token);
}

void assert_has_min_tokens(int min_tokens, int line_offset, line *curr_line) {
    if (curr_line->end - line_offset < min_tokens)
        raise_compiler_error("Incomplete defintion", curr_line);
}

void assert_unique_var(char *var_name, namespace *ns, line *curr_line) {
    if (var_lookup(ns, var_name))
        raise_compiler_error("`%s` is already defined", curr_line, var_name);
}

/**
 * Raises a fatal compiler error, displays info about the error
 * @param message Message related to the type of error
 * @param error_line Line the error occured on
 * @param ... Format arguments
 */
void raise_compiler_error(char *message, line *error_line, ...) {
    va_list args;
    va_start(args, error_line);

    fprintf(stderr, ERROR_LINE_MESSAGE, error_line->filename, error_line->line_num);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(1);
}
