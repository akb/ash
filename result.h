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
  RESULT_VALUE_Q_EXPRESSION,
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
ResultValue* new__IntegerResultValue(long);

/**
 * new__ErrorResultValue
 * Constructs a new ResultValue struct containing an error
 */
ResultValue* new__ErrorResultValue(char*);

/**
 * new__DecimalResultValue
 * Constructs a new ResultValue struct containing a double precision floating
 * point number
 */
ResultValue* new__DecimalResultValue(double);

/**
 * new__SymbolResultValue
 * Constructs a new ResultValue struct containing a symbol
 */
ResultValue* new__SymbolResultValue(char*);

/**
 * new__SExpressionResultValue
 * Constructs a new ResultValue struct containing an empty S-Expression
 */
ResultValue* new__SExpressionResultValue(void);

/**
 * new__QExpressionResultValue
 * Constructs a new ResultValue struct containing an empty Q-Expression
 */
ResultValue* new__QExpressionResultValue(void);


/**
 * ResultValue__free
 * Frees any memory allocated to the passed-in ResultValue, deleting the
 * ResultValue
 */
void ResultValue__free(ResultValue*);

/**
 * ResultValue__print
 * Prints the passed-in ResultValue to STDOUT
 */
void ResultValue__print(ResultValue*);

/**
 * IntegerResultValue__print
 * Prints the passed-in ResultValue containing an integer to STDOUT
 */
void IntegerResultValue__print(ResultValue*);

/**
 * DecimalResultValue__print
 * Prints the passed-in ResultValue containing a decimal number to STDOUT
 */
void DecimalResultValue__print(ResultValue*);

/**
 * ErrorResultValue__print
 * Prints the passed-in ResultValue containing an error to STDOUT
 */
void ErrorResultValue__print(ResultValue*);

/**
 * SymbolResultValue__print
 * Prints the passed-in ResultValue containing a symbol to STDOUT
 */
void SymbolResultValue__print(ResultValue*);

/**
 * ExpressionResultValue__print
 * Prints the passed-in ResultValue containing an expression to STDOUT
 */
void ExpressionResultValue__print(ResultValue*, char, char);

/**
 * ResultValue__println
 * Prints the passed-in ResultValue to STDOUT followed by a newline
 */
void ResultValue__println(ResultValue*);

/**
 */
double ResultValue__to_double(ResultValue*);

/**
 */
long ResultValue__to_long(ResultValue*);
#endif
