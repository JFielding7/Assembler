#ifndef LINE_ITERATOR_H
#define LINE_ITERATOR_H

#include "vec.h"

#define SUSPICIOUS_INDENT -1

typedef struct line_s {
    size_t line_num;
    size_t start;
    size_t end;
    size_t indent;
} line;

typedef struct line_iterator_s {
    vec *tokenv;
    line curr_line;
} line_iterator;

void init_line_iterator(line_iterator *iter, vec *tokenv);

line *next_line(line_iterator *iter);

#endif //LINE_ITERATOR_H
