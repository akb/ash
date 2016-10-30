#include "node.h"
#include "mpc.h"

#define ASSERT_NODE(ARGS, COND, ERR) \
  if (!(COND)) { \
    Node__free(ARGS); \
    return new__ErrorNode(ERR); \
  }

#define ASSERT_NODE_LENGTH(ARGS, COUNT, ERR) \
  if (ARGS->count != COUNT) { \
    Node__free(ARGS); \
    return new__ErrorNode(ERR); \
  }

Node* Node__evaluate(Node*);
Node* Node__evaluate_s_expression(Node*);
Node* Node__pop(Node*, int);
Node* Node__take(Node*, int);
void NumberNode__negate_mutate(Node*);
void NumberNode__add_mutate(Node*, Node*);
void NumberNode__subtract_mutate(Node*, Node*);
void NumberNode__multiply_mutate(Node*, Node*);
void NumberNode__divide_mutate(Node*, Node*);
Node* builtin_op(Node*, char*);
Node* builtin(Node*, char*);
