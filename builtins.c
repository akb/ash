#include <stdlib.h>
#include <strings.h>

#include "environment.h"
#include "node.h"

Node* builtin_op(Environment* e, Node* a, char* op) {
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
  ASSERT_NODE_LENGTH(a, 1, "Function \"head\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"head\" only operates on Q-expressions");
  ASSERT_NODE(a, a->cell[0]->count != 0,
    "Function \"head\" was passed {}");

  Node* v = Node__take(a, 0);

  while (v->count > 1) Node__free(Node__pop(v, 1));
  return v;
}

Node* builtin_tail(Environment* e, Node* a) {
  ASSERT_NODE_LENGTH(a, 1, "Function \"tail\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"tail\" only operates on Q-expressions");
  ASSERT_NODE(a, a->cell[0]->count != 0,
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
  ASSERT_NODE_LENGTH(a, 1, "Function \"evaluate\" only accepts one argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"evaluate\" only operates on Q-expressions");

  Node* x = Node__take(a, 0);
  x->type = NODE_S_EXPRESSION;
  return Node__evaluate(e, x);
}

Node* builtin_join(Environment* e, Node* a) {
  for (int i = 0; i < a->count; i++) {
    ASSERT_NODE(a, a->cell[i]->type == NODE_Q_EXPRESSION,
      "Function \"join\" passed an invalid type");
  }

  Node* x = Node__pop(a, 0);
  while (a->count) x = Node__join(x, Node__pop(a, 0));

  Node__free(a);
  return x;
}

Node* builtin_construct(Environment* e, Node* a) {
  ASSERT_NODE_LENGTH(a, 2, "Function \"construct\" requires two arguments");
  ASSERT_NODE(a, a->cell[1]->type == NODE_Q_EXPRESSION,
    "Function \"construct\" requires a Q-expression as its second argument");

  Node* head = Node__pop(a, 0);
  Node* tail = Node__pop(a, 0);
  Node__free(a);
  Node* x = new__QExpressionNode();
  x = Node__add(x, head);
  while (tail->count) x = Node__add(x, Node__pop(tail, 0));
  return x;
}

Node* builtin_length(Environment* e, Node* a) {
  ASSERT_NODE_LENGTH(a, 1, "Function \"length\" requires a single argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"length\" requires a Q-expression as its only argument");

  long count = a->cell[0]->count;
  Node__free(a);
  return new__IntegerNode(count);
}

Node* builtin_initial(Environment* e, Node* a) {
  ASSERT_NODE_LENGTH(a, 1, "Function \"length\" requires a single argument");
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "Function \"length\" requires a Q-expression as its only argument");

  Node* list = Node__pop(a, 0);
  Node__free(a);

  Node* x = new__QExpressionNode();
  while (list->count > 1) x = Node__add(x, Node__pop(list, 0));
  return x;
}

Node* builtin_define(Environment* e, Node* a) {
  ASSERT_NODE(a, a->cell[0]->type == NODE_Q_EXPRESSION,
    "The first argument passed to 'define' must be a list of symbols");

  Node* symbols = a->cell[0];

  for (int i = 0; i < symbols->count - 1; i++) {
    ASSERT_NODE(a, symbols->cell[i]->type == NODE_SYMBOL,
      "cannot define a non-symbol as an argument");
  }

  ASSERT_NODE(a, symbols->count == a->count - 1,
    "incorrect number of arguments");

  for (int i = 0; i < symbols->count; i++) {
    Environment__put(e, symbols->cell[i], a->cell[i + 1]);
  }

  Node__free(a);
  return new__SExpressionNode();
}
