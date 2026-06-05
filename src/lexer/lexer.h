#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>

enum class TokenType
{
    /**
     * Keywords
     * Keyword type e.g. string or float
     */
    KW_INT,   // integer keyword
    KW_FLOAT, // float keyword
    KW_STR,   // string keyword
    KW_CHAR,  // char keyword

    /**
     * Value
     * Actual value of the type, e.g. "hello" or 5.0
     */
    VAL_FLOAT,
    VAL_STRING,
    VAL_CHAR,
    VAL_INT,

    /**
     * Operators
     */
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /
    ASSIGN,      // =
    GREATER,     // >
    LESS,        // <
    BANG,        // !
    EE,          // ==
    NE,          // !=
    GEQ,         // >=
    LEQ,         // <=
    PLUS_EQUAL,  // +=
    MINUS_EQUAL, // -=
    PLUS_PLUS,   // ++
    MINUS_MINUS, // --
    STAR_EQUAL,  // *=
    SLASH_EQUAL, // /=

    /**
     * Other
     */
    IDENTIFIER,  // variable name
    OPEN_PAREN,  // \(
    CLOSE_PAREN, // \)
    OPEN_BRACE,  // {
    CLOSE_BRACE, // }
    RETURN,      // return
    VAR,         // variable
    SEMICOLON,   // ;
    TK_EOF,      // end of file
    ERROR,       // error
    CONST,       // const
    COMMA,       // ,

    /**
     * Statements
     */
    IF,    // if(...)
    WHILE, // while(...)
    FOR,   // for(...)
    ELSE,  // else(...)
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
    Token scanChar();
    Token scanNum();
    Token scanIdentifierOrKeyword();
    void skip();

    // Utils
    void skipWhitespace();

    // State
    std::string src;
    std::string filename;
    size_t start;
    size_t curr;
    int line;
    int col;

    // Keyword table
    static const std::unordered_map<std::string, TokenType> keywords;
};

#endif