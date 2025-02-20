#include <stdio.h>
#include <stdlib.h>

#include "pattern.h"

#include <errno.h>
#include <string.h>

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

bool next_token(char *start, regmatch_t *match) {
    return regexec(&token_regex, start, 1, match, 0) == 0;
}

bool full_match(regex_t *regex, char *str) {
    return regexec(regex, str, 0, NULL, 0) == 0;
}

bool valid_symbol(char *symbol) {
    return full_match(&symbol_regex, symbol);
}

bool valid_i64_literal(char *literal) {
    errno = 0;
    char *endptr;
    strtol(literal, &endptr, 0);
    return errno == 0 && *endptr == '\0';
}

bool valid_string_literal(char *literal) {
    size_t len = strlen(literal);
    return len > 1 && literal[0] == '"' && literal[len - 1] == '"';
}
