#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#define TOKEN_REGEX "[-+*/%|&~^()=]|\\w+"

char *next_token() {

}

int tokenize(char *source_code, char **tokens_buffer, size_t *length) {
    regex_t regex;
    regmatch_t match[1];

    int ret = regcomp(&regex, TOKEN_REGEX, REG_EXTENDED);
    if (ret != 0) {
        perror("Failed to compile token regex\n");
        return ret;
    }

    char *token = source_code;
    while (regexec(&regex, token, 1, match, 0) == 0) {
        tokens_buffer[*length++] = token;
        token += match->rm_eo;
    }

    return 0;
}
