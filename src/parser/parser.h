#ifndef PARSER_H
#define PARSER_H

#include "../ast/ast.h"
#include "../lexer/lexer.h"

#include <iostream>

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    std::unique_ptr<Program> parse();

private:
    std::vector<Token> tokens;
    int curr;

    // parse grammar rule expressions

    ExprPtr parseExpression();
    ExprPtr parseEquality();
    ExprPtr parseComparison();
    ExprPtr parseFactor();  // * / etc.
    ExprPtr parseBinary();  // a + b, x * y, left == right etc.
    ExprPtr parseUnary();   // -5, !Ok, ++x etc
    ExprPtr parsePrimary(); // "hello", true, 67, (), {} etc.
    ExprPtr parseTerm();    // + - etc.

    // parse grammar rule statements

    StmtPtr parseDeclaration();
    StmtPtr parseVarDeclaration();
    StmtPtr parseFunctionDeclaration();
    StmtPtr parseExpressionStatement();
    StmtPtr parseStatement();
    StmtPtr parseIfStatement();
    StmtPtr parseWhileStatement();
    StmtPtr parseForStatement();
    StmtPtr parseBlock();

    // helpers

    bool isEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    const Token& peek() const;
    const Token& prev() const;

    Token advance();
    Token consume(TokenType type, const std::string& message);
};

#endif