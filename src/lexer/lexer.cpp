#include "lexer.h"

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"int", TokenType::KW_INT},     {"float", TokenType::KW_FLOAT},
    {"char", TokenType::KW_CHAR},   {"return", TokenType::RETURN},
    {";", TokenType::SEMICOLON},    {"const", TokenType::CONST},
    {"for", TokenType::FOR},        {"while", TokenType::WHILE},
    {"if", TokenType::IF},          {"else", TokenType::ELSE},
    {"include", TokenType::INCLUDE}};

Lexer::Lexer(const std::string& src, const std::string& filename)
    : src(src), filename(filename), start(0), curr(0), line(1), col(1) {};

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (true)
    {
        Token nxt = nextToken();
        tokens.push_back(nxt);
        if (nxt.type == TokenType::TK_EOF)
            break;
    }

    return tokens;
}

// sifts through src string to find the next string which makes the next token
Token Lexer::nextToken()
{
    skipWhitespace();
    start = curr;

    if (isEnd())
        return makeToken(TokenType::TK_EOF);

    char c = advance();

    // scan for identifiers and keywords
    if (std::isalpha(c))
        return scanIdentifierOrKeyword();

    // scan numbers
    if (std::isdigit(c))
        return scanNum();

    // scan strings
    if (c == '"')
        return scanString();

    // scan chars
    if (c == '\'')
    {
        return scanChar();
    }

    // braces and other
    switch (c)
    {
        case '(':
            return makeToken(TokenType::OPEN_PAREN);
        case ')':
            return makeToken(TokenType::CLOSE_PAREN);
        case '{':
            return makeToken(TokenType::OPEN_BRACE);
        case '}':
            return makeToken(TokenType::CLOSE_BRACE);
        case '[':
            return makeToken(TokenType::OPEN_BRACKET);
        case ']':
            return makeToken(TokenType::CLOSE_BRACKET);
        case ';':
            return makeToken(TokenType::SEMICOLON);
        case ',':
            return makeToken(TokenType::COMMA);
        case '#':
            return makeToken(TokenType::HASH);
        case '=':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::EE);
            }
            return makeToken(TokenType::ASSIGN);
        case '!':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::NE);
            }
            return makeToken(TokenType::BANG);
        case '<':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::LEQ);
            }
            return makeToken(TokenType::LESS);
        case '>':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::GEQ);
            }
            return makeToken(TokenType::GREATER);
        case '*':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::STAR_EQUAL);
            }
            return makeToken(TokenType::STAR);
        case '/':
            if (match('='))
            {
                advance();
                return makeToken(TokenType::SLASH_EQUAL);
            }
            return makeToken(TokenType::SLASH);
        case '&':
            if (match('&'))
            {
                advance();
                return makeToken(TokenType::AND);
            }
            return makeToken(TokenType::AMPERSAND);
        case '|':
            if (match('|'))
            {
                advance();
                return makeToken(TokenType::OR);
            }
            return makeToken(TokenType::BIT_OR);
        case '+':
            if (match('+'))
            {
                advance();
                return makeToken(TokenType::PLUS_PLUS);
            }
            if (match('='))
            {
                advance();
                return makeToken(TokenType::PLUS_EQUAL);
            }
            return makeToken(TokenType::PLUS);
        case '-':
            if (match('-'))
            {
                advance();
                return makeToken(TokenType::MINUS_MINUS);
            }
            if (match('='))
            {
                advance();
                return makeToken(TokenType::MINUS_EQUAL);
            }
            return makeToken(TokenType::MINUS);

        default:
            return errToken("Not a viable token.");
    }
};

Token Lexer::makeToken(TokenType type)
{
    return Token{type, src.substr(start, curr - start), line, col};
};

Token Lexer::errToken(std::string message)
{
    return Token{TokenType::ERROR, message, line, col};
};

bool Lexer::match(char expected)
{
    if (isEnd())
        return false;
    if (src[curr] == expected)
        return true;

    return false;
};

char Lexer::peek() const
{
    if (isEnd())
        return '\0';

    return src[curr];
};

char Lexer::peekNext() const
{
    if (curr + 1 >= src.size())
        return '\0';

    return src[curr + 1];
};

bool Lexer::isEnd() const
{
    if (curr >= src.size())
        return true;

    return false;
};

char Lexer::advance()
{
    // post increment, c is at curr - 1
    char c = src[curr++];

    if (c == '\n')
    {
        line++;
        col = 1;
    }
    else
    {
        col++;
    }

    return c;
};

Token Lexer::scanIdentifierOrKeyword()
{
    while (!isEnd() && std::isalnum(peek()) || peek() == '_')
    {
        advance();
    }

    std::string text = src.substr(start, curr - start);

    auto it = keywords.find(text);

    if (it == keywords.end())
        return makeToken(TokenType::IDENTIFIER);

    return makeToken(it->second);
};

Token Lexer::scanString()
{
    while (!isEnd() && peek() != '"')
    {
        advance();
    }

    advance();
    return makeToken(TokenType::VAL_STRING);
};

// TODO: change this to smthing better
Token Lexer::scanChar()
{
    while (!isEnd() && peek() != '\'')
    {
        advance();
    }

    advance();
    return makeToken(TokenType::VAL_CHAR);
};

Token Lexer::scanNum()
{
    while (!isEnd() && std::isdigit(peek()))
    {
        advance();
    }

    if (peek() == '.' && std::isdigit(peekNext()))
    {
        advance();

        while (!isEnd() && std::isdigit(peek()))
        {
            advance();
        }
        return makeToken(TokenType::VAL_FLOAT);
    }

    return makeToken(TokenType::VAL_INT);
};

void Lexer::skipWhitespace()
{
    // advances curr to end of whitespace including comments
    while (!isEnd())
    {
        char c = peek();

        // whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            advance();
            continue;
        }

        break;
    }
};