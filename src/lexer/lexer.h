#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType
{
    /**
     * Keywords
     */
    KW_INT,   // integer keyword
    KW_FLOAT, // float keyword
    KW_STR,   // string keyword
    /**
     * Tokens
     */
    KEYWORD,     // keyword value
    INTEGER,     // integer value
    FLOAT,       // float value
    STRING,      // string value
    IDENTIFIER,  // variable name
    OPEN_PAREN,  // \(
    CLOSE_PAREN, // \)
    OPEN_BRACE,  // {
    CLOSE_BRACE, // }
    RETURN,      // return
    VAR,         // variable
    OPERATOR,    // operator
    SEMICOLON,   // ;
    TK_EOF,      // end of file
    ERROR,       // error
};

struct Token
{
    TokenType type;  // token type
    std::string val; // token content
    int line;        // line of token (for errors)
    int col;         // column of token (for errors)
};

class Lexer
{
public:
    Lexer(const std::string& src, const std::string& filename = "input");
    std::vector<Token> tokenize();

private:
    // Character primitives
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    bool isEnd() const;

    // Token scanning
    Token nextToken();
    Token makeToken(TokenType type);
    Token errToken(std::string message);
    Token scanString();
    Token scanNum();

    // Utils
    void skipWhitespace();

    Token scanIdentifierOrKeyword();
    // Token scanString();
    Token scanNumber();
    void skip();

    // State
    std::string src;
    std::string filename;
    size_t start;
    size_t current;
    int line;
    int col;

    // kw table
    static const std::unordered_map<std::string, TokenType> keywords;
};

#endif