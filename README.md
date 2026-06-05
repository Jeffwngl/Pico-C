# Pico-C
A tiny C compiler written in C++ 17

## Grammar Notation

### Statement Grammar
```
program     → declaration* EOF ;

declaration → type IDENTIFIER functionDeclaration
            | type IDENTIFIER varDeclaration
            | statement ;

type        → "int"
            | "float"
            | "string"
            | "char" ;

functionDeclaration
            → "(" parameters? ")" block ;

parameters  → parameter ( "," parameter )* ;

parameter   → type IDENTIFIER ;

varDeclaration
            → ( "=" expression )? ";" ;

statement   → ifStatement
            | whileStatement
            | forStatement
            | block
            | returnStatement
            | expressionStatement ;

block       → declaration* "}" ;

ifStatement → "if" "(" expression ")" statement
              ( "else" statement )? ;

whileStatement
            → "while" "(" expression ")" statement ;

forStatement
            → "for" "(" forInitializer? ";" forCondition? forIncrement? ")" statement ;

forInitializer
            → type declaration ;

forCondition
            → IDENTIFIER expression ;

forIncrement
            → IDENTIFIER expression ;

returnStatement
            → "return" expression? ";" ;

expressionStatement
            → expression ";" ;
```

### Expression grammar:
```
expression  → assignment ;

assignment  → equality ;

equality    → comparison ( ( "==" | "!=" ) comparison )* ;

comparison  → term ( ( "<" | ">" | "<=" | ">=" ) term )* ;

term        → factor ( ( "+" | "-" ) factor )* ;

factor      → unary ( ( "*" | "/" ) unary )* ;

unary       → post fix ;

post fix    → call ;

call        → primary ( "(" arguments? ")" )* ;

arguments   → expression ( "," expression )* ;

primary     → INT_LITERAL
            | FLOAT_LITERAL
            | STRING_LITERAL
            | CHAR_LITERAL
            | IDENTIFIER
            | "(" expression ")" ;
```

## Resources Used

http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf
norasandler.com/2017/11/29/Write-a-Compiler.html
https://craftinginterpreters.com/introduction.html
https://github.com/rui314/chibicc
https://www.youtube.com/watch?v=ENKT0Z3gldE