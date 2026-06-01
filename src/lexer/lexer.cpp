#include "lexer.h"

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"int", TokenType::KW_INT},    {"float", TokenType::KW_FLOAT},
    {"string", TokenType::KW_STR}, {"return", TokenType::RETURN},
    {";", TokenType::SEMICOLON},   {"main", TokenType::MAIN},
    {"const", TokenType::CONST},   {"printf", TokenType::PRINTF},
};

Lexer::Lexer(const std::string& src, const std::string& filename)
    : src{src}, filename{filename}, start{0}, current{0}, line{1}, col{1} {};

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
    start = current;

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

    // braces
    switch (c)
    {
        case '(':
            return makeToken(TokenType::OPEN_PAREN);
        case ')':
            return makeToken(TokenType::CLOSE_PAREN);
        case '{':
            return makeToken(TokenType::OPEN_BRACE);
        case '}':
            return makeToken(TokenType::CLOSE_PAREN);
        case ';':
            return makeToken(TokenType::SEMICOLON);
        case '=':
            return makeToken(match('=') ? TokenType::EE : TokenType::ASSIGN);
        case '!':
            return makeToken(match('=') ? TokenType::NE : TokenType::BANG);
        case '<':
            return makeToken(match('=') ? TokenType::LEQ : TokenType::LESS);
        case '>':
            return makeToken(match('=') ? TokenType::GEQ : TokenType::GREATER);
        case '+':
            if (match('+'))
                return makeToken(TokenType::PLUS_PLUS);
            if (match('='))
                return makeToken(TokenType::PLUS_EQUAL);
            return makeToken(TokenType::PLUS);
        case '-':
            if (match('-'))
                return makeToken(TokenType::MINUS_MINUS);
            if (match('='))
                return makeToken(TokenType::MINUS_EQUAL);
            return makeToken(TokenType::MINUS);

        default:
            return errToken("Not a viable token.");
    }
};

Token Lexer::makeToken(TokenType type)
{
    return Token{type, src.substr(start, current - start), line, col};
};

Token Lexer::errToken(std::string message)
{
    return Token{TokenType::ERROR, message, line, col};
};

bool Lexer::match(char expected)
{
    if (isEnd())
        return false;
    if (src[current] == expected)
        return true;

    return false;
};

char Lexer::peek() const
{
    if (isEnd())
        return '\0';

    return src[current];
};

char Lexer::peekNext() const
{
    if (current + 1 >= src.size())
        return '\0';

    return src[current + 1];
};

bool Lexer::isEnd() const
{
    if (current >= src.size())
        return true;

    return false;
};

char Lexer::advance()
{
    // post increment, c is at current - 1
    char c = src[current++];

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

    std::string text = src.substr(start, current - start);

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
    // advances current to end of whitespace including comments
    while (!isEnd())
    {
        char c = peek();

        // whitespace
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            advance();
            continue;
        }

        // line comment
        if (c == '/' && peekNext() == '/')
        {
            while (!isEnd() && peek() != '\n')
            {
                advance();
            }
            continue;
        }

        break;
    }
};