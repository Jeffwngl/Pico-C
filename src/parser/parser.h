#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

// takes in a flat sequence of tokens and builds a tree structure

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    AST parse();

private:
    std::vector<Token> tokens;

    // parse grammar rule expressions
    ExprPtr parseStatement();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseFactor();
    ExprPtr parseBinary();
    ExprPtr parseUnary();
    ExprPtr parseLiteral();

    // parse grammar rule statements
    StmtPtr parseIfStatement();
    StmtPtr parseWhileStatement();
    StmtPtr parseForStatement();
    StmtPtr parseBlock();
    StmtPtr parseVarDeclaration();
    StmtPtr parseDeclaration();
};

#endif