#ifndef AST_H
#define AST_H

#include <memory>
#include <utility>
#include <vector>

#include "../lexer/lexer.h"

typedef std::unique_ptr<Expr> ExprPtr;
typedef std::unique_ptr<Stmt> StmtPtr;

/**
 * Define destructors through base class using virtual destructor
 */

struct ASTNode
{
    ~ASTNode() = default;
};

// expression: produces values
struct Expr : ASTNode
{
    ~Expr() = default;
};

// statement: performs actions
struct Stmt : ASTNode
{
    ~Stmt() = default;
};

// raw value written in code, e.g. "this is a string"
struct LiteralExpr : Expr
{
    Token value;
    explicit LiteralExpr(Token token) : value(std::move(token)) {}
};
// explicit stops silent conversions, we have to convert it ourselves
// move avoids creating copy when possible, also needed for unique_ptr

// identifier variable or name, e.g. x = 5, name = x
struct IdentifierExpr : Expr
{
    Token name;
    explicit IdentifierExpr(Token token) : name(std::move(token)) {}
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

// variable declaration, e.g. let x = 5, name = x, value = 5
struct VarDecStmt : Stmt
{
    Token name;
    ExprPtr value;
    explicit VarDecStmt(Token token, ExprPtr value)
        : name(std::move(token)), value(std::move(value))
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
    ExprPtr start;
    ExprPtr end;
    ExprPtr range;
    StmtPtr step;
    explicit ForStmt(ExprPtr start, ExprPtr end, ExprPtr range, StmtPtr step)
        : start(std::move(start)), end(std::move(end)), range(std::move(range)),
          step(std::move(step))
    {
    }
};

class AST
{
public:
private:
    ASTNode root;
};

#endif