#ifndef AST_H
#define AST_H

#include <memory>
#include <utility>
#include <vector>

#include "../lexer/lexer.h"

// TODO: Move this to a generator to generate ast.h, metaprogram the trees

/**
 * AST nodes here only store the data of the node, no operations, operations
 * are defined separately as functions which take in a node to avoid the
 * expression problem.
 */

struct Expr;
struct Stmt;

struct LiteralExpr;
struct IdentifierExpr;
struct BinaryExpr;
struct UnaryExpr;
struct PostFixExpr;

typedef std::unique_ptr<Expr> ExprPtr;
typedef std::unique_ptr<Stmt> StmtPtr;

/**
 * Define destructors through base class using virtual destructor
 */

struct AstNode
{
    virtual ~AstNode() = default;
};

struct Param
{
    Token type;
    Token name;

    explicit Param(Token type, Token name)
        : type(std::move(type)), name(std::move(name))
    {
    }
};

struct Program : AstNode
{
    std::vector<StmtPtr> statements;
};

// expression: produces values
struct Expr : AstNode
{
    // pure virtual functions, all concrete functions must implement
    // overridden by all expression implementations, e.g. literal expr
    virtual ~Expr() = default;
};

// statement: performs actions
struct Stmt : AstNode
{
    virtual ~Stmt() = default;
};

// raw value written in code, e.g. "this is a string"
struct PrimaryExpr : Expr
{
    Token value;
    explicit PrimaryExpr(Token token) : value(std::move(token))
    {
    }
};
// explicit stops silent conversions, we have to convert it ourselves
// move avoids creating copy when possible, also needed for unique_ptr

// identifier variable or name, e.g. x = 5, name = x
struct IdentifierExpr : Expr
{
    Token name;
    explicit IdentifierExpr(Token token) : name(std::move(token))
    {
    }
};

// expression with two sides and an operator, e.g. 4 + 2
struct BinaryExpr : Expr
{
    Token op;
    ExprPtr l;
    ExprPtr r;
    explicit BinaryExpr(Token token, ExprPtr left, ExprPtr right)
        : op(std::move(token)), l(std::move(left)), r(std::move(right))
    {
    }
};

// expression with one operand and operator, e.g. --x or -(x + 4)
struct UnaryExpr : Expr
{
    Token op;
    ExprPtr r;
    explicit UnaryExpr(Token token, ExprPtr right)
        : op(std::move(token)), r(std::move(right))
    {
    }
};

// e.g. x++, x--;
struct PostFixExpr : Expr
{
    Token op;
    ExprPtr l;
    explicit PostFixExpr(Token token, ExprPtr left)
        : op(std::move(token)), l(std::move(left))
    {
    }
};

// function calls
struct CallExpr : Expr
{
    ExprPtr called;
    std::vector<ExprPtr> args;
    explicit CallExpr(ExprPtr callee, std::vector<ExprPtr> arguments)
        : called(std::move(callee)), args(std::move(arguments))
    {
    }
};

// variable declaration, e.g. let x = 5, name = x, value = 5
struct VarDecStmt : Stmt
{
    Token type;
    Token name;
    ExprPtr value;
    explicit VarDecStmt(Token type, Token token, ExprPtr value)
        : type(std::move(type)), name(std::move(token)), value(std::move(value))
    {
    }
};

struct IfStmt : Stmt
{
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
    explicit IfStmt(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch))
    {
    }
};

struct WhileStmt : Stmt
{
    ExprPtr condition;
    StmtPtr body;
    explicit WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)), body(std::move(body))
    {
    }
};

struct ForStmt : Stmt
{
    StmtPtr initializer;
    ExprPtr condition;
    ExprPtr increment;
    StmtPtr body;
    explicit ForStmt(StmtPtr initializer, ExprPtr condition, ExprPtr increment,
                     StmtPtr body)
        : initializer(std::move(initializer)), condition(std::move(condition)),
          increment(std::move(increment)), body(std::move(body))
    {
    }
};

// high level block of code within a statement
struct BlockStmt : Stmt
{
    std::vector<StmtPtr> statements;
    explicit BlockStmt(std::vector<StmtPtr> statements)
        : statements(std::move(statements))
    {
    }
};

struct FunctionStmt : Stmt
{
    Token returnType;
    Token name;
    std::vector<Param> params; // TODO: make params a struct
    StmtPtr body;

    explicit FunctionStmt(Token returnType, Token name,
                          std::vector<Param> params, StmtPtr body)
        : returnType(std::move(returnType)), name(std::move(name)),
          params(std::move(params)), body(std::move(body))
    {
    }
};

struct ExprStmt : Stmt
{
    ExprPtr expr;
    explicit ExprStmt(ExprPtr expr) : expr(std::move(expr))
    {
    }
};

struct ReturnStmt : Stmt
{
    Token keyword;
    ExprPtr value;
    ReturnStmt(Token keyword, ExprPtr value)
        : keyword(std::move(keyword)), value(std::move(value))
    {
    }
};

#endif