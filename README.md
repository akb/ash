## Ash

Ash is a toy programming language written while reading the book
"Build your own LISP" by Daniel Holden.

Features from book:
- REPL
- Basic math operators (+, -, *, /)
- S-Expressions
- Q-Expressions
- Builtin functions for Q-Expressions (list, head, tail, join, evaluate)
- Values may be bound to symbols using 'define' builtin
- Functions

Added features:
- Modular codebase
- Functional test suite (written in [Chicken Scheme](https://www.call-cc.org/))
- Modulus operator (%) for both integers and floating-point
- Textual operator aliases (add, subtract, multiply, divide, modulo)
- Double-precision floating-point numbers
- Generic "number" type
- Automatic type coercion between integers and floating-point numbers
- "construct" (cons) builtin
- "length" (len) builtin
- "initial" builtin
- Command-line argument parsing
- "-s" command-line flag for disabling informational output
- "exit" builtin w/ status code

Problems that prevent this from being practical:
- No Unicode support
- S-expressions are variable-length arrays instead of linked-lists/cons cells
- No conditionals
- No loops/recursion
- Unnecessary allocations (arrays and should use buffer pools)