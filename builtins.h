#include "environment.h"
#include "node.h"

Node* builtin_initial(Environment*, Node*);
Node* builtin_length(Environment*, Node*);
Node* builtin_construct(Environment*, Node*);
Node* builtin_join(Environment*, Node*);
Node* builtin_evaluate(Environment*, Node*);
Node* builtin_list(Environment*, Node*);
Node* builtin_tail(Environment*, Node*);
Node* builtin_head(Environment*, Node*);
Node* builtin_modulo(Environment*, Node*);
Node* builtin_divide(Environment*, Node*);
Node* builtin_multiply(Environment*, Node*);
Node* builtin_subtract(Environment*, Node*);
Node* builtin_add(Environment*, Node*);
Node* builtin_define(Environment*, Node*);
