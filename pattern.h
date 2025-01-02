#ifndef PATTERN_H
#define PATTERN_H

#include <regex.h>
#include <stdbool.h>

void compile_regexps();

void free_regexps();

bool next_token(char *token, regmatch_t *match);

bool valid_symbol(char *symbol);

bool valid_type(char *type);

#endif //PATTERN_H
