#include "parser.h"
#include "../lexer/lexer.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), curr(0)
{
}

// Starts the program, starts with the highest level
std::unique_ptr<Program> Parser::parse()
{
    auto program = std::make_unique<Program>();

    while (!isEnd())
    {
        program->statements.push_back(parseDeclaration());
    }

    return program;
};

/**
 * Helpers
 */

bool Parser::isEnd() const
{
    return peek().type == TokenType::TK_EOF;
};

const Token& Parser::peek() const
{
    return tokens[curr];
};

const Token& Parser::prev() const
{
    return tokens[curr - 1];
};

Token Parser::advance()
{
    if (!isEnd())
        return tokens[curr++];

    return prev();
};

bool Parser::check(TokenType type) const
{
    if (isEnd())
        return false;

    return peek().type == type;
};

bool Parser::match(TokenType type)
{
    if (check(type))
    {
        advance();
        return true;
    }

    return false;
};

bool Parser::match(std::initializer_list<TokenType> types)
{
    for (auto type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }

    return false;
};

bool Parser::isType(TokenType type) const
{
    return type == TokenType::KW_INT || type == TokenType::KW_FLOAT ||
           type == TokenType::KW_CHAR;
};

Token Parser::consume(TokenType type, const std::string& message)
{
    if (check(type))
        return advance();

    const Token& token = peek();

    throw std::runtime_error(message + " at line " +
                             std::to_string(token.line) + ", col " +
                             std::to_string(token.col));
};

/**
 * Parse Expressions
 */

// handles base expressions e.g. 5 + 2 * 3 -> (2 * 3) + 5
ExprPtr Parser::parseExpression()
{
    return parseAssignment();
};

// handles non declaration assignment e.g. a = 0, a += 2
ExprPtr Parser::parseAssignment()
{
    ExprPtr expr = parseEquality();

    if (match({TokenType::ASSIGN, TokenType::PLUS_EQUAL, TokenType::MINUS_EQUAL,
               TokenType::STAR_EQUAL, TokenType::SLASH_EQUAL}))
    {
        Token op = prev();
        ExprPtr value = parseAssignment();

        return std::make_unique<BinaryExpr>(std::move(op), std::move(expr),
                                            std::move(value));
    }

    return expr;
};

// handles actual equation e.g. int x = 5 <- curr + 2 * 3
ExprPtr Parser::parseEquality()
{
    ExprPtr expr = parseComparison();

    // expr1 == expr2 case or expr1 != expr2 case, stops at start of expr2
    // e.g. bool ok = expr1 == expr2;
    while (match({TokenType::EE, TokenType::NE}))
    {
        Token op = prev();
        ExprPtr r = parseComparison();

        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr),
                                            std::move(r));
    }

    return expr;
};

// handles comparison operators e.g. >=, <, >
ExprPtr Parser::parseComparison()
{
    ExprPtr expr = parseTerm();

    while (match(
        {TokenType::LESS, TokenType::GREATER, TokenType::LEQ, TokenType::GEQ}))
    {
        Token op = prev();
        ExprPtr r = parseTerm();

        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr),
                                            std::move(r));
    }

    return expr;
};

// handles terms e.g. +, -, *
ExprPtr Parser::parseTerm()
{
    ExprPtr expr = parseFactor();

    while (match({TokenType::PLUS, TokenType::MINUS}))
    {
        Token op = prev();
        ExprPtr r = parseFactor();

        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr),
                                            std::move(r));
    }

    return expr;
};

// handles factors e.g. 5 * 3, a / b
ExprPtr Parser::parseFactor()
{
    ExprPtr expr = parseUnary();

    while (match({TokenType::STAR, TokenType::SLASH}))
    {
        Token op = prev();
        ExprPtr r = parseUnary();

        expr = std::make_unique<BinaryExpr>(std::move(op), std::move(expr),
                                            std::move(r));
    }

    return expr;
};

// handles unary and calls e.g. x++, !ok, -5, printf("hello")
ExprPtr Parser::parseUnary()
{
    if (match({TokenType::BANG, TokenType::MINUS, TokenType::PLUS_PLUS,
               TokenType::MINUS_MINUS, TokenType::AMPERSAND, TokenType::STAR}))
    {
        Token op = prev();
        ExprPtr right = parseUnary();

        return std::make_unique<UnaryExpr>(std::move(op), std::move(right));
    }

    return parsePostfix();
};

// handles post fix operations e.g. x++
ExprPtr Parser::parsePostfix()
{
    ExprPtr expr = parseCall();

    while (match({TokenType::PLUS_PLUS, TokenType::MINUS_MINUS}))
    {
        Token op = prev();

        expr = std::make_unique<PostFixExpr>(std::move(op), std::move(expr));
    }

    return expr;
};

