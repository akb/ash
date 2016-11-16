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

typedef struct ParserStruct {
  mpc_parser_t* decimal;
  mpc_parser_t* integer;
  mpc_parser_t* number;
  mpc_parser_t* symbol;
  mpc_parser_t* s_expression;
  mpc_parser_t* q_expression;
  mpc_parser_t* expression;
  mpc_parser_t* ash;
} Parser;

Parser* new_parser(void);
void parser_delete(Parser* parser);
Node* new_node_from_ast(mpc_ast_t* t);
#endif
