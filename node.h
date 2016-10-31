#ifndef NODE_TYPE_DEFINED
#define NODE_TYPE_DEFINED
#include "environment.h"

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

/**
 * new__IntegerNode
 * Constructs a new Node struct containing a long integer
 */
Node* new__IntegerNode(long);

/**
 * new__ErrorNode
 * Constructs a new Node struct containing an error
 */
Node* new__ErrorNode(char*);

/**
 * new__DecimalNode
 * Constructs a new Node struct containing a double precision floating
 * point number
 */
Node* new__DecimalNode(double);

/**
 * new__SymbolNode
 * Constructs a new Node struct containing a symbol
 */
Node* new__SymbolNode(char*);

/**
 * new__SExpressionNode
 * Constructs a new Node struct containing an empty S-Expression
 */
Node* new__SExpressionNode(void);

/**
 * new__QExpressionNode
 * Constructs a new Node struct containing an empty Q-Expression
 */
Node* new__QExpressionNode(void);

/**
 * new__FunctionNode(BuiltIn function)
 */
Node* new__FunctionNode(BuiltIn function);

/**
 * Node__free
 * Frees any memory allocated to the passed-in Node, deleting the
 * Node
 */
void Node__free(Node*);

/**
 * Node* Node__copy(Node*);
 * Creates a copy of the passed-in Node and returns it
 */
Node* Node__copy(Node*);

/**
 * Node__print
 * Prints the passed-in Node to STDOUT
 */
void Node__print(Node*);

/**
 * IntegerNode__print
 * Prints the passed-in Node containing an integer to STDOUT
 */
void IntegerNode__print(Node*);

/**
 * DecimalNode__print
 * Prints the passed-in Node containing a decimal number to STDOUT
 */
void DecimalNode__print(Node*);

/**
 * ErrorNode__print
 * Prints the passed-in Node containing an error to STDOUT
 */
void ErrorNode__print(Node*);

/**
 * SymbolNode__print
 * Prints the passed-in Node containing a symbol to STDOUT
 */
void SymbolNode__print(Node*);

/**
 * ExpressionNode__print
 * Prints the passed-in Node containing an expression to STDOUT
 */
void ExpressionNode__print(Node*, char, char);

/**
 * Node__println
 * Prints the passed-in Node to STDOUT followed by a newline
 */
void Node__println(Node*);

/**
 */
double Node__to_double(Node*);

/**
 */
long Node__to_long(Node*);

Node* Node__evaluate(Environment* e, Node*);
Node* Node__evaluate_s_expression(Environment* e, Node*);
Node* Node__pop(Node*, int);
Node* Node__take(Node*, int);
Node* Node__join(Node* x, Node* y);
Node* Node__add(Node* v, Node* x);
void NumberNode__negate_mutate(Node*);
void NumberNode__add_mutate(Node*, Node*);
void NumberNode__subtract_mutate(Node*, Node*);
void NumberNode__multiply_mutate(Node*, Node*);
void NumberNode__divide_mutate(Node*, Node*);

#endif
