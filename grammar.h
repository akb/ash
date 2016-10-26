#ifndef ASH_GRAMMAR_DEFINED
#define ASH_GRAMMAR_DEFINED
static char* ASH_GRAMMAR =
  " decimal    : /-?[0-9]*\\.[0-9]+/ ;"
  " integer    : /-?[0-9]+/ ;"
  " number     : <decimal> | <integer> ; "
  " operator   : '+' | \"add\"      |"
  "              '-' | \"subtract\" |"
  "              '*' | \"multiply\" |"
  "              '/' | \"divide\"   |"
  "              '%' | \"modulo\"   ;"
  " expression : <number> | '(' <operator> <expression>+ ')' ;"
  " ash        : /^/ <operator> <expression>+ /$/ ;";
#endif
