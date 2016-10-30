#include "result.h"
#include "mpc.h"

#define ASSERT_NODE(ARGS, COND, ERR) \
  if (!(COND)) { \
    Node__free(ARGS); \
    return new__ErrorNode(ERR); \
  }

Node* Node__eval(Node*);
Node* Node__eval_s_expression(Node*);
Node* Node__pop(Node*, int);
Node* Node__take(Node*, int);
void NumberNode__negate_mutate(Node*);
void NumberNode__add_mutate(Node*, Node*);
void NumberNode__subtract_mutate(Node*, Node*);
void NumberNode__multiply_mutate(Node*, Node*);
void NumberNode__divide_mutate(Node*, Node*);
Node* builtin_op(Node*, char*);
Node* builtin(Node*, char*);
