#include <stdlib.h>

#include "mpc.h"
#include "grammar.h"
#include "result.h"
#include "parser.h"

AshParser* new__AshParser(void) {
  AshParser* parser = malloc(sizeof(AshParser));
  parser->decimal      = mpc_new(PARSER_TAG_DECIMAL);
  parser->integer      = mpc_new(PARSER_TAG_INTEGER);
  parser->number       = mpc_new(PARSER_TAG_NUMBER);
  parser->symbol       = mpc_new(PARSER_TAG_SYMBOL);
  parser->s_expression = mpc_new(PARSER_TAG_S_EXPRESSION);
  parser->expression   = mpc_new(PARSER_TAG_EXPRESSION);
  parser->ash          = mpc_new(PARSER_TAG_ASH);

  mpca_lang(MPCA_LANG_DEFAULT, ASH_GRAMMAR,
    parser->decimal, parser->integer, parser->number,
    parser->symbol, parser->s_expression, parser->expression,
    parser->ash
  );

  return parser;
}

void AshParser__free(AshParser* parser) {
  mpc_cleanup(7,
    parser->decimal, parser->integer, parser->number,
    parser->symbol, parser->s_expression, parser->expression,
    parser->ash);
  free(parser);
}


ResultValue* FromIntegerAST__ResultValue(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE
    ? new__IntegerResultValue(x)
    : new__ErrorResultValue("Invalid integer number.");
}

ResultValue* FromDecimalAST__ResultValue(mpc_ast_t* t) { // lval_read_number
  errno = 0;
  double x = strtod(t->contents, NULL);
  return errno != ERANGE
    ? new__DecimalResultValue(x)
    : new__ErrorResultValue("Invalid decimal number.");
}

ResultValue* FromAST__ResultValue(mpc_ast_t* t) { // lval_read
  if (strstr(t->tag, PARSER_TAG_INTEGER))
    return FromIntegerAST__ResultValue(t);
  if (strstr(t->tag, PARSER_TAG_DECIMAL))
    return FromDecimalAST__ResultValue(t);
  if (strstr(t->tag, PARSER_TAG_SYMBOL))
    return new__SymbolResultValue(t->contents);

  ResultValue* x = NULL;
  if (strcmp(t->tag, PARSER_TAG_ROOT) == 0)
    x = new__SExpressionResultValue();
  if (strstr(t->tag, PARSER_TAG_S_EXPRESSION))
    x = new__SExpressionResultValue();

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) continue;
    if (strcmp(t->children[i]->contents, ")") == 0) continue;
    if (strcmp(t->children[i]->contents, "{") == 0) continue;
    if (strcmp(t->children[i]->contents, "}") == 0) continue;
    if (strcmp(t->children[i]->tag, "regex") == 0) continue;
    x = ResultValue__add(x, FromAST__ResultValue(t->children[i]));
  }

  return x;
}

ResultValue* ResultValue__add(ResultValue* v, ResultValue* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(ResultValue*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}
