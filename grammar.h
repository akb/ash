#ifndef ASH_GRAMMAR_DEFINED
#define ASH_GRAMMAR_DEFINED
static char* ASH_GRAMMAR =
  "decimal : /-?[0-9]*\\.[0-9]+/ ;"
  "integer : /-?[0-9]+/ ;"
  "number : <decimal> | <integer> ;"
  "symbol : '+' | \"add\"      |"
  "         '-' | \"subtract\" |"
  "         '*' | \"multiply\" |"
  "         '/' | \"divide\"   |"
  "         '%' | \"modulo\"   ;"
  "sexpression : '(' <expression>* ')' ;"
  "expression : <number> | <symbol> | <sexpression> ;"
  "ash        : /^/ <expression>* /$/ ;";
#endif
