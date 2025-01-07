#ifndef EXPRESSION_H
#define EXPRESSION_H

#define ASSIGNMENT "="
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"
#define MOD "%"

#define PAREN_OPEN "("
#define PAREN_CLOSE ")"

typedef struct expression_parser_s {
  vec *tokenv;
  char *token;
  size_t token_index;
  size_t expr_start;
  size_t start;
  size_t end;
  size_t op_group_index;
  size_t line_num;
  size_t *paren_matches;
  namespace *ns;
} expression_parser;

static ast_node *assignment(expression_parser *parser);

static ast_node *binary_operator(expression_parser *parser);

static ast_node *parse_sub_expression(expression_parser *parser);

ast_node *parse_expression(vec *tokenv, size_t start, size_t end, size_t line_num, namespace *ns);

#endif //EXPRESSION_H
