#ifndef ASH_PARSER_DEFINED
#define ASH_PARSER_DEFINED
#include "mpc.h"
#include "node.h"

#define PARSER_TAG_ROOT         ">"
#define PARSER_TAG_DECIMAL      "decimal"
#define PARSER_TAG_INTEGER      "integer"
#define PARSER_TAG_NUMBER       "number"
#define PARSER_TAG_SYMBOL       "symbol"
#define PARSER_TAG_S_EXPRESSION "s_expression"
#define PARSER_TAG_Q_EXPRESSION "q_expression"
#define PARSER_TAG_EXPRESSION   "expression"
#define PARSER_TAG_ASH          "ash"

typedef struct AshParserStruct {
  mpc_parser_t* decimal;
  mpc_parser_t* integer;
  mpc_parser_t* number;
  mpc_parser_t* symbol;
  mpc_parser_t* s_expression;
  mpc_parser_t* q_expression;
  mpc_parser_t* expression;
  mpc_parser_t* ash;
} AshParser;

AshParser* new__AshParser(void);
void AshParser__free(AshParser* parser);
Node* FromIntegerAST__Node(mpc_ast_t* t);
Node* FromDecimalAST__Node(mpc_ast_t* t);
Node* FromSymbolAST__ResultVaule(mpc_ast_t* t);
Node* FromSExpression__Node(mpc_ast_t* t);
Node* FromAST__Node(mpc_ast_t* t);
Node* Node__add(Node* v, Node* x);
#endif
