#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens) {}

/**
 * The parser uses recursive descent parsing
 */

AST Parser::parse()
{
    for (auto& const t : tokens)
    {
        //
    }
}

//
ExprPtr Parser::parseStatement() {

};