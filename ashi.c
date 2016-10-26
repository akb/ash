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
#include "parser.h"
#include "result.h"
#include "eval.h"

void print_startup_message(void);

int main(int argc, char** argv) {
  print_startup_message();

  while (1) {
    char* input = readline("ash: ");
    add_history(input);

    AshParser* parser = new__AshParser();

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, parser->ash, &r)) {
      ResultValue* result = ResultValue__eval(FromAST__ResultValue(r.output));
      ResultValue__println(result);
      ResultValue__free(result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    AshParser__free(parser);
    free(input);
  }

  return 0;
}

void print_startup_message() {
  puts("ashi: interactive ash v0.0.1");
  puts("Press Ctrl+C to exit.\n");
}
