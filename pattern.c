#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pattern.h"
#include "regex.h"
#include "vec.h"

#define TOKEN_REGEX "\n[ \t]*|[-+*/%|&~^()=,]|\\w+|\".*?[^\\\\]\""
#define SYMBOL_REGEX "^\\w+$"

static regex_t token_regex;
static regex_t symbol_regex;

void compile_regex(regex_t *regex, const char *pattern) {
    const int ret = regcomp(regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        perror("Failed to compile token regex\n");
        exit(ret);
    }
}

void compile_regexps() {
    compile_regex(&token_regex, TOKEN_REGEX);
    compile_regex(&symbol_regex, SYMBOL_REGEX);
}

void free_regexps() {
    regfree(&token_regex);
    regfree(&symbol_regex);
}

bool next_token(char *token, regmatch_t *match) {
    return regexec(&token_regex, token, 1, match, 0) == 0;
}

bool valid_symbol(char *symbol) {
    return regexec(&symbol_regex, symbol, 0, NULL, 0) == 0;
}
