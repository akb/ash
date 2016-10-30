#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "parser.h"
#include "eval.h"
#include "errors.h"

Node* Node__eval_s_expression(Node* v) {
  for (int i = 0; i < v->count; i++)
    v->cell[i] = Node__eval(v->cell[i]);

  for (int i = 0; i < v->count; i++)
    if (v->cell[i]->type == NODE_ERROR)
      return Node__take(v, i);

  if (v->count == 0) return v;
  if (v->count == 1) return Node__take(v, 0);

  Node* f = Node__pop(v, 0);
  if (f->type != NODE_SYMBOL) {
    Node__free(f);
    Node__free(v);
    return new__ErrorNode("S-Expression does not begin with a symbol.");
  }

  Node* result = builtin(v, f->symbol);
  Node__free(f);
  return result;
}

Node* Node__eval(Node* v) {
  if (v->type == NODE_S_EXPRESSION)
    return Node__eval_s_expression(v);
  else
    return v;
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

Node* builtin_op(Node* a, char* op) {
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != NODE_INTEGER &&
        a->cell[i]->type != NODE_DECIMAL) {
      Node__free(a);
      return new__ErrorNode("Cannot operate on non-number.");
    }
  }

  Node* x = Node__pop(a, 0);

  if ((strcmp(op, "-") || strcmp(op, "subtract")) && a->count == 0) {
    NumberNode__negate_mutate(x);
  }

  while (a->count > 0) {
    Node* y = Node__pop(a, 0);
    if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0)
      NumberNode__add_mutate(x, y);
    if (strcmp(op, "-") == 0 || strcmp(op, "subtract") == 0)
      NumberNode__subtract_mutate(x, y);
    if (strcmp(op, "*") == 0 || strcmp(op, "multiply") == 0)
      NumberNode__multiply_mutate(x, y);
    if (strcmp(op, "/") == 0 || strcmp(op, "divide") == 0) {
      if (y->type == NODE_INTEGER && y->integer == 0) {
        Node__free(x);
        Node__free(y);
        x = new__ErrorNode("Cannot divide by zero.");
        break;
      } else if (y->type == NODE_DECIMAL && y->decimal == 0.0) {
        Node__free(x);
        Node__free(y);
        x = new__ErrorNode("Cannot divide by zero.");
        break;
      } else {
        NumberNode__divide_mutate(x, y);
      }
    }

    Node__free(y);
  }

  Node__free(a);
  return x;
}

Node* builtin_head(Node* a) {
  ASSERT_NODE(a, a->count == 1,
    "Function \"head\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"head\" only operates on Q-expressions");
  ASSERT_NODE(a, a->cell[0]->count != 0,
    "Function \"head\" was passed {}");

  Node* v = Node__take(a, 0);

  while (v->count > 1) Node__free(Node__pop(v, 1));
  return v;
}

Node* builtin_tail(Node* a) {
  ASSERT_NODE(a, a->count == 1,
    "Function \"tail\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"tail\" only operates on Q-expressions");
  ASSERT_NODE(a, a->cell[0]->count != 0,
    "Function \"tail\" was passed {}");

  Node* v = Node__take(a, 0);
  Node__free(Node__pop(v, 0));
  return v;
}

Node* builtin_list(Node* a) {
  a->type = NODE_Q_EXPRESSION;
  return a;
}

Node* builtin_eval(Node* a) {
  ASSERT_NODE(a, a->count == 1,
    "Function \"eval\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"eval\" only operates on Q-expressions");

  Node* x = Node__take(a, 0);
  x->type = NODE_S_EXPRESSION;
  return Node__eval(x);
}

Node* builtin_join(Node* a) {
  for (int i = 0; i < a->count; i++) {
    ASSERT_NODE(a, a->cell[i]->type == NODE_Q_EXPRESSION,
      "Function \"join\" passed an invalid type");
  }

  Node* x = Node__pop(a, 0);
  while (a->count) x = Node__join(x, Node__pop(a, 0));

  Node__free(a);
  return x;
}

Node* builtin(Node* a, char* symbol) {
  if (strcmp("list", symbol) == 0) return builtin_list(a);
  if (strcmp("head", symbol) == 0) return builtin_head(a);
  if (strcmp("tail", symbol) == 0) return builtin_tail(a);
  if (strcmp("join", symbol) == 0) return builtin_join(a);
  if (strcmp("eval", symbol) == 0) return builtin_eval(a);
  if (strstr("+-/*%", symbol)) return builtin_op(a, symbol);
  if (strcmp("add", symbol)      == 0 ||
      strcmp("subtract", symbol) == 0 ||
      strcmp("multiply", symbol) == 0 ||
      strcmp("divide", symbol)   == 0 ||
      strcmp("modulo", symbol)   == 0)
    return builtin_op(a, symbol);
  Node__free(a);

  char* format = "Symbol %s is undefined";
  char message[strlen(symbol)+strlen(format)-2];
  sprintf(message, format, symbol);
  return new__ErrorNode(message);
}