// handles function calls e.g. printf("hello")
ExprPtr Parser::parseCall()
{
    ExprPtr expr = parsePrimary();

    while (true)
    {
        if (match(TokenType::OPEN_PAREN))
        {
            std::vector<ExprPtr> arguments;

            if (!check(TokenType::CLOSE_PAREN))
            {
                arguments.push_back(parseExpression());
                while (match(TokenType::COMMA))
                {
                    arguments.push_back(parseExpression());
                }
            }

            consume(TokenType::CLOSE_PAREN, "Expected ')' after arguments");
            expr = std::make_unique<CallExpr>(std::move(expr),
                                              std::move(arguments));
        }
        else
            break;
    }

    return expr;
}

// handles primaries e.g. 5, 3.14, 67, "six_seven", either int, float or string
ExprPtr Parser::parsePrimary()
{
    if (match({TokenType::VAL_INT, TokenType::VAL_FLOAT, TokenType::VAL_STRING,
               TokenType::VAL_CHAR}))
        return std::make_unique<PrimaryExpr>(prev());

    if (match(TokenType::IDENTIFIER))
        return std::make_unique<IdentifierExpr>(prev());

    if (match(TokenType::OPEN_PAREN))
    {
        ExprPtr expr = parseExpression();
        consume(TokenType::CLOSE_PAREN, "Expected ')' after expression.");
        return expr;
    }

    throw std::runtime_error("Expected expression at line " +
                             std::to_string(peek().line) + ", col " +
                             std::to_string(peek().col));
};

/**
 * Parse Statements
 */

// handles high level type declarations e.g. int, string, char
StmtPtr Parser::parseDeclaration()
{
    if (match({TokenType::KW_INT, TokenType::KW_FLOAT, TokenType::KW_CHAR}))
    {
        Token type = prev();

        int pointerDepth = 0;

        while (match(TokenType::STAR))
        {
            pointerDepth++;
        }

        Token name =
            consume(TokenType::IDENTIFIER, "Expected name after type.");

        if (check(TokenType::OPEN_PAREN))
        {
            return parseFunctionDeclaration(std::move(type), std::move(name));
        }

        if (check(TokenType::OPEN_BRACKET))
        {
            return parseArrDeclaration(std::move(type), std::move(name));
        }

        return parseVarDeclaration(std::move(type), std::move(name),
                                   std::move(pointerDepth));
    }

    return parseStatement();
};

// handles array declarations e.g. char str[20] = {'h ', ...} or "this_string"
StmtPtr Parser::parseArrDeclaration(Token type, Token name)
{
    std::vector<ExprPtr> value;
    ExprPtr size = nullptr;

    consume(TokenType::OPEN_BRACKET, "Expected '[' after array name");

    // if size is an identifier, this is determined at compile time
    if (!check(TokenType::CLOSE_BRACKET))
    {
        size = parseExpression();
    }

    consume(TokenType::CLOSE_BRACKET, "Expected ']' after array size");

    if (match(TokenType::ASSIGN))
    {
        if (match(TokenType::VAL_STRING))
        {
            // turn string to char and append to value vec
            Token stringToken = prev();

            std::string text = stringToken.val;

            if (text.size() >= 2 && text.front() == '"' && text.back() == '"')
            {
                text = text.substr(1, text.size() - 2);
            }

            for (auto& c : text)
            {
                Token token{TokenType::VAL_CHAR, std::string(1, c),
                            stringToken.line, stringToken.col};
                value.push_back(std::make_unique<PrimaryExpr>(token));
            }

            Token nullToken{TokenType::VAL_CHAR, "\\0", stringToken.line,
                            stringToken.col};

            value.push_back(std::make_unique<PrimaryExpr>(nullToken));
        }
        else if (match(TokenType::OPEN_BRACE))
        {
            if (!check(TokenType::CLOSE_BRACE))
            {

                value.push_back(parseExpression());

                while (match(TokenType::COMMA))
                {
                    if (check(TokenType::CLOSE_BRACE))
                        break;

                    value.push_back(parseExpression());
                }
            }

            consume(TokenType::CLOSE_BRACE,
                    "Expected '}' after array value initialization");
        }
        else
        {
            throw std::runtime_error("Expected string literal or '{' after "
                                     "array assignment at line " +
                                     std::to_string(peek().line) + ", col " +
                                     std::to_string(peek().col));
        }
    }

    if (size)
    {
        const auto* primary = dynamic_cast<const PrimaryExpr*>(size.get());

        if (primary && primary->value.type == TokenType::VAL_INT)
        {
            int declaredSize = std::stoi(primary->value.val);

            if (value.size() > static_cast<size_t>(declaredSize))
            {
                throw std::runtime_error(
                    "Excess elements in array initializer.");
            }

            // if not up to size, initialize rest to 0
            while (value.size() < static_cast<size_t>(declaredSize))
            {
                Token zeroToken{TokenType::VAL_INT, "0", name.line, name.col};

                value.push_back(std::make_unique<PrimaryExpr>(zeroToken));
            }
        }
    }
    else if (!value.empty())
    {
        Token sizeToken{TokenType::VAL_INT, std::to_string(value.size()),
                        name.line, name.col};

        size = std::make_unique<PrimaryExpr>(sizeToken);
    }

    consume(TokenType::SEMICOLON, "Expected ';' after array declaration");

    return std::make_unique<ArrDecStmt>(std::move(type), std::move(name),
                                        std::move(size), std::move(value));
};

