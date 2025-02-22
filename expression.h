#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "line_iterator.h"

ast_node *parse_expression(vec tokenv, line *curr_line, size_t start, size_t end, namespace *ns);

#endif //EXPRESSION_H
