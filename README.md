# Pico-C
A tiny C compiler written in C++ 17.

## Description

While the plan was for this project to be a tiny subset of the C compiler (*Pico* C), I have continuously added new features such that the end project may be closer to the actual compiler than a small subset, this will still fall under the toy compiler category implementing some but not all of the original C11 compiler but the aim for this project is to be able to compile simple to medium large projects, e.g. git.

Order of processing:

- Preprocessor
- Lexer
- Parser
- AST
- Semantic analysis
- Code Generation

### Preprocessor

Takes in raw file as str, then handles all preprocessing elements, e.g. replacing includes with code, macro definitions, changing file contents based on `ifndef` etc.

Not implemented:
- pragma
- block comment ignore /** and /*
- #undef

### Lexer

Not implemented:
- double
- uints e.g. uint_64
- signed
- unsigned
- long
- short
- struct
- enum
- <<, >>, <<= and >>=
- break
- continue
- &=, ^=, |=, % and %=
- ~
- sizeof
- 2d arrays

### Parser

To write about.

### Codegen

To implement.

## Grammar Notation

### Statement Grammar
```
program     → declaration* EOF ;

declaration → type IDENTIFIER functionDeclaration
            | type IDENTIFIER varDeclaration
            | statement ;

type        → "int"
            | "float"
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

### Expression grammar
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
https://norasandler.com/2017/11/29/Write-a-Compiler.html  
https://craftinginterpreters.com/introduction.html  
https://github.com/rui314/chibicc  
https://www.youtube.com/watch?v=ENKT0Z3gldE  
https://en.cppreference.com/c/language  