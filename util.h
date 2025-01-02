#ifndef UTIL_H
#define UTIL_H
#include "line_iterator.h"

void assert_valid_type(char *type, line *curr_line);

void assert_valid_symbol(char *symbol, line *curr_line);

void assert_token_equals(char *token, char *s, line *curr_line);

void raise_compiler_error(char *message, ...);

#endif //UTIL_H
