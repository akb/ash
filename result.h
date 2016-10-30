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
  RESULT_VALUE_SYMBOL,
  RESULT_VALUE_S_EXPRESSION,
  RESULT_VALUE_INTEGER,
  RESULT_VALUE_DECIMAL,
  RESULT_VALUE_ERROR
};

typedef struct ResultValueStruct {
  int    type;
  long   integer;
  double decimal;
  char*  error;
  char*  symbol;
  int    count;
  struct ResultValueStruct** cell;
} ResultValue;

/**
 * new__IntegerResultValue
 * Constructs a new ResultValue struct containing a long integer
 */
ResultValue* new__IntegerResultValue(long x);

/**
 * new__ErrorResultValue
 * Constructs a new ResultValue struct containing an error
 */
ResultValue* new__ErrorResultValue(char* error);

/**
 * new__DecimalResultValue
 * Constructs a new ResultValue struct containing a double precision floating
 * point number
 */
ResultValue* new__DecimalResultValue(double x);

/**
 * new__SymbolResultValue
 * Constructs a new ResultValue struct containing a symbol
 */
ResultValue* new__SymbolResultValue(char* symbol);

/**
 * new__SExpressionResultValue
 * Constructs a new ResultValue struct containing an empty S-Expression
 */
ResultValue* new__SExpressionResultValue(void);

/**
 * ResultValue__free
 * Frees any memory allocated to the passed-in ResultValue, deleting the
 * ResultValue
 */
void ResultValue__free(ResultValue* v);

/**
 * ResultValue__print
 * Prints the passed-in ResultValue to STDOUT
 */
void ResultValue__print(ResultValue* v);

/**
 * IntegerResultValue__print
 * Prints the passed-in ResultValue containing an integer to STDOUT
 */
void IntegerResultValue__print(ResultValue* v);

/**
 * DecimalResultValue__print
 * Prints the passed-in ResultValue containing a decimal number to STDOUT
 */
void DecimalResultValue__print(ResultValue* v);

/**
 * ErrorResultValue__print
 * Prints the passed-in ResultValue containing an error to STDOUT
 */
void ErrorResultValue__print(ResultValue* v);

/**
 * SymbolResultValue__print
 * Prints the passed-in ResultValue containing a symbol to STDOUT
 */
void SymbolResultValue__print(ResultValue* v);

/**
 * SExpressionResultValue__print
 * Prints the passed-in ResultValue containing an s-expression to STDOUT
 */
void SExpressionResultValue__print(ResultValue* v, char open, char close);

/**
 * ResultValue__println
 * Prints the passed-in ResultValue to STDOUT followed by a newline
 */
void ResultValue__println(ResultValue* v);

/**
 */
double ResultValue__to_double(ResultValue* v);

/**
 */
long ResultValue__to_long(ResultValue* v);
#endif
