#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "node.h"

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

char* NodeType__name(int type) {
  switch (type) {
    case NODE_SYMBOL:       return "symbol";
    case NODE_S_EXPRESSION: return "s_expression";
    case NODE_Q_EXPRESSION: return "q_expression";
    case NODE_INTEGER:      return "integer";
    case NODE_DECIMAL:      return "decimal";
    case NODE_ERROR:        return "error";
    case NODE_FUNCTION:     return "function";
    default:                return "unknown";
  }
}

Node* new__IntegerNode(long x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_INTEGER;
  v->integer = x;
  return v;
}

Node* new__DecimalNode(double x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_DECIMAL;
  v->decimal = x;
  return v;
}

Node* new__ErrorNode(char* fmt, ...) {
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

Node* new__SymbolNode(char* symbol) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol);
  return v;
}

Node* new__SExpressionNode(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_S_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

Node* new__QExpressionNode(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_Q_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

Node* new__FunctionNode(BuiltIn function, char* name) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_FUNCTION;
  v->function = function;
  if (name == NULL) {
    name = "(anonymous)";
  }
  v->symbol = malloc(strlen(name) + 1);
  strcpy(v->symbol, name);
  return v;
}

Node* Node__copy(Node* v) {
  Node* x = malloc(sizeof(Node));
  x->type = v->type;

  switch (v->type) {
    case NODE_INTEGER:  x->integer = v->integer;   break;
    case NODE_DECIMAL:  x->decimal = v->decimal;   break;
    case NODE_ERROR:
      x->error = malloc(strlen(v->error) + 1);
      strcpy(x->error, v->error);
      break;
    case NODE_FUNCTION:
      x->function = v->function;
    case NODE_SYMBOL:
      x->symbol = malloc(strlen(v->symbol) + 1);
      strcpy(x->symbol, v->symbol);
      break;
    case NODE_S_EXPRESSION:
    case NODE_Q_EXPRESSION:
      x->count = v->count;
      x->cell = malloc(sizeof(Node*) * x->count);
      for (int i = 0; i < x->count; i++) {
        x->cell[i] = Node__copy(v->cell[i]);
      }
      break;
  }
  return x;
}

void Node__free(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:                 break;
    case NODE_DECIMAL:                 break;
    case NODE_FUNCTION:
    case NODE_SYMBOL: free(v->symbol); break;
    case NODE_ERROR:  free(v->error);  break;
    case NODE_Q_EXPRESSION:
    case NODE_S_EXPRESSION:
      for (int i = 0; i < v->count; i++)
        Node__free(v->cell[i]);
      free(v->cell);
      break;
  }
  free(v);
}

void Node__print(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:      printf("%li", v->integer);          break;
    case NODE_DECIMAL:      printf("%g", v->decimal);           break;
    case NODE_SYMBOL:       printf("%s", v->symbol);            break;
    case NODE_S_EXPRESSION: ExpressionNode__print(v, '(', ')'); break;
    case NODE_Q_EXPRESSION: ExpressionNode__print(v, '{', '}'); break;
    case NODE_ERROR:        ErrorNode__print(v);                break;
    case NODE_FUNCTION:     printf("<function:%s>", v->symbol); break;
  }
}

void ExpressionNode__print(Node* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    Node__print(v->cell[i]);
    if (i != (v->count - 1)) putchar(' ');
  }
  putchar(close);
}

void ErrorNode__print(Node* v) {
  fprintf(stderr, "Error: %s", v->error);
}

void Node__println(Node* v) {
  Node__print(v);
  putchar('\n');
}

double Node__to_double(Node* v) {
  switch (v->type) {
    case NODE_INTEGER: return (double)v->integer;
    case NODE_DECIMAL: return v->decimal;
    default: return 0;
  }
}

long Node__to_long(Node* v) {
  switch (v->type) {
    case NODE_INTEGER: return v->integer;
    case NODE_DECIMAL: return dtol(v->decimal);
    default:           return 0;
  }
}

Node* Node__evaluate(Environment* e, Node* v) {
  if (v->type == NODE_SYMBOL) {
    Node* x = environment_get(e, v);
    Node__free(v);
    return x;
  } else if (v->type == NODE_S_EXPRESSION) {
    return Node__evaluate_s_expression(e, v);
  } else {
    return v;
  }
}

Node* Node__evaluate_s_expression(Environment* e, Node* v) {
  for (int i = 0; i < v->count; i++)
    v->cell[i] = Node__evaluate(e, v->cell[i]);

  for (int i = 0; i < v->count; i++)
    if (v->cell[i]->type == NODE_ERROR)
      return Node__take(v, i);

  if (v->count == 0) return v;
  if (v->count == 1) return Node__take(v, 0);

  Node* f = Node__pop(v, 0);
  if (f->type != NODE_FUNCTION) {
    Node__free(f);
    Node__free(v);
    return new__ErrorNode("S-Expression does not begin with a function.");
  }

  Node* result = f->function(e, v);
  Node__free(f);
  return result;
}

Node* Node__pop(Node* v, int i) {
  Node* x = v->cell[i];
  memmove(&v->cell[i], &v->cell[i+1], sizeof(Node*) * (v->count-i-1));
  v->count--;
  v->cell = realloc(v->cell, sizeof(Node*) * v->count);
  return x;
}

Node* Node__take(Node* v, int i) {
  Node* x = Node__pop(v, i);
  Node__free(v);
  return x;
}

Node* Node__join(Node* x, Node* y) {
  while (y->count) x = Node__add(x, Node__pop(y, 0));
  Node__free(y);
  return x;
}

void NumberNode__negate_mutate(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer = -v->integer;
      return;
    case NODE_DECIMAL:
      v->decimal = -v->decimal;
      return;
  }
}

void NumberNode__add_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer += x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal += x->decimal;
      return;
  }
}

void NumberNode__subtract_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer -= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal -= x->decimal;
      return;
  }
}

void NumberNode__multiply_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer *= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal *= x->decimal;
      return;
  }
}

void NumberNode__divide_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer /= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal /= x->decimal;
      return;
  }
}

void NumberNode__modulo_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer %= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal = fmod(v->decimal, x->decimal);
      return;
  }
}

Node* Node__add(Node* v, Node* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(Node*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}
