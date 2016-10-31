#ifndef ASH_GRAMMAR_DEFINED
#define ASH_GRAMMAR_DEFINED
static char* ASH_GRAMMAR =
  "decimal : /-?[0-9]*\\.[0-9]+/ ;"
  "integer : /-?[0-9]+/ ;"
  "number : <decimal> | <integer> ;"
  "symbol : /[A-Za-z0-9_+\\-*\\/\\\\=!&]+/ ;"
  "s_expression : '(' <expression>* ')' ;"
  "q_expression : '{' <expression>* '}' ;"
  "expression : <number> | <symbol> | <s_expression> | <q_expression> ;"
  "ash        : /^/ <expression>* /$/ ;";
#endif

//  "symbol : '+' | \"add\"      |"
//  "         '-' | \"subtract\" |"
//  "         '*' | \"multiply\" |"
//  "         '/' | \"divide\"   |"
//  "         '%' | \"modulo\"   |"
//  "         \"list\" | \"head\" | \"tail\" | \"join\" | \"evaluate\" | "
//  "         \"initial\" | \"length\" | \"construct\";"
