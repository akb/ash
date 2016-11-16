#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "node.h"

Node* builtin_evaluate(Environment*, Node*);

Node* node_call(Environment* e, Node* f, Node* a);
void node_expression_print(Node*, char, char);
Node* node_evaluate_s_expression(Environment* e, Node*);
void node_print(Node*);

/**
 * dtol
 * converts double-precision floating-point numbers to long integers
 * from: https://www.cs.tut.fi/~jkorpela/round.html
 */
long dtol(double x) {
  assert(x >= LONG_MIN-0.5);
  assert(x <= LONG_MAX+0.5);
  if (x >= 0) { return (long) (x+0.5); }
  return (long) (x-0.5);
}

char* nodetype_name(int type) {
  switch (type) {
    case NODE_SYMBOL:       return "symbol";
    case NODE_S_EXPRESSION: return "s_expression";
    case NODE_Q_EXPRESSION: return "q_expression";
    case NODE_INTEGER:      return "integer";
    case NODE_DECIMAL:      return "decimal";
    case NODE_ERROR:        return "error";
    case NODE_BUILTIN:      return "builtin";
    case NODE_EXIT:         return "exit";
    default:                return "unknown";
  }
}

Node* new_node_integer(long x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_INTEGER;
  v->integer = x;
  return v;
}

Node* new_node_decimal(double x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_DECIMAL;
  v->decimal = x;
  return v;
}

Node* new_node_error(char* fmt, ...) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_ERROR;

  va_list va;
  va_start(va, fmt);

  v->error = malloc(512);
  vsnprintf(v->error, 511, fmt, va);
  v->error = realloc(v->error, strlen(v->error) + 1);

  va_end(va);
  return v;
}

Node* new_node_symbol(char* symbol) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol);
  return v;
}

Node* new_node_s_expression(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_S_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

Node* new_node_q_expression(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_Q_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

Node* new_node_builtin(BuiltIn builtin, char* name) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_BUILTIN;
  v->builtin = builtin;
  if (name == NULL) name = "(anonymous)";
  v->symbol = malloc(strlen(name) + 1);
  strcpy(v->symbol, name);
  return v;
}

Node* new_node_function(Node* arguments, Node* body) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_FUNCTION;
  v->builtin = NULL;
  v->environment = new_environment();
  v->arguments = arguments;
  v->body = body;
  return v;
}

Node* new_node_exit(int code) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_EXIT;
  v->exit_code = code;
  return v;
}

Node* node_copy(Node* v) {
  Node* x = malloc(sizeof(Node));
  x->type = v->type;

  switch (v->type) {
    case NODE_INTEGER: x->integer = v->integer; break;
    case NODE_DECIMAL: x->decimal = v->decimal; break;
    case NODE_ERROR:
      x->error = malloc(strlen(v->error) + 1);
      strcpy(x->error, v->error);
      break;
    case NODE_FUNCTION:
    case NODE_BUILTIN:
      if (v->builtin) {
        x->builtin = v->builtin;
      } else {
        x->builtin = NULL;
        x->environment = environment_copy(v->environment);
        x->arguments = node_copy(v->arguments);
        x->body = node_copy(v->body);
      }
    case NODE_SYMBOL:
      x->symbol = malloc(strlen(v->symbol) + 1);
      strcpy(x->symbol, v->symbol);
      break;
    case NODE_S_EXPRESSION:
    case NODE_Q_EXPRESSION:
      x->count = v->count;
      x->cell = malloc(sizeof(Node*) * x->count);
      for (int i = 0; i < x->count; i++) {
        x->cell[i] = node_copy(v->cell[i]);
      }
      break;
  }
  return x;
}

