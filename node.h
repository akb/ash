#ifndef NODE_TYPE_DEFINED
#define NODE_TYPE_DEFINED
#include "environment.h"

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

/**
 * NODE_INTEGER
 * Indicates that the Node contains an integer
 *
 * NODE_DECIMAL
 * Indicates that the Node contains a decimal number
 *
 * NODE_ERROR
 * Indicates that the Node contains an error
 */
enum {
  NODE_SYMBOL,
  NODE_S_EXPRESSION,
  NODE_Q_EXPRESSION,
  NODE_INTEGER,
  NODE_DECIMAL,
  NODE_ERROR,
  NODE_FUNCTION
};

typedef struct NodeStruct {
  int     type;
  long    integer;
  double  decimal;
  char*   error;
  char*   symbol;
  BuiltIn function;
  int     count;
  struct  NodeStruct** cell;
} Node;

char* nodetype_name(int);

/**
 * new_node_integer
 * Constructs a new Node struct containing a long integer
 */
Node* new_node_integer(long);

/**
 * new_node_error
 * Constructs a new Node struct containing an error
 */
Node* new_node_error(char*, ...);

/**
 * new_node_decimal
 * Constructs a new Node struct containing a double precision floating
 * point number
 */
Node* new_node_decimal(double);

/**
 * new_node_symbol
 * Constructs a new Node struct containing a symbol
 */
Node* new_node_symbol(char*);

/**
 * new_node_s_expression
 * Constructs a new Node struct containing an empty S-Expression
 */
Node* new_node_s_expression(void);

/**
 * new_node_q_expression
 * Constructs a new Node struct containing an empty Q-Expression
 */
Node* new_node_q_expression(void);

/**
 * new_node_function(BuiltIn function)
 */
Node* new_node_function(BuiltIn, char*);

/**
 * node_delete
 * Frees any memory allocated to the passed-in Node, deleting the
 * Node
 */
void node_delete(Node*);

/**
 * Node* node_copy(Node*);
 * Creates a copy of the passed-in Node and returns it
 */
Node* node_copy(Node*);

/**
 * node_print
 * Prints the passed-in Node to STDOUT
 */
void node_print(Node*);

/**
 * node_println
 * Prints the passed-in Node to STDOUT followed by a newline
 */
void node_println(Node*);

Node* node_evaluate(Environment* e, Node*);
Node* node_evaluate_s_expression(Environment* e, Node*);
Node* node_pop(Node*, int);
Node* node_take(Node*, int);
Node* node_join(Node* x, Node* y);
Node* node_add(Node* v, Node* x);
void node_number_negate_mutate(Node*);
void node_number_add_mutate(Node*, Node*);
void node_number_subtract_mutate(Node*, Node*);
void node_number_multiply_mutate(Node*, Node*);
void node_number_divide_mutate(Node*, Node*);
void node_number_modulo_mutate(Node*, Node*);

#endif
