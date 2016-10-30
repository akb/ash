#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "node.h"

/**
 * dtol
 * converts double-precision floating-point numbers to long integers
 * from: https://www.cs.tut.fi/~jkorpela/round.html
 */
long dtol(double x) {
  assert(x >= LONG_MIN-0.5);
  assert(x <= LONG_MAX+0.5);
  if (x >= 0) { return (long) (x+0.5); }
  return (long) (x-0.5);
}

Node* new__IntegerNode(long x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_INTEGER;
  v->integer = x;
  return v;
}

Node* new__DecimalNode(double x) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_DECIMAL;
  v->decimal = x;
  return v;
}

Node* new__ErrorNode(char* error) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_ERROR;
  v->error = malloc(strlen(error) + 1);
  strcpy(v->error, error); // TODO: null terminator?
  return v;
}

Node* new__SymbolNode(char* symbol) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol); // TODO: null terminator?
  return v;
}

Node* new__SExpressionNode(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_S_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

Node* new__QExpressionNode(void) {
  Node* v = malloc(sizeof(Node));
  v->type = NODE_Q_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void Node__free(Node* v) {
  switch (v->type) {
    case NODE_INTEGER: break;
    case NODE_DECIMAL: break;
    case NODE_ERROR:   free(v->error); break;
    case NODE_SYMBOL:  free(v->symbol); break;
    case NODE_Q_EXPRESSION:
    case NODE_S_EXPRESSION:
      for (int i = 0; i < v->count; i++)
        Node__free(v->cell[i]);
      free(v->cell);
      break;
  }
  free(v);
}

void Node__print(Node* v) {
  switch (v->type) {
    case NODE_INTEGER:
      IntegerNode__print(v);
      break;
    case NODE_DECIMAL:
      DecimalNode__print(v);
      break;
    case NODE_SYMBOL:
      SymbolNode__print(v);
      break;
    case NODE_S_EXPRESSION:
      ExpressionNode__print(v, '(', ')');
      break;
    case NODE_Q_EXPRESSION:
      ExpressionNode__print(v, '{', '}');
      break;
    case NODE_ERROR:
      ErrorNode__print(v);
      break;
  }
}

void IntegerNode__print(Node* v) {
  printf("%li", v->integer);
}

void DecimalNode__print(Node* v) {
  printf("%g", v->decimal);
}

void SymbolNode__print(Node* v) {
  printf("%s", v->symbol);
}

void ExpressionNode__print(Node* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    Node__print(v->cell[i]);
    if (i != (v->count - 1)) putchar(' ');
  }
  putchar(close);
}

void ErrorNode__print(Node* v) {
  fprintf(stderr, "Error: %s", v->error);
}

void Node__println(Node* v) {
  Node__print(v);
  putchar('\n');
}

double Node__to_double(Node* v) {
  switch (v->type) {
    case NODE_INTEGER: return (double)v->integer;
    case NODE_DECIMAL: return v->decimal;
    default: return 0;
  }
}

long Node__to_long(Node* v) {
  switch (v->type) {
    case NODE_INTEGER: return v->integer;
    case NODE_DECIMAL: return dtol(v->decimal);
    default: return 0;
  }
}
