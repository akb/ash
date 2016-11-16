#include "environment.h"
#include "node.h"

#define ASSERT_ARGUMENT(ARGS, COND, FMT, ...) \
  if (!(COND)) { \
    Node* error = new_node_error(FMT, ##__VA_ARGS__); \
    node_delete(ARGS); \
    return error; \
  }

#define ASSERT_ARGUMENT_COUNT(NAME, ARGS, COUNT) \
  if (ARGS->count != COUNT) { \
    Node* error = new_node_error( \
      "Function '%s' was passed wrong number of arguments. Expected %i got %i.", \
      NAME, COUNT, ARGS->count); \
    node_delete(ARGS); \
    return error; \
  }

#define ASSERT_ARGUMENT_TYPE(NAME, ARGS, POS, TYPE) \
  if (ARGS->cell[POS]->type != TYPE) { \
    Node* error = new_node_error( \
      "Wrong argument type passed to %s, position %i. Expected %s, got %s.", \
      NAME, POS, nodetype_name(TYPE), nodetype_name(ARGS->cell[POS]->type)); \
    node_delete(ARGS); \
    return error; \
  }

Node* builtin_divide(Environment*, Node*);
Node* builtin_multiply(Environment*, Node*);
Node* builtin_subtract(Environment*, Node*);
Node* builtin_add(Environment*, Node*);
Node* builtin_modulo(Environment*, Node*);

Node* builtin_list(Environment*, Node*);
Node* builtin_construct(Environment*, Node*);
Node* builtin_join(Environment*, Node*);
Node* builtin_length(Environment*, Node*);
Node* builtin_head(Environment*, Node*);
Node* builtin_tail(Environment*, Node*);
Node* builtin_initial(Environment*, Node*);

Node* builtin_evaluate(Environment*, Node*);
Node* builtin_define(Environment*, Node*);
