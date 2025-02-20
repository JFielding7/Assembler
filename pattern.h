#ifndef PATTERN_H
#define PATTERN_H

#include <regex.h>
#include <stdbool.h>

void compile_regexps();

void free_regexps();

bool next_token(char *start, regmatch_t *match);

bool valid_symbol(char *symbol);

bool valid_i64_literal(char *literal);

bool valid_string_literal(char *literal);

#endif //PATTERN_H
