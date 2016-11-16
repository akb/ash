#ifndef NODE_TYPE_DEFINED
#define NODE_TYPE_DEFINED
#include "environment.h"

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
  NODE_BUILTIN,
  NODE_FUNCTION,
  NODE_EXIT
};

typedef struct NodeStruct {
  int     type;

  /* simple */
  long   integer;
  double decimal;
  char*  error;
  char*  symbol;
  int    exit_code;

  /* builtin function */
  BuiltIn builtin;

  /* lambda function */
  Environment* environment;
  struct NodeStruct* arguments;
  struct NodeStruct* body;

  /* expression */
  int     count;
  struct  NodeStruct** cell;
} Node;

/**
 * nodetype_name
 * returns a string representation of the node type passed in.
 */
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
 * new_node_builtin(BuiltIn builtin)
 */
Node* new_node_builtin(BuiltIn, char*);

/**
 * new_node_function
 */
Node* new_node_function(Node*, Node*);

/**
 * new_node_exit
 * Creates a new Node which indicates to the runtime that the program should
 * exit. The first argument specifies the exit code to return to the OS.
 */
Node* new_node_exit(int);

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
 * node_println
 * Prints the passed-in Node to STDOUT followed by a newline
 */
void node_println(Node*);

Node* node_evaluate(Environment* e, Node*);
Node* node_pop(Node*, int);
Node* node_take(Node*, int);
Node* node_join(Node* x, Node* y);
Node* node_add(Node* v, Node* x);
#endif
