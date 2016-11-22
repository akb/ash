#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>

#ifdef _WIN32
#include "editline.h"
#else
#include <editline/readline.h>
#endif

#include "mpc.h"
#include "debug.h"
#include "environment.h"
#include "node.h"
#include "parser.h"

static bool flag_silent = false;

void ash_print_startup_message(void);
void ash_parse_arguments(int, char**);

int main(int argc, char** argv) {
  ash_parse_arguments(argc, argv);
  if (flag_silent == false)
    ash_print_startup_message();

  Environment* e = new_environment();
  environment_add_builtins(e);

  int exit_code = -1;
  while (exit_code == -1) {
    char* input = readline("ash: ");
    if (input == NULL) break;

    add_history(input);

    Parser* parser = new_parser();

    mpc_result_t r;
    log_debug("main: parsing expression");
    if (mpc_parse("<stdin>", input, parser->ash, &r)) {
      if (flag_debug == true) mpc_ast_print(r.output);
      log_debug("main: evaluating expression");
      Node* result = node_evaluate(e, new_node_from_ast(r.output));
      log_debug("main: displaying result");
      node_println(result);
      if (result->type == NODE_EXIT) exit_code = result->exit_code;
      node_delete(result);
      mpc_ast_delete(r.output);
    } else {
      log_debug("main: expression evaluated to error.");
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    parser_delete(parser);
    free(input);
  }
  log_debug("main: received exit code, exiting");

  environment_delete(e);
  return exit_code;
}

void ash_parse_arguments(int argc, char** argv) {
  int opt;
  while ((opt = getopt(argc, argv, "sd")) != -1) {
    switch (opt) {
      case 's': flag_silent = true; break;
      case 'd': flag_debug  = true; break;
      default: return;
    }
  }
}

void ash_print_startup_message() {
  puts("ash: application shell v0.0.1");
  puts("Press Ctrl+C to exit.\n");
}
