## Ash

Ash is a toy language made for the book "Build your own LISP" by Daniel Holden.

Features from book:
- REPL
- Basic math operators (+, -, *, /)
- S-Expressions
- Q-Expressions
- Builtin functions for Q-Expressions (list, head, tail, join, evaluate)

Added features:
- Modular codebase
- Modulus operator (%) for both integers and floating-point
- Textual operator aliases (add, subtract, multiply, divide, modulo)
- Double-precision floating-point numbers
- Generic "number" type
- Automatic type coercion between integers and floating-point numbers
- "construct" (cons) builtin
- "length" (len) builtin
- "initial" builtin

Problems that prevent this from being practical:
- No Unicode support
- S-expressions are variable arrays instead of linked-lists/cons cells
- No user-defined symbols
- No conditionals
- No loops/recursion
