## Ash

Ash is a toy language made for the book "Build your own LISP" by Daniel Holden.

Features from book:
- REPL
- Basic math operators (+, -, *, /)

Added features:
- Modular codebase
- Modulus operator (%) for both integers and floating-point
- Textual operator aliases (add, subtract, multiply, divide, modulo)
- Double-precision floating-point numbers
- Generic "number" type
- Automatic type coercion between integers and floating-point numbers

Problems that prevent this from being practical:
- No Unicode support
- S-expressions are variable arrays instead of linked-lists/cons cells
