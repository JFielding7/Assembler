#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

#define TOKEN_REGEX "[-+*/%|&~^()=]|\\w+"

int tokenize(regex_t *regex, regmatch_t* match, char *source_code_cursor, vec *tokenv) {
    while (regexec(regex, source_code_cursor, 1, match, 0) == 0) {
        source_code_cursor[match->rm_eo] = '\0';
        source_code_cursor += match->rm_so;
        vec_add(tokenv, char*, source_code_cursor);
        source_code_cursor += match->rm_eo + 1;
    }

    return 0;
}

/**
 *
 * @param files
 * @return
 */
vec *tokenize_source_code_files(vec *files) {
    regex_t regex;
    regmatch_t match[1];

    const int ret = regcomp(&regex, TOKEN_REGEX, REG_EXTENDED);
    if (ret != 0) {
        perror("Failed to compile token regex\n");
        return NULL;
    }

    vec *tokenv = new_vec(char*);
    for (size_t i = 0; i < files->len; i++) {
        char *source_code = vec_get(files, char*, i);
        tokenize(&regex, match, source_code, tokenv);
    }

    regfree(&regex);

    return tokenv;
}

