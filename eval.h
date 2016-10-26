#include "result.h"
#include "mpc.h"

/**
 * eval
 * evalutes an AST
 */
ResultValue eval(mpc_ast_t* t);

/**
 * eval_op
 * Performs the operation on the given operands
 */
ResultValue eval_op(ResultValue x, char* op, ResultValue y);

ResultValue eval_op_add(int type, ResultValue x, ResultValue y);
ResultValue eval_op_subtract(int type, ResultValue x, ResultValue y);
ResultValue eval_op_multiply(int type, ResultValue x, ResultValue y);
ResultValue eval_op_divide(int type, ResultValue x, ResultValue y);
ResultValue eval_op_modulo(int type, ResultValue x, ResultValue y);
