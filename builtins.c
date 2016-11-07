#include <stdlib.h>
#include <strings.h>

#include "environment.h"
#include "node.h"

Node* builtin_op(Environment* e, Node* a, char* op) {
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != NODE_INTEGER &&
        a->cell[i]->type != NODE_DECIMAL) {
      Node__free(a);
      return new__ErrorNode("Cannot call '%s' on non-number", op);
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
    if (strcmp(op, "%") == 0 || strcmp(op, "modulo") == 0)
      NumberNode__modulo_mutate(x, y);
    if (strcmp(op, "/") == 0 || strcmp(op, "divide") == 0) {
      if ((y->type == NODE_INTEGER && y->integer == 0) ||
          (y->type == NODE_DECIMAL && y->decimal == 0.0)) {
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

  Node* v = Node__take(a, 0);

  while (v->count > 1) Node__free(Node__pop(v, 1));
  return v;
}

Node* builtin_tail(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("tail", a, 1);
  ASSERT_ARGUMENT_TYPE("tail", a, 0, NODE_Q_EXPRESSION);
  ASSERT_ARGUMENT(a, a->cell[0]->count != 0,
    "Function \"tail\" was passed {}");

  Node* v = Node__take(a, 0);
  Node__free(Node__pop(v, 0));
  return v;
}

Node* builtin_list(Environment* e, Node* a) {
  a->type = NODE_Q_EXPRESSION;
  return a;
}

Node* builtin_evaluate(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("evaluate", a, 1);
  ASSERT_ARGUMENT_TYPE("evaluate", a, 0, NODE_Q_EXPRESSION);

  Node* x = Node__take(a, 0);
  x->type = NODE_S_EXPRESSION;
  return Node__evaluate(e, x);
}

Node* builtin_join(Environment* e, Node* a) {
  for (int i = 0; i < a->count; i++) {
    ASSERT_ARGUMENT_TYPE("join", a, i, NODE_Q_EXPRESSION);
  }

  Node* x = Node__pop(a, 0);
  while (a->count) x = Node__join(x, Node__pop(a, 0));

  Node__free(a);
  return x;
}

Node* builtin_construct(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("construct", a, 2);
  ASSERT_ARGUMENT_TYPE("construct", a, 1, NODE_Q_EXPRESSION);

  Node* head = Node__pop(a, 0);
  Node* tail = Node__pop(a, 0);
  Node__free(a);
  Node* x = new__QExpressionNode();
  x = Node__add(x, head);
  while (tail->count) x = Node__add(x, Node__pop(tail, 0));
  return x;
}

Node* builtin_length(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("length", a, 1);
  ASSERT_ARGUMENT_TYPE("length", a, 0, NODE_Q_EXPRESSION);

  long count = a->cell[0]->count;
  Node__free(a);
  return new__IntegerNode(count);
}

Node* builtin_initial(Environment* e, Node* a) {
  ASSERT_ARGUMENT_COUNT("initial", a, 1);
  ASSERT_ARGUMENT_TYPE("initial", a, 0, NODE_Q_EXPRESSION);

  Node* list = Node__pop(a, 0);
  Node__free(a);

  Node* x = new__QExpressionNode();
  while (list->count > 1) x = Node__add(x, Node__pop(list, 0));
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
    Environment__put(e, symbols->cell[i], a->cell[i + 1]);
  }

  Node__free(a);
  return new__SExpressionNode();
}
