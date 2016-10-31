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
#include "environment.h"
#include "node.h"
#include "parser.h"

void print_startup_message(void);

int main(int argc, char** argv) {
  print_startup_message();
  Environment* e = new__Environment();
  Environment__add_builtins(e);

  while (1) {
    char* input = readline("ash: ");
    add_history(input);

    AshParser* parser = new__AshParser();

    mpc_result_t r;
    if (mpc_parse("<stdin>", input, parser->ash, &r)) {
      //mpc_ast_print(r.output); // uncomment to display AST
      Node* result = Node__evaluate(e, FromAST__Node(r.output));
      Node__println(result);
      Node__free(result);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    AshParser__free(parser);
    free(input);
  }

  Environment__free(e);
  return 0;
}

void print_startup_message() {
  puts("ashi: interactive ash v0.0.1");
  puts("Press Ctrl+C to exit.\n");
}
