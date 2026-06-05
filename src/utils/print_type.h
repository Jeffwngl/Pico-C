#include <iostream>
#include <string>
#include <vector>

#include "../ast/ast.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
        case TokenType::KW_INT:
            return "KW_INT";
        case TokenType::KW_FLOAT:
            return "KW_FLOAT";
        case TokenType::KW_STR:
            return "KW_STR";
        case TokenType::KW_CHAR:
            return "KW_CHAR";

        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::STAR:
            return "STAR";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::ASSIGN:
            return "ASSIGN";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::LESS:
            return "LESS";
        case TokenType::BANG:
            return "BANG";
        case TokenType::EE:
            return "EE";
        case TokenType::NE:
            return "NE";
        case TokenType::GEQ:
            return "GEQ";
        case TokenType::LEQ:
            return "LEQ";
        case TokenType::PLUS_EQUAL:
            return "PLUS_EQUAL";
        case TokenType::MINUS_EQUAL:
            return "MINUS_EQUAL";
        case TokenType::PLUS_PLUS:
            return "PLUS_PLUS";
        case TokenType::MINUS_MINUS:
            return "MINUS_MINUS";
        case TokenType::COMMA:
            return "COMMA";

        case TokenType::FOR:
            return "FOR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::IF:
            return "IF";

        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::OPEN_PAREN:
            return "OPEN_PAREN";
        case TokenType::CLOSE_PAREN:
            return "CLOSE_PAREN";
        case TokenType::OPEN_BRACE:
            return "OPEN_BRACE";
        case TokenType::CLOSE_BRACE:
            return "CLOSE_BRACE";
        case TokenType::RETURN:
            return "RETURN";
        case TokenType::VAR:
            return "VAR";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::TK_EOF:
            return "TK_EOF";
        case TokenType::ERROR:
            return "ERROR";
        // case TokenType::MAIN:
        //     return "MAIN";
        case TokenType::CONST:
            return "CONST";

        case TokenType::VAL_FLOAT:
            return "VAL_FLOAT";
        case TokenType::VAL_STRING:
            return "VAL_STRING";
        case TokenType::VAL_INT:
            return "VAL_INT";
        case TokenType::VAL_CHAR:
            return "VAL_CHAR";

        default:
            return "UNKNOWN_TOKEN";
    }
}