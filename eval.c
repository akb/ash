#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "eval.h"

ResultValue eval(mpc_ast_t* t) {
  if (strstr(t->tag, "integer")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE
      ? new__IntegerResultValue(x)
      : new__ErrorResultValue(ERROR_NUMBER_TOO_LARGE);
  }

  if (strstr(t->tag, "decimal")) {
    errno = 0;
    // TODO: handle other parse errors which would be returned via pointer
    // passed-in as the second argument instead of NULL (see: man strtod)
    double x = strtod(t->contents, NULL);
    return errno != ERANGE
      ? new__DecimalResultValue(x)
      : new__ErrorResultValue(ERROR_NUMBER_TOO_LARGE);
  }

  char* op = t->children[1]->contents;
  ResultValue x = eval(t->children[2]);

  int i = 3;
  while (strstr(t->children[i]->tag, "expression")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

ResultValue eval_op(ResultValue x, char* op, ResultValue y) {
  if (x.type == RESULT_VALUE_ERROR) { return x; }
  if (y.type == RESULT_VALUE_ERROR) { return y; }

  int type = RESULT_VALUE_INTEGER;
  if (x.type == RESULT_VALUE_DECIMAL || y.type == RESULT_VALUE_DECIMAL) {
    type = RESULT_VALUE_DECIMAL;
  }

  if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0) {
    return eval_op_add(type, x, y);
  }
  if (strcmp(op, "-") == 0 || strcmp(op, "subtract") == 0) {
    return eval_op_subtract(type, x, y);
  }
  if (strcmp(op, "*") == 0 || strcmp(op, "multiply") == 0) {
    return eval_op_multiply(type, x, y);
  }
  if (strcmp(op, "/") == 0 || strcmp(op, "divide") == 0) {
    return eval_op_divide(type, x, y);
  }
  if (strcmp(op, "%") == 0 || strcmp(op, "modulo") == 0) {
    return eval_op_modulo(type, x, y);
  }

  return new__ErrorResultValue(ERROR_UNKNOWN_OPERATOR);
}

ResultValue eval_op_add(int type, ResultValue x, ResultValue y) {
  switch (type) {
    case RESULT_VALUE_DECIMAL:
      return new__DecimalResultValue(
        ResultValue__to_double(x) + ResultValue__to_double(y)
      );
    case RESULT_VALUE_INTEGER:
      return new__IntegerResultValue(
        ResultValue__to_long(x) + ResultValue__to_long(y)
      );
    default:
      return new__ErrorResultValue(ERROR_UNKNOWN_RESULT_TYPE);
  }
}

ResultValue eval_op_subtract(int type, ResultValue x, ResultValue y) {
  switch (type) {
    case RESULT_VALUE_DECIMAL:
      return new__DecimalResultValue(
        ResultValue__to_double(x) - ResultValue__to_double(y)
      );
    case RESULT_VALUE_INTEGER:
      return new__IntegerResultValue(
        ResultValue__to_long(x) - ResultValue__to_long(y)
      );
    default:
      return new__ErrorResultValue(ERROR_UNKNOWN_RESULT_TYPE);
  }
}

ResultValue eval_op_multiply(int type, ResultValue x, ResultValue y) {
  switch (type) {
    case RESULT_VALUE_DECIMAL:
      return new__DecimalResultValue(
        ResultValue__to_double(x) * ResultValue__to_double(y)
      );
    case RESULT_VALUE_INTEGER:
      return new__IntegerResultValue(
        ResultValue__to_long(x) * ResultValue__to_long(y)
      );
    default:
      return new__ErrorResultValue(ERROR_UNKNOWN_RESULT_TYPE);
  }
}

ResultValue eval_op_divide(int type, ResultValue xval, ResultValue yval) {
  double dx, dy;
  long lx, ly;
  switch (type) {
    case RESULT_VALUE_DECIMAL:
      dx = ResultValue__to_double(xval);
      dy = ResultValue__to_double(yval);
      return dy == 0.0
        ? new__ErrorResultValue(ERROR_DIVIDE_BY_ZERO)
        : new__DecimalResultValue(dx / dy);
    case RESULT_VALUE_INTEGER:
      lx = ResultValue__to_long(xval);
      ly = ResultValue__to_long(yval);
      return ly == 0
        ? new__ErrorResultValue(ERROR_DIVIDE_BY_ZERO)
        : new__IntegerResultValue(lx / ly);
    default:
      return new__ErrorResultValue(ERROR_UNKNOWN_RESULT_TYPE);
  }
}

ResultValue eval_op_modulo(int type, ResultValue xval, ResultValue yval) {
  double dx, dy;
  long lx, ly;
  switch (type) {
    case RESULT_VALUE_DECIMAL:
      dx = ResultValue__to_double(xval);
      dy = ResultValue__to_double(yval);
      return dy == 0.0
        ? new__ErrorResultValue(ERROR_DIVIDE_BY_ZERO)
        : new__DecimalResultValue(fmod(dx, dy));
    case RESULT_VALUE_INTEGER:
      lx = ResultValue__to_long(xval);
      ly = ResultValue__to_long(yval);
      return ly == 0
        ? new__ErrorResultValue(ERROR_DIVIDE_BY_ZERO)
        : new__IntegerResultValue(lx % ly);
    default:
      return new__ErrorResultValue(ERROR_UNKNOWN_RESULT_TYPE);
  }
}

