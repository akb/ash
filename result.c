#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

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

ResultValue new__IntegerResultValue(long x) {
  ResultValue v;
  v.type = RESULT_VALUE_INTEGER;
  v.integer = x;
  return v;
}

ResultValue new__DecimalResultValue(double x) {
  ResultValue v;
  v.type = RESULT_VALUE_DECIMAL;
  v.decimal = x;
  return v;
}

ResultValue new__ErrorResultValue(int e) {
  ResultValue v;
  v.type = RESULT_VALUE_ERROR;
  v.error = e;
  return v;
}

void ResultValue__print(ResultValue v) {
  switch (v.type) {
    case RESULT_VALUE_INTEGER:
      IntegerResultValue__print(v);
      break;
    case RESULT_VALUE_DECIMAL:
      DecimalResultValue__print(v);
      break;
    case RESULT_VALUE_ERROR:
      ErrorResultValue__print(v);
      break;
  }
}

void IntegerResultValue__print(ResultValue v) {
  printf("%li", v.integer);
}

void DecimalResultValue__print(ResultValue v) {
  printf("%g", v.decimal);
}

void ErrorResultValue__print(ResultValue v) {
  switch (v.error) {
    case ERROR_DIVIDE_BY_ZERO:
      printf("Error: Attempt to divide by zero.");
      break;
    case ERROR_UNKNOWN_OPERATOR:
      printf("Error: Unknown operator.");
      break;
    case ERROR_NUMBER_TOO_LARGE:
      printf("Error: Number is too large.");
      break;
  }
}

void ResultValue__println(ResultValue v) {
  ResultValue__print(v);
  putchar('\n');
}

double ResultValue__to_double(ResultValue v) {
  switch (v.type) {
    case RESULT_VALUE_INTEGER: return (double)v.integer;
    case RESULT_VALUE_DECIMAL: return v.decimal;
    default: return 0;
  }
}

long ResultValue__to_long(ResultValue v) {
  switch (v.type) {
    case RESULT_VALUE_INTEGER: return v.integer;
    case RESULT_VALUE_DECIMAL: return dtol(v.decimal);
    default: return 0;
  }
}
