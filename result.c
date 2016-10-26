#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "result.h"

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

ResultValue* new__IntegerResultValue(long x) {
  ResultValue* v = malloc(sizeof(ResultValue));
  v->type = RESULT_VALUE_INTEGER;
  v->integer = x;
  return v;
}

ResultValue* new__DecimalResultValue(double x) {
  ResultValue* v = malloc(sizeof(ResultValue));
  v->type = RESULT_VALUE_DECIMAL;
  v->decimal = x;
  return v;
}

ResultValue* new__ErrorResultValue(char* error) {
  ResultValue* v = malloc(sizeof(ResultValue));
  v->type = RESULT_VALUE_ERROR;
  v->error = malloc(strlen(error) + 1);
  strcpy(v->error, error); // TODO: null terminator?
  return v;
}

ResultValue* new__SymbolResultValue(char* symbol) {
  ResultValue* v = malloc(sizeof(ResultValue));
  v->type = RESULT_VALUE_SYMBOL;
  v->symbol = malloc(strlen(symbol) + 1);
  strcpy(v->symbol, symbol); // TODO: null terminator?
  return v;
}

ResultValue* new__SExpressionResultValue(void) {
  ResultValue* v = malloc(sizeof(ResultValue));
  v->type = RESULT_VALUE_S_EXPRESSION;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void ResultValue__free(ResultValue* v) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER: break;
    case RESULT_VALUE_DECIMAL: break;
    case RESULT_VALUE_ERROR:   free(v->error); break;
    case RESULT_VALUE_SYMBOL:  free(v->symbol); break;
    case RESULT_VALUE_S_EXPRESSION:
      for (int i = 0; i < v->count; i++)
        ResultValue__free(v->cell[i]);
      free(v->cell);
      break;
  }
  free(v);
}

void ResultValue__print(ResultValue* v) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      IntegerResultValue__print(v);
      break;
    case RESULT_VALUE_DECIMAL:
      DecimalResultValue__print(v);
      break;
    case RESULT_VALUE_SYMBOL:
      SymbolResultValue__print(v);
      break;
    case RESULT_VALUE_S_EXPRESSION:
      SExpressionResultValue__print(v, '(', ')');
      break;
    case RESULT_VALUE_ERROR:
      ErrorResultValue__print(v);
      break;
  }
}

void IntegerResultValue__print(ResultValue* v) {
  printf("%li", v->integer);
}

void DecimalResultValue__print(ResultValue* v) {
  printf("%g", v->decimal);
}

void SymbolResultValue__print(ResultValue* v) {
  printf("%s", v->symbol);
}

void SExpressionResultValue__print(ResultValue* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    ResultValue__print(v->cell[i]);
    if (i != (v->count - 1)) putchar(' ');
  }
  putchar(close);
}

void ErrorResultValue__print(ResultValue* v) {
  fprintf(stderr, "Error: %s", v->error);
}

void ResultValue__println(ResultValue* v) {
  ResultValue__print(v);
  putchar('\n');
}

double ResultValue__to_double(ResultValue* v) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER: return (double)v->integer;
    case RESULT_VALUE_DECIMAL: return v->decimal;
    default: return 0;
  }
}

long ResultValue__to_long(ResultValue* v) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER: return v->integer;
    case RESULT_VALUE_DECIMAL: return dtol(v->decimal);
    default: return 0;
  }
}
