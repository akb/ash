#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "environment.h"
#include "node.h"
#include "builtins.h"

/**
 * new_environment
 */
Environment* new_environment(void) {
  Environment* e = malloc(sizeof(Environment));
  e->count = 0;
  e->symbols = NULL;
  e->nodes = NULL;
  return e;
}

void environment_delete(Environment* e) {
  for (int i = 0; i < e->count; i++) {
    free(e->symbols[i]);
    Node__free(e->nodes[i]);
  }
  free(e->symbols);
  free(e->nodes);
  free(e);
}

Node* environment_get(Environment* e, Node* k) {
  for (int i = 0; i < e->count; i++)
    if (strcmp(e->symbols[i], k->symbol) == 0)
      return Node__copy(e->nodes[i]);
  return new__ErrorNode("Undefined symbol '%s'", k->symbol);
}

void environment_put(Environment* e, Node* k, Node* v) {
  for (int i = 0; i < e->count; i++) {
    if (strcmp(e->symbols[i], k->symbol) == 0) {
      Node__free(e->nodes[i]);
      e->nodes[i] = Node__copy(v);
      return;
    }
  }

  e->count++;
  e->symbols = realloc(e->symbols, sizeof(char*) * e->count);
  e->nodes   = realloc(e->nodes,   sizeof(Node*) * e->count);

  e->symbols[e->count - 1] = malloc(strlen(k->symbol) + 1);
  strcpy(e->symbols[e->count - 1], k->symbol);

  e->nodes[e->count - 1] = Node__copy(v);
}

void environment_add_builtin(Environment* e, char* name, BuiltIn function) {
  Node* k = new__SymbolNode(name);
  char display_name[strlen(name) + 3];
  sprintf(display_name, "(%s)", name);
  Node* v = new__FunctionNode(function, display_name);
  environment_put(e, k, v);
  Node__free(k);
  Node__free(v);
}

void environment_add_builtins(Environment* e) {
  environment_add_builtin(e, "define", builtin_define);

  environment_add_builtin(e, "list", builtin_list);
  environment_add_builtin(e, "head", builtin_head);
  environment_add_builtin(e, "tail", builtin_tail);
  environment_add_builtin(e, "join", builtin_join);
  environment_add_builtin(e, "length", builtin_length);
  environment_add_builtin(e, "initial", builtin_initial);
  environment_add_builtin(e, "construct", builtin_construct);
  environment_add_builtin(e, "evaluate", builtin_evaluate);

  environment_add_builtin(e, "+", builtin_add);
  environment_add_builtin(e, "-", builtin_subtract);
  environment_add_builtin(e, "*", builtin_multiply);
  environment_add_builtin(e, "/", builtin_divide);
  environment_add_builtin(e, "%", builtin_modulo);

  environment_add_builtin(e, "add", builtin_add);
  environment_add_builtin(e, "subtract", builtin_subtract);
  environment_add_builtin(e, "multiply", builtin_multiply);
  environment_add_builtin(e, "divide", builtin_divide);
  environment_add_builtin(e, "modulo", builtin_modulo);
}
