#include <stdlib.h>

#include "mpc.h"
#include "grammar.h"
#include "node.h"
#include "parser.h"

AshParser* new__AshParser(void) {
  AshParser* parser = malloc(sizeof(AshParser));
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

void AshParser__free(AshParser* parser) {
  mpc_cleanup(8,
    parser->decimal, parser->integer, parser->number,
    parser->symbol, parser->s_expression, parser->q_expression,
    parser->expression, parser->ash);
  free(parser);
}


Node* FromIntegerAST__Node(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE
    ? new__IntegerNode(x)
    : new__ErrorNode("Invalid integer number.");
}

Node* FromDecimalAST__Node(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  double x = strtod(t->contents, NULL);
  return errno != ERANGE
    ? new__DecimalNode(x)
    : new__ErrorNode("Invalid decimal number.");
}

Node* FromAST__Node(mpc_ast_t* t) { // lval_read
  if (strstr(t->tag, PARSER_TAG_INTEGER))
    return FromIntegerAST__Node(t);
  else if (strstr(t->tag, PARSER_TAG_DECIMAL))
    return FromDecimalAST__Node(t);
  else if (strstr(t->tag, PARSER_TAG_SYMBOL))
    return new__SymbolNode(t->contents);

  Node* x = NULL;
  if (strcmp(t->tag, PARSER_TAG_ROOT) == 0)
    x = new__SExpressionNode();
  else if (strstr(t->tag, PARSER_TAG_Q_EXPRESSION))
    x = new__QExpressionNode();
  else if (strstr(t->tag, PARSER_TAG_S_EXPRESSION))
    x = new__SExpressionNode();

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) continue;
    if (strcmp(t->children[i]->contents, ")") == 0) continue;
    if (strcmp(t->children[i]->contents, "{") == 0) continue;
    if (strcmp(t->children[i]->contents, "}") == 0) continue;
    if (strcmp(t->children[i]->tag,  "regex") == 0) continue;
    x = Node__add(x, FromAST__Node(t->children[i]));
  }

  return x;
}
