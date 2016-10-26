#ifndef RESULT_VALUE_TYPE_DEFINED
#define RESULT_VALUE_TYPE_DEFINED
/**
 * RESULT_VALUE_INTEGER
 * Indicates that the ResultValue contains an integer
 *
 * RESULT_VALUE_DECIMAL
 * Indicates that the ResultValue contains a decimal number
 *
 * RESULT_VALUE_ERROR
 * Indicates that the ResultValue contains an error
 */
enum {
  RESULT_VALUE_INTEGER,
  RESULT_VALUE_DECIMAL,
  RESULT_VALUE_ERROR
};

/**
 * ERROR_DIVIDE_BY_ZERO,
 * Indicates that the error ResultValue was caused by an attempt to evaluate a
 * division by zero
 *
 * ERROR_UNKNOWN_OPERATOR,
 * Indicates that the error ResultValue was caused by the use of an undefined
 * operator
 *
 * ERROR_NUMBER_TOO_LARGE
 * Indicates that the error ResultValue was caused by using a number greater
 * than what can be held by a long integer
 */
enum {
  ERROR_DIVIDE_BY_ZERO,
  ERROR_UNKNOWN_OPERATOR,
  ERROR_NUMBER_TOO_LARGE,
  ERROR_UNKNOWN_RESULT_TYPE
};

typedef struct ResultValueTag {
  int    type;
  long   integer;
  double decimal;
  int    error;
} ResultValue;

/**
 * new__IntegerResultValue
 * Constructs a new ResultValue struct containing a long integer
 */
ResultValue new__IntegerResultValue(long x);

/**
 * new__ErrorResultValue
 * Constructs a new ResultValue struct containing an error
 */
ResultValue new__ErrorResultValue(int e);

/**
 * new__DecimalResultValue
 * Constructs a new ResultValue struct containing a double precision floating
 * point number
 */
ResultValue new__DecimalResultValue(double x);

/**
 * ResultValue__print
 * Prints the passed-in ResultValue to STDOUT
 */
void ResultValue__print(ResultValue v);

/**
 * IntegerResultValue__print
 * Prints the passed-in ResultValue containing an integer to STDOUT
 */
void IntegerResultValue__print(ResultValue v);

/**
 * DecimalResultValue__print
 * Prints the passed-in ResultValue containing a decimal number to STDOUT
 */
void DecimalResultValue__print(ResultValue v);

/**
 * ErrorResultValue__print
 * Prints the passed-in ResultValue containing an error to STDOUT
 */
void ErrorResultValue__print(ResultValue v);

/**
 * ResultValue__println
 * Prints the passed-in ResultValue to STDOUT followed by a newline
 */
void ResultValue__println(ResultValue v);

/**
 */
double ResultValue__to_double(ResultValue v);

/**
 */
long ResultValue__to_long(ResultValue v);
#endif
