#include <stdlib.h>

#include "mpc.h"
#include "grammar.h"
#include "node.h"
#include "parser.h"

Node* new_node_from_ast_integer(mpc_ast_t* t);
Node* new_node_from_ast_decimal(mpc_ast_t* t);

Parser* new_parser(void) {
  Parser* parser = malloc(sizeof(Parser));
  parser->decimal      = mpc_new(PARSER_TAG_DECIMAL);
  parser->integer      = mpc_new(PARSER_TAG_INTEGER);
  parser->number       = mpc_new(PARSER_TAG_NUMBER);
  parser->symbol       = mpc_new(PARSER_TAG_SYMBOL);
  parser->s_expression = mpc_new(PARSER_TAG_S_EXPRESSION);
  parser->q_expression = mpc_new(PARSER_TAG_Q_EXPRESSION);
  parser->expression   = mpc_new(PARSER_TAG_EXPRESSION);
  parser->ash          = mpc_new(PARSER_TAG_ASH);

  mpca_lang(MPCA_LANG_DEFAULT, ASH_GRAMMAR,
    parser->decimal, parser->integer, parser->number,
    parser->symbol, parser->s_expression, parser->q_expression,
    parser->expression, parser->ash
  );

  return parser;
}

void parser_delete(Parser* parser) {
  mpc_cleanup(8,
    parser->decimal, parser->integer, parser->number,
    parser->symbol, parser->s_expression, parser->q_expression,
    parser->expression, parser->ash);
  free(parser);
}


Node* new_node_from_ast_integer(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE
    ? new_node_integer(x)
    : new_node_error("Invalid integer number.");
}

Node* new_node_from_ast_decimal(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  double x = strtod(t->contents, NULL);
  return errno != ERANGE
    ? new_node_decimal(x)
    : new_node_error("Invalid decimal number.");
}

Node* new_node_from_ast(mpc_ast_t* t) { // lval_read
  if (strstr(t->tag, PARSER_TAG_INTEGER))
    return new_node_from_ast_integer(t);
  else if (strstr(t->tag, PARSER_TAG_DECIMAL))
    return new_node_from_ast_decimal(t);
  else if (strstr(t->tag, PARSER_TAG_SYMBOL))
    return new_node_symbol(t->contents);

  Node* x = NULL;
  if (strcmp(t->tag, PARSER_TAG_ROOT) == 0)
    x = new_node_s_expression();
  else if (strstr(t->tag, PARSER_TAG_Q_EXPRESSION))
    x = new_node_q_expression();
  else if (strstr(t->tag, PARSER_TAG_S_EXPRESSION))
    x = new_node_s_expression();

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) continue;
    if (strcmp(t->children[i]->contents, ")") == 0) continue;
    if (strcmp(t->children[i]->contents, "{") == 0) continue;
    if (strcmp(t->children[i]->contents, "}") == 0) continue;
    if (strcmp(t->children[i]->tag,  "regex") == 0) continue;
    x = node_add(x, new_node_from_ast(t->children[i]));
  }

  return x;
}
