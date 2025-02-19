#include "line_iterator.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define SPACE ' '
#define TAB '\t'
#define TAB_SHIFT 2
#define TAB_WIDTH (1 << TAB_SHIFT)

void init_line_iterator(line_iterator *iter, vec tokenv) {
    iter->curr_line.end = 0;
    iter->curr_line.line_num = 0;
    iter->tokenv = tokenv;
}

int get_indent_level(char *indent_token) {
    int count = 0;
    for (char *curr = indent_token; *curr != '\0'; curr++) {
        switch (*curr) {
            case TAB:
                count += TAB_WIDTH;
            break;
            case SPACE:
                count++;
            break;
            default:
                break;
        }
    }

    return count & TAB_WIDTH - 1 ? SUSPICIOUS_INDENT : count >> TAB_SHIFT;
}

line *next_line(line_iterator *iter) {
    vec tokenv = iter->tokenv;
    line *curr_line = &iter->curr_line;

    curr_line->line_num++;
    int indent = get_indent_level(vec_get(tokenv, curr_line->end));
    if (indent == SUSPICIOUS_INDENT) {
        raise_compiler_error("Indent size not a multiple of %d\n", curr_line->line_num, TAB_WIDTH);
    }

    curr_line->indent = indent;
    curr_line->start = ++curr_line->end;

    while (curr_line->end < vec_len(tokenv)) {
        char *token = vec_get(tokenv, curr_line->end);
        if (*token == '\n') {
            return curr_line;
        }
        curr_line->end++;
    }

    return NULL;
}
