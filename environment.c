#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "environment.h"
#include "node.h"
#include "builtins.h"

/**
 * new__Environment
 */
Environment* new__Environment(void) {
  Environment* e = malloc(sizeof(Environment));
  e->count = 0;
  e->symbols = NULL;
  e->nodes = NULL;
  return e;
}

void Environment__free(Environment* e) {
  for (int i = 0; i < e->count; i++) {
    free(e->symbols[i]);
    Node__free(e->nodes[i]);
  }
  free(e->symbols);
  free(e->nodes);
  free(e);
}

Node* Environment__get(Environment* e, Node* k) {
  for (int i = 0; i < e->count; i++)
    if (strcmp(e->symbols[i], k->symbol) == 0)
      return Node__copy(e->nodes[i]);
  return new__ErrorNode("Undefined symbol '%s'", k->symbol);
}

void Environment__put(Environment* e, Node* k, Node* v) {
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

void Environment__add_builtin(Environment* e, char* name, BuiltIn function) {
  Node* k = new__SymbolNode(name);
  char display_name[strlen(name) + 3];
  sprintf(display_name, "(%s)", name);
  Node* v = new__FunctionNode(function, display_name);
  Environment__put(e, k, v);
  Node__free(k);
  Node__free(v);
}

void Environment__add_builtins(Environment* e) {
  Environment__add_builtin(e, "define", builtin_define);
  Environment__add_builtin(e, "environment", builtin_environment);

  Environment__add_builtin(e, "list", builtin_list);
  Environment__add_builtin(e, "head", builtin_head);
  Environment__add_builtin(e, "tail", builtin_tail);
  Environment__add_builtin(e, "join", builtin_join);
  Environment__add_builtin(e, "length", builtin_length);
  Environment__add_builtin(e, "initial", builtin_initial);
  Environment__add_builtin(e, "construct", builtin_construct);
  Environment__add_builtin(e, "evaluate", builtin_evaluate);

  Environment__add_builtin(e, "+", builtin_add);
  Environment__add_builtin(e, "-", builtin_subtract);
  Environment__add_builtin(e, "*", builtin_multiply);
  Environment__add_builtin(e, "/", builtin_divide);
  Environment__add_builtin(e, "%", builtin_modulo);

  Environment__add_builtin(e, "add", builtin_add);
  Environment__add_builtin(e, "subtract", builtin_subtract);
  Environment__add_builtin(e, "multiply", builtin_multiply);
  Environment__add_builtin(e, "divide", builtin_divide);
  Environment__add_builtin(e, "modulo", builtin_modulo);
}
