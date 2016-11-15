#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>

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

static bool flag_silent = false;

void ash_print_startup_message(void);
void ash_parse_arguments(int, char**);

int main(int argc, char** argv) {
  ash_parse_arguments(argc, argv);
  if (flag_silent == false) {
    ash_print_startup_message();
  }

  Environment* e = new_environment();
  environment_add_builtins(e);

  while (1) {
    char* input = readline("ash: ");
    if (input == NULL) break;

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

  environment_delete(e);
  return 0;
}

void ash_parse_arguments(int argc, char** argv) {
  int opt;
  while ((opt = getopt(argc, argv, "s")) != -1) {
    switch (opt) {
      case 's': flag_silent = true; break;
      default: goto parse_arguments_done;
    }
  }
parse_arguments_done:
  return;
}

void ash_print_startup_message() {
  puts("ash: application shell v0.0.1");
  puts("Press Ctrl+C to exit.\n");
}