// handles variable declarations e.g. int x = 5 + 3;
StmtPtr Parser::parseVarDeclaration(Token type, Token name, int pointerDepth)
{
    ExprPtr value = nullptr;

    if (match(TokenType::ASSIGN))
    {
        value = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");

    return std::make_unique<VarDecStmt>(std::move(type), std::move(name),
                                        std::move(value),
                                        std::move(pointerDepth));
};

// handles function declarations e.g. int main() {}
StmtPtr Parser::parseFunctionDeclaration(Token type, Token name)
{
    consume(TokenType::OPEN_PAREN, "Expected '(' after function name.");

    std::vector<Param> params;

    if (!check(TokenType::CLOSE_PAREN))
    {
        params.push_back(parseParam());

        while (match(TokenType::COMMA))
        {
            params.push_back(parseParam());
        }
    }

    consume(TokenType::CLOSE_PAREN, "Expected ')' after function parameters.");

    consume(TokenType::OPEN_BRACE, "Expected '{' before function body.");

    StmtPtr body = parseBlock();

    return std::make_unique<FunctionStmt>(std::move(type), std::move(name),
                                          std::move(params), std::move(body));
};

// handles function parameters e.g. int a, int b
Param Parser::parseParam()
{
    if (!isType(peek().type))
    {
        throw std::runtime_error("Expected parameter type at line " +
                                 std::to_string(peek().line) + ", col " +
                                 std::to_string(peek().col));
    }

    Token type = advance();

    Token name = consume(TokenType::IDENTIFIER, "Expected parameter name.");

    return Param(std::move(type), std::move(name));
};

// handles high level statements e.g. while(...), if(...)
StmtPtr Parser::parseStatement()
{
    if (match(TokenType::IF))
        return parseIfStatement();

    if (match(TokenType::WHILE))
        return parseWhileStatement();

    if (match(TokenType::FOR))
        return parseForStatement();

    if (match(TokenType::OPEN_BRACE))
        return parseBlock();

    if (match(TokenType::RETURN))
        return parseReturnStatement();

    return parseExpressionStatement();
};

// high level handler for expression that is used as a statement e.g. x++;
StmtPtr Parser::parseExpressionStatement()
{
    ExprPtr expr = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after expression");

    return std::make_unique<ExprStmt>(std::move(expr));
}

// handles if statement
StmtPtr Parser::parseIfStatement()
{
    consume(TokenType::OPEN_PAREN, "Expected '(' after 'if'.");

    ExprPtr condition = parseExpression();

    consume(TokenType::CLOSE_PAREN, "Expected ')' after if condition.");

    StmtPtr thenBranch = parseStatement();

    StmtPtr elseBranch = nullptr;

    if (match(TokenType::ELSE))
    {
        // consume(TokenType::OPEN_BRACE, "Expected '{' after else");
        elseBranch = parseStatement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch),
                                    std::move(elseBranch));
};

// handles while statement
StmtPtr Parser::parseWhileStatement()
{
    consume(TokenType::OPEN_PAREN, "Expected '(' after 'while'");

    ExprPtr condition = parseExpression();

    consume(TokenType::CLOSE_PAREN, "Expected ')' after while condition");

    StmtPtr body = parseStatement();

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
};

// handles for statement
StmtPtr Parser::parseForStatement()
{
    consume(TokenType::OPEN_PAREN, "Expected '(' after 'for'");

    StmtPtr initializer = nullptr;

    if (match(TokenType::SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (match(
                 {TokenType::KW_CHAR, TokenType::KW_INT, TokenType::KW_FLOAT}))
    {

        Token type = prev();
        Token name = consume(TokenType::IDENTIFIER,
                             "Expected variable name in for initializer");
        int pointerDepth = 0;

        while (match(TokenType::STAR))
        {
            pointerDepth++;
        }
        initializer = parseVarDeclaration(type, name, pointerDepth);
    }
    else
    {
        initializer = parseExpressionStatement();
    }

    ExprPtr condition = nullptr;

    if (!check(TokenType::SEMICOLON))
        condition = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after condition");

    ExprPtr increment = nullptr;

    if (!check(TokenType::CLOSE_PAREN))
        increment = parseExpression();

    consume(TokenType::CLOSE_PAREN, "Expected ')' after for condition");

    StmtPtr body = parseStatement();

    return std::make_unique<ForStmt>(std::move(initializer),
                                     std::move(condition), std::move(increment),
                                     std::move(body));
};

StmtPtr Parser::parseBlock()
{
    std::vector<StmtPtr> statements;

    while (!isEnd() && !check(TokenType::CLOSE_BRACE))
    {
        statements.push_back(parseDeclaration());
    }

    consume(TokenType::CLOSE_BRACE, "Expected '}'");

    return std::make_unique<BlockStmt>(std::move(statements));
};

StmtPtr Parser::parseReturnStatement()
{
    Token keyword = prev();

    ExprPtr value = nullptr;

    if (!check(TokenType::SEMICOLON))
    {
        value = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after return value");

    return std::make_unique<ReturnStmt>(std::move(keyword), std::move(value));
}