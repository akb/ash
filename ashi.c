#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include "editline.h"
#else
#include <editline/readline.h>
#ifndef __APPLE__
  #include <editline/history.h>
  #endif
#endif


#include "mpc.h"
#include "result.h"
#include "eval.h"
#include "grammar.h"

void print_startup_message(void);

int main(int argc, char** argv) {
  print_startup_message();

  mpc_parser_t* Decimal    = mpc_new("decimal");
  mpc_parser_t* Integer    = mpc_new("integer");
  mpc_parser_t* Number     = mpc_new("number");
  mpc_parser_t* Operator   = mpc_new("operator");
  mpc_parser_t* Expression = mpc_new("expression");
  mpc_parser_t* Ash        = mpc_new("ash");

  mpca_lang(MPCA_LANG_DEFAULT, ASH_GRAMMAR,
    Decimal, Integer, Number, Operator, Expression, Ash);

  while (1) {
    char* input = readline("ash: ");
    add_history(input);

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Ash, &r)) {
      //mpc_ast_print(r.output); // Uncomment to print unevaluated AST
      ResultValue result = eval(r.output);
      ResultValue__println(result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }

  mpc_cleanup(4, Integer, Decimal, Operator, Expression, Ash);
  return 0;
}

void print_startup_message() {
  puts("ashi: interactive ash v0.0.1");
  puts("Press Ctrl+C to exit.\n");
}
