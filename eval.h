#include "result.h"
#include "mpc.h"

ResultValue* ResultValue__eval(ResultValue*);
ResultValue* ResultValue__eval_s_expression(ResultValue*);
ResultValue* ResultValue__pop(ResultValue*, int);
ResultValue* ResultValue__take(ResultValue*, int);
void NumberResultValue__negate_mutate(ResultValue*);
void NumberResultValue__add_mutate(ResultValue*, ResultValue*);
void NumberResultValue__subtract_mutate(ResultValue*, ResultValue*);
void NumberResultValue__multiply_mutate(ResultValue*, ResultValue*);
void NumberResultValue__divide_mutate(ResultValue*, ResultValue*);
ResultValue* builtin_op(ResultValue*, char*);
