#ifndef ASH_PARSER_DEFINED
#define ASH_PARSER_DEFINED
#include "mpc.h"
#include "result.h"

#define PARSER_TAG_ROOT         ">"
#define PARSER_TAG_DECIMAL      "decimal"
#define PARSER_TAG_INTEGER      "integer"
#define PARSER_TAG_NUMBER       "number"
#define PARSER_TAG_SYMBOL       "symbol"
#define PARSER_TAG_S_EXPRESSION "sexpression"
#define PARSER_TAG_EXPRESSION   "expression"
#define PARSER_TAG_ASH          "ash"

typedef struct AshParserStruct {
  mpc_parser_t* decimal;
  mpc_parser_t* integer;
  mpc_parser_t* number;
  mpc_parser_t* symbol;
  mpc_parser_t* s_expression;
  mpc_parser_t* expression;
  mpc_parser_t* ash;
} AshParser;

AshParser* new__AshParser(void);
void AshParser__free(AshParser* parser);
ResultValue* FromIntegerAST__ResultValue(mpc_ast_t* t);
ResultValue* FromDecimalAST__ResultValue(mpc_ast_t* t);
ResultValue* FromSymbolAST__ResultVaule(mpc_ast_t* t);
ResultValue* FromSExpression__ResultValue(mpc_ast_t* t);
ResultValue* FromAST__ResultValue(mpc_ast_t* t);
ResultValue* ResultValue__add(ResultValue* v, ResultValue* x);
#endif
