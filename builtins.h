#include "environment.h"
#include "node.h"

Node* builtin_initial(Environment* e, Node* a);
Node* builtin_length(Environment* e, Node* a);
Node* builtin_construct(Environment* e, Node* a);
Node* builtin_join(Environment* e, Node* a);
Node* builtin_evaluate(Environment* e, Node* a);
Node* builtin_list(Environment* e, Node* a);
Node* builtin_tail(Environment* e, Node* a);
Node* builtin_head(Environment* e, Node* a);
Node* builtin_modulo(Environment* e, Node* a);
Node* builtin_divide(Environment* e, Node* a);
Node* builtin_multiply(Environment* e, Node* a);
Node* builtin_subtract(Environment* e, Node* a);
Node* builtin_add(Environment* e, Node* a);
