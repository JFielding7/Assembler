#ifndef PATTERN_H
#define PATTERN_H

#include <regex.h>
#include <stdbool.h>

void compile_regexps();

void free_regexps();

bool next_token(char *token, regmatch_t *match);

bool valid_symbol(char *symbol);

#endif //PATTERN_H
