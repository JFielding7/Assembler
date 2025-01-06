#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_node.h"
#include "pattern.h"
#include "types.h"

#define ERROR_LINE_MESSAGE "ERROR: line %lu: "

void assert_valid_type(char *type, line *curr_line) {
    if (!valid_type(type))
        raise_compiler_error("Invalid type `%s`", curr_line->line_num, type);
}

void assert_valid_symbol(char *symbol, line *curr_line) {
    if (!valid_symbol(symbol))
        raise_compiler_error("Invalid symbol `%s`", curr_line->line_num, symbol);
}

void assert_token_equals(char *token, char *s, line *curr_line) {
    if (strcmp(token, s) != 0)
        raise_compiler_error("Expected `%s`", curr_line->line_num);
}

void assert_complete_definition(int min_tokens, int line_offset, line *curr_line) {
    if (curr_line->end - line_offset < min_tokens)
        raise_compiler_error("Incomplete defintion", curr_line->line_num);
}

void assert_unique_var(char *var_name, namespace *ns, line *curr_line) {
    if (var_lookup(ns, var_name))
        raise_compiler_error("`%s` is already defined", curr_line->line_num, var_name);
}

void raise_compiler_error(char *message, ...) {
    va_list args;
    va_start(args, message);

    size_t base_len = strlen(ERROR_LINE_MESSAGE);
    size_t message_len = strlen(message);

    char error_message[base_len + message_len + 2];
    strncpy(error_message, ERROR_LINE_MESSAGE, base_len);
    strncpy(error_message + base_len, message, message_len);
    strcpy(error_message + base_len + message_len, "\n");

    vfprintf(stderr, error_message, args);
    va_end(args);

    exit(1);
}