void node_delete(Node* v) {
  switch (v->type) {
    case NODE_EXIT:                    break;
    case NODE_INTEGER:                 break;
    case NODE_DECIMAL:                 break;
    case NODE_BUILTIN:
    case NODE_SYMBOL: free(v->symbol); break;
    case NODE_ERROR:  free(v->error);  break;
    case NODE_Q_EXPRESSION:
    case NODE_S_EXPRESSION:
      for (int i = 0; i < v->count; i++)
        node_delete(v->cell[i]);
      free(v->cell);
      break;
    case NODE_FUNCTION:
      environment_delete(v->environment);
      node_delete(v->arguments);
      node_delete(v->body);
  }
  free(v);
}

void node_print(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:      printf("%li", v->integer);                  break;
    case NODE_DECIMAL:      printf("%g", v->decimal);                   break;
    case NODE_SYMBOL:       printf("%s", v->symbol);                    break;
    case NODE_S_EXPRESSION: node_expression_print(v, '(', ')');         break;
    case NODE_Q_EXPRESSION: node_expression_print(v, '{', '}');         break;
    case NODE_ERROR:        fprintf(stderr, "Error: %s", v->error);     break;
    case NODE_BUILTIN:      printf("<builtin:%s>", v->symbol);          break;
    case NODE_EXIT:         printf("Exiting. Code %d\n", v->exit_code); break;
    case NODE_FUNCTION:
      printf("(fn ");
      node_print(v->arguments);
      putchar(' ');
      node_print(v->body);
      putchar(' ');
  }
}

void node_expression_print(Node* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    node_print(v->cell[i]);
    if (i != (v->count - 1)) putchar(' ');
  }
  putchar(close);
}

void node_println(Node* v) {
  node_print(v);
  putchar('\n');
}

Node* node_evaluate(Environment* e, Node* v) {
  if (v->type == NODE_SYMBOL) {
    Node* x = environment_get(e, v);
    node_delete(v);
    return x;
  } else if (v->type == NODE_S_EXPRESSION) {
    return node_evaluate_s_expression(e, v);
  } else {
    return v;
  }
}

Node* node_evaluate_s_expression(Environment* e, Node* v) {
  for (int i = 0; i < v->count; i++)
    v->cell[i] = node_evaluate(e, v->cell[i]);

  for (int i = 0; i < v->count; i++)
    if (v->cell[i]->type == NODE_ERROR)
      return node_take(v, i);

  if (v->count == 0) return v;
  if (v->count == 1) return node_take(v, 0);

  Node* f = node_pop(v, 0);
  if (f->type != NODE_BUILTIN) {
    node_delete(f);
    node_delete(v);
    return new_node_error("S-Expression does not begin with a symbol.");
  }

  Node* result = node_call(e, f, v);
  node_delete(f);
  return result;
}

Node* node_pop(Node* v, int i) {
  Node* x = v->cell[i];
  memmove(&v->cell[i], &v->cell[i+1], sizeof(Node*) * (v->count-i-1));
  v->count--;
  v->cell = realloc(v->cell, sizeof(Node*) * v->count);
  return x;
}

Node* node_take(Node* v, int i) {
  Node* x = node_pop(v, i);
  node_delete(v);
  return x;
}

Node* node_join(Node* x, Node* y) {
  while (y->count) x = node_add(x, node_pop(y, 0));
  node_delete(y);
  return x;
}

Node* node_add(Node* v, Node* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(Node*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

Node* node_call(Environment* e, Node* f, Node* a) {
  if (f->builtin) return f->builtin(e, a);

  int given = a->count;
  int total = f->arguments->count;

  while (a->count) {
    if (f->arguments->count == 0) {
      node_delete(a);
      return new_node_error(
        "Function passed too many arguments. "
        "Got %i, Expected %i.", given, total);
    }

    Node* symbol = node_pop(f->arguments, 0);
    Node* value = node_pop(a, 0);
    environment_put(f->environment, symbol, value);
    node_delete(symbol);
    node_delete(value);
  }

  node_delete(a);

  if (f->arguments->count == 0) {
    f->environment->parent = e;
    return builtin_evaluate(
      f->environment, node_add(new_node_s_expression(), node_copy(f->body))
    );
  } else {
    return node_copy(f);
  }
}