#ifndef LINE_ITERATOR_H
#define LINE_ITERATOR_H

#include "vec.h"

typedef struct line_s {
    size_t line_num;
    size_t start;
    size_t end;
    size_t indent;
    char *filename;
} line;

typedef struct line_iterator_s {
    vec tokenv;
    line curr_line;
} line_iterator;

void init_line_iterator(line_iterator *iter, char *filename, vec tokenv);

line *next_line(line_iterator *iter);

#endif //LINE_ITERATOR_H
