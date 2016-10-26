#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "parser.h"
#include "eval.h"
#include "errors.h"

ResultValue* ResultValue__eval_s_expression(ResultValue* v) {
  for (int i = 0; i < v->count; i++)
    v->cell[i] = ResultValue__eval(v->cell[i]);

  for (int i = 0; i < v->count; i++)
    if (v->cell[i]->type == RESULT_VALUE_ERROR)
      return ResultValue__take(v, i);

  if (v->count == 0) return v;
  if (v->count == 1) return ResultValue__take(v, 0);

  ResultValue* f = ResultValue__pop(v, 0);
  if (f->type != RESULT_VALUE_SYMBOL) {
    ResultValue__free(f);
    ResultValue__free(v);
    return new__ErrorResultValue("S-Expression does not begin with a symbol.");
  }

  ResultValue* result = builtin_op(v, f->symbol);
  ResultValue__free(f);
  return result;
}

ResultValue* ResultValue__eval(ResultValue* v) {
  if (v->type == RESULT_VALUE_S_EXPRESSION)
    return ResultValue__eval_s_expression(v);
  else
    return v;
}

ResultValue* ResultValue__pop(ResultValue* v, int i) {
  ResultValue* x = v->cell[i];
  memmove(&v->cell[i], &v->cell[i+1], sizeof(ResultValue*) * (v->count-i-1));
  v->count--;
  v->cell = realloc(v->cell, sizeof(ResultValue*) * v->count);
  return x;
}

ResultValue* ResultValue__take(ResultValue* v, int i) {
  ResultValue* x = ResultValue__pop(v, i);
  ResultValue__free(v);
  return x;
}

void NumberResultValue__negate_mutate(ResultValue* v) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      v->integer = -v->integer;
      return;
    case RESULT_VALUE_DECIMAL:
      v->decimal = -v->decimal;
      return;
  }
}

void NumberResultValue__add_mutate(ResultValue* v, ResultValue* x) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      v->integer += x->integer;
      return;
    case RESULT_VALUE_DECIMAL:
      v->decimal += x->decimal;
      return;
  }
}

void NumberResultValue__subtract_mutate(ResultValue* v, ResultValue* x) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      v->integer -= x->integer;
      return;
    case RESULT_VALUE_DECIMAL:
      v->decimal -= x->decimal;
      return;
  }
}

void NumberResultValue__multiply_mutate(ResultValue* v, ResultValue* x) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      v->integer *= x->integer;
      return;
    case RESULT_VALUE_DECIMAL:
      v->decimal *= x->decimal;
      return;
  }
}

void NumberResultValue__divide_mutate(ResultValue* v, ResultValue* x) {
  switch (v->type) {
    case RESULT_VALUE_INTEGER:
      v->integer /= x->integer;
      return;
    case RESULT_VALUE_DECIMAL:
      v->decimal /= x->decimal;
      return;
  }
}

ResultValue* builtin_op(ResultValue* a, char* op) {
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != RESULT_VALUE_INTEGER &&
        a->cell[i]->type != RESULT_VALUE_DECIMAL) {
      ResultValue__free(a);
      return new__ErrorResultValue("Cannot operate on non-number.");
    }
  }

  ResultValue* x = ResultValue__pop(a, 0);

  if ((strcmp(op, "-") || strcmp(op, "subtract")) && a->count == 0) {
    NumberResultValue__negate_mutate(x);
  }

  while (a->count > 0) {
    ResultValue* y = ResultValue__pop(a, 0);
    if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0)
      NumberResultValue__add_mutate(x, y);
    if (strcmp(op, "-") == 0 || strcmp(op, "subtract") == 0)
      NumberResultValue__subtract_mutate(x, y);
    if (strcmp(op, "*") == 0 || strcmp(op, "multiply") == 0)
      NumberResultValue__multiply_mutate(x, y);
    if (strcmp(op, "/") == 0 || strcmp(op, "divide") == 0) {
      if (y->type == RESULT_VALUE_INTEGER && y->integer == 0) {
        ResultValue__free(x);
        ResultValue__free(y);
        x = new__ErrorResultValue("Cannot divide by zero.");
        break;
      } else if (y->type == RESULT_VALUE_DECIMAL && y->decimal == 0.0) {
        ResultValue__free(x);
        ResultValue__free(y);
        x = new__ErrorResultValue("Cannot divide by zero.");
        break;
      } else {
        NumberResultValue__divide_mutate(x, y);
      }
    }

    ResultValue__free(y);
  }

  ResultValue__free(a);
  return x;
}
