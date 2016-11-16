#include <stdlib.h>
#include <strings.h>
#include <math.h>

#include "environment.h"
#include "node.h"
#include "builtins.h"

void node_number_negate_mutate(Node*);
void node_number_add_mutate(Node*, Node*);
void node_number_subtract_mutate(Node*, Node*);
void node_number_multiply_mutate(Node*, Node*);
void node_number_divide_mutate(Node*, Node*);
void node_number_modulo_mutate(Node*, Node*);

Node* builtin_op(Environment* e, Node* a, char* op) {
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != NODE_INTEGER &&
        a->cell[i]->type != NODE_DECIMAL) {
      node_delete(a);
      return new_node_error("Cannot call '%s' on non-number", op);
    }
  }

  Node* x = node_pop(a, 0);

  if ((strcmp(op, "-") || strcmp(op, "subtract")) && a->count == 0) {
    node_number_negate_mutate(x);
  }

  while (a->count > 0) {
    Node* y = node_pop(a, 0);
    if ((x->type == NODE_DECIMAL) != (y->type == NODE_DECIMAL)) {
      if (x->type == NODE_INTEGER) {
        x->type = NODE_DECIMAL;
        x->decimal = (double)x->integer;
      }
      if (y->type == NODE_INTEGER) {
        y->type = NODE_DECIMAL;
        y->decimal = (double)y->integer;
      }
    }
    if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0)
      node_number_add_mutate(x, y);
    if (strcmp(op, "-") == 0 || strcmp(op, "subtract") == 0)
      node_number_subtract_mutate(x, y);
    if (strcmp(op, "*") == 0 || strcmp(op, "multiply") == 0)
      node_number_multiply_mutate(x, y);
    if (strcmp(op, "%") == 0 || strcmp(op, "modulo") == 0)
      node_number_modulo_mutate(x, y);
    if (strcmp(op, "/") == 0 || strcmp(op, "divide") == 0) {
      if ((y->type == NODE_INTEGER && y->integer == 0) ||
          (y->type == NODE_DECIMAL && y->decimal == 0.0)) {
        node_delete(x);
        node_delete(y);
        x = new_node_error("Cannot divide by zero.");
        break;
      } else {
        node_number_divide_mutate(x, y);
      }
    }

    node_delete(y);
  }

  node_delete(a);
  return x;
}

Node* builtin_add(Environment* e, Node* a) {
  return builtin_op(e, a, "+");
}

Node* builtin_subtract(Environment* e, Node* a) {
  return builtin_op(e, a, "-");
}

Node* builtin_multiply(Environment* e, Node* a) {
  return builtin_op(e, a, "*");
}

Node* builtin_divide(Environment* e, Node* a) {
  return builtin_op(e, a, "/");
}

Node* builtin_modulo(Environment* e, Node* a) {
  return builtin_op(e, a, "%");
}

Node* builtin_head(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("head", a, 1);
  ASSERT_ARGUMENT_TYPE("head", a, 0, NODE_Q_EXPRESSION);
  ASSERT_ARGUMENT(a, a->cell[0]->count != 0,
    "Function \"head\" was passed {}");

  Node* v = node_take(a, 0);

  while (v->count > 1) node_delete(node_pop(v, 1));
  return v;
}

Node* builtin_tail(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("tail", a, 1);
  ASSERT_ARGUMENT_TYPE("tail", a, 0, NODE_Q_EXPRESSION);
  ASSERT_ARGUMENT(a, a->cell[0]->count != 0,
    "Function \"tail\" was passed {}");

  Node* v = node_take(a, 0);
  node_delete(node_pop(v, 0));
  return v;
}

Node* builtin_list(Environment* e, Node* a) {
  a->type = NODE_Q_EXPRESSION;
  return a;
}

Node* builtin_evaluate(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("evaluate", a, 1);
  ASSERT_ARGUMENT_TYPE("evaluate", a, 0, NODE_Q_EXPRESSION);

  Node* x = node_take(a, 0);
  x->type = NODE_S_EXPRESSION;
  return node_evaluate(e, x);
}

Node* builtin_join(Environment* e, Node* a) {
  for (int i = 0; i < a->count; i++) {
    ASSERT_ARGUMENT_TYPE("join", a, i, NODE_Q_EXPRESSION);
  }

  Node* x = node_pop(a, 0);
  while (a->count) x = node_join(x, node_pop(a, 0));

  node_delete(a);
  return x;
}

Node* builtin_construct(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("construct", a, 2);
  ASSERT_ARGUMENT_TYPE("construct", a, 1, NODE_Q_EXPRESSION);

  Node* head = node_pop(a, 0);
  Node* tail = node_pop(a, 0);
  node_delete(a);
  Node* x = new_node_q_expression();
  x = node_add(x, head);
  while (tail->count) x = node_add(x, node_pop(tail, 0));
  return x;
}

Node* builtin_length(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("length", a, 1);
  ASSERT_ARGUMENT_TYPE("length", a, 0, NODE_Q_EXPRESSION);

  long count = a->cell[0]->count;
  node_delete(a);
  return new_node_integer(count);
}

Node* builtin_initial(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("initial", a, 1);
  ASSERT_ARGUMENT_TYPE("initial", a, 0, NODE_Q_EXPRESSION);

  Node* list = node_pop(a, 0);
  node_delete(a);

  Node* x = new_node_q_expression();
  while (list->count > 1) x = node_add(x, node_pop(list, 0));
  return x;
}

Node* builtin_define(Environment* e, Node* a) {
  ASSERT_ARGUMENT_TYPE("define", a, 0, NODE_Q_EXPRESSION);

  Node* symbols = a->cell[0];

  for (int i = 0; i < symbols->count - 1; i++) {
    ASSERT_ARGUMENT_TYPE("define", symbols, i, NODE_SYMBOL);
  }

  ASSERT_ARGUMENT(a, symbols->count == a->count - 1,
    "incorrect number of arguments");

  for (int i = 0; i < symbols->count; i++) {
    environment_put(e, symbols->cell[i], a->cell[i + 1]);
  }

  node_delete(a);
  return new_node_s_expression();
}

void node_number_negate_mutate(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer = -v->integer;
      return;
    case NODE_DECIMAL:
      v->decimal = -v->decimal;
      return;
  }
}

void node_number_add_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer += x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal += x->decimal;
      return;
  }
}

void node_number_subtract_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer -= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal -= x->decimal;
      return;
  }
}

void node_number_multiply_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer *= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal *= x->decimal;
      return;
  }
}

void node_number_divide_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer /= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal /= x->decimal;
      return;
  }
}

void node_number_modulo_mutate(Node* v, Node* x) {
  switch (v->type) {
    case NODE_INTEGER:
      v->integer %= x->integer;
      return;
    case NODE_DECIMAL:
      v->decimal = fmod(v->decimal, x->decimal);
      return;
  }
}
