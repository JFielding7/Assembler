#include <regex.h>
#include <stdio.h>
#include "assembler.h"
#define TOKEN_REGEX "[-+*/%|&~^()=]|\\w+"

char *next_token() {

}

char **tokenize(char *source_code) {
    regex_t regex;
    regmatch_t match[1];

    int ret = regcomp(&regex, TOKEN_REGEX, REG_EXTENDED);
    if (ret != 0) {
        perror("Failed to compile token regex\n");
        return NULL;
    }

    char *token = source_code;
    while (regexec(&regex, token, 1, match, 0) == 0) {
        printf("Token: %s\n", token);
        token += match->rm_eo;
    }

    return NULL;
}
