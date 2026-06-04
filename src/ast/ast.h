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

struct ExprVisitor
{
    // overriden by class which implements the visit function, e.g. print,
    // compile
    virtual std::string visitLiteralExpr(const LiteralExpr& expr) = 0;
    virtual std::string visitBinaryExpr(const BinaryExpr& expr) = 0;
    virtual std::string visitUnaryExpr(const UnaryExpr& expr) = 0;
    virtual std::string visitIdentifierExpr(const IdentifierExpr& expr) = 0;
    virtual std::string visitPostFixExpr(const PostFixExpr& expr) = 0;

    virtual ~ExprVisitor() = default;
};

/**
 * Define destructors through base class using virtual destructor
 */

struct AstNode
{
    virtual ~AstNode() = default;
};

// expression: produces values
struct Expr : AstNode
{
    // pure virtual functions, all concrete functions must implement
    // overridden by all expression implementations, e.g. literal expr
    virtual std::string accept(ExprVisitor& visitor) const = 0;
    virtual ~Expr() = default;
};

// statement: performs actions
struct Stmt : AstNode
{
    virtual ~Stmt() = default;
};

// raw value written in code, e.g. "this is a string"
struct LiteralExpr : Expr
{
    Token value;
    explicit LiteralExpr(Token token) : value(std::move(token)) {}

    std::string accept(ExprVisitor& visitor) const override
    {
        // passes in the current LiteralExpr object
        return visitor.visitLiteralExpr(*this);
    }
};
// explicit stops silent conversions, we have to convert it ourselves
// move avoids creating copy when possible, also needed for unique_ptr

// identifier variable or name, e.g. x = 5, name = x
struct IdentifierExpr : Expr
{
    Token name;
    explicit IdentifierExpr(Token token) : name(std::move(token)) {}

    std::string accept(ExprVisitor& visitor) const override
    {
        // passes in the current object into the defined function, e.g. print
        // print implements visitIdentifierExpr and accept returns the result
        // of prints implementation of visitIdentifierExpr
        return visitor.visitIdentifierExpr(*this);
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

    std::string accept(ExprVisitor& visitor) const override
    {
        return visitor.visitBinaryExpr(*this);
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

    std::string accept(ExprVisitor& visitor) const override
    {
        return visitor.visitUnaryExpr(*this);
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

    std::string accept(ExprVisitor& visitor) const override
    {
        return visitor.visitPostFixExpr(*this);
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

// TODO: See if using dynamic cast might be better than creating new struct for
// every return type
class AstPrinter : public ExprVisitor
{
public:
    std::string print(const Expr& expr)
    {
        // implement dynamic cast here
        return expr.accept(*this);
    }

    std::string visitLiteralExpr(const LiteralExpr& expr) override
    {
        return expr.value.val;
    }

    std::string visitIdentifierExpr(const IdentifierExpr& expr) override
    {
        return expr.name.val;
    }

private:
    std::string parenthesize(const std::string& name, const Expr& expr)
    {
        return '(' + name + ' ' + print(expr) + ')';
    }
};

#endif