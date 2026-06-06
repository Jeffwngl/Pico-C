#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include <iostream>

void printExpr(const Expr* expr);
void printStmt(const Stmt* stmt);

void printExpr(const Expr* expr)
{
    if (expr == nullptr)
    {
        std::cout << "null";
        return;
    }

    if (const auto* primary = dynamic_cast<const PrimaryExpr*>(expr))
    {
        std::cout << "Primary(" << primary->value.val << ")";
    }
    else if (const auto* ident = dynamic_cast<const IdentifierExpr*>(expr))
    {
        std::cout << "Identifier(" << ident->name.val << ")";
    }
    else if (const auto* binary = dynamic_cast<const BinaryExpr*>(expr))
    {
        std::cout << "Binary(";
        printExpr(binary->l.get());
        std::cout << " " << binary->op.val << " ";
        printExpr(binary->r.get());
        std::cout << ")";
    }
    else if (const auto* unary = dynamic_cast<const UnaryExpr*>(expr))
    {
        std::cout << "Unary(" << unary->op.val << " ";
        printExpr(unary->r.get());
        std::cout << ")";
    }
    else if (const auto* postfix = dynamic_cast<const PostFixExpr*>(expr))
    {
        std::cout << "Postfix(";
        printExpr(postfix->l.get());
        std::cout << postfix->op.val << ")";
    }
    else if (const auto* call = dynamic_cast<const CallExpr*>(expr))
    {
        std::cout << "Call(";

        printExpr(call->called.get());

        std::cout << "(";

        for (size_t i = 0; i < call->args.size(); i++)
        {
            if (i > 0)
                std::cout << ", ";

            printExpr(call->args[i].get());
        }

        std::cout << "))";
    }
    else
    {
        std::cout << "UnknownExpr";
    }
}

void printStmt(const Stmt* stmt)
{
    if (stmt == nullptr)
    {
        std::cout << "null";
        return;
    }

    if (const auto* var = dynamic_cast<const VarDecStmt*>(stmt))
    {
        std::cout << "VarDec(" << var->name.val;

        if (var->value)
        {
            std::cout << " = ";
            printExpr(var->value.get());
        }

        std::cout << ")";
    }
    else if (const auto* exprStmt = dynamic_cast<const ExprStmt*>(stmt))
    {
        std::cout << "ExprStmt(";
        printExpr(exprStmt->expr.get());
        std::cout << ")";
    }
    else if (const auto* ifStmt = dynamic_cast<const IfStmt*>(stmt))
    {
        std::cout << "If(";
        printExpr(ifStmt->condition.get());

        std::cout << ", then = ";
        printStmt(ifStmt->thenBranch.get());

        if (ifStmt->elseBranch)
        {
            std::cout << ", else = ";
            printStmt(ifStmt->elseBranch.get());
        }

        std::cout << ")";
    }
    else if (const auto* whileStmt = dynamic_cast<const WhileStmt*>(stmt))
    {
        std::cout << "While(";
        printExpr(whileStmt->condition.get());
        std::cout << ", body = ";
        printStmt(whileStmt->body.get());
        std::cout << ")";
    }
    else if (const auto* forStmt = dynamic_cast<const ForStmt*>(stmt))
    {
        std::cout << "For(";

        std::cout << "init = ";
        printStmt(forStmt->initializer.get());

        std::cout << ", condition = ";
        printExpr(forStmt->condition.get());

        std::cout << ", increment = ";
        printExpr(forStmt->increment.get());

        std::cout << ", body = ";
        printStmt(forStmt->body.get());

        std::cout << ")";
    }
    else if (const auto* block = dynamic_cast<const BlockStmt*>(stmt))
    {
        std::cout << "Block{\n";

        for (const auto& statement : block->statements)
        {
            std::cout << "  ";
            printStmt(statement.get());
            std::cout << "\n";
        }

        std::cout << "}";
    }
    else if (const auto* func = dynamic_cast<const FunctionStmt*>(stmt))
    {
        std::cout << "Function(";
        std::cout << func->returnType.val << " ";
        std::cout << func->name.val;
        std::cout << "(";

        for (size_t i = 0; i < func->params.size(); i++)
        {
            if (i > 0)
                std::cout << ", ";

            std::cout << func->params[i].type.val << ' '
                      << func->params[i].name.val;
        }

        std::cout << ") ";

        printStmt(func->body.get());

        std::cout << ")";
    }
    else if (const auto* ret = dynamic_cast<const ReturnStmt*>(stmt))
    {
        std::cout << "Return(";

        if (ret->value)
        {
            printExpr(ret->value.get());
        }

        std::cout << ")";
    }
    else if (const auto* arr = dynamic_cast<const ArrDecStmt*>(stmt))
    {
        std::cout << "ArrDec(";
        std::cout << arr->type.val << " ";
        std::cout << arr->name.val;
        std::cout << "[";

        if (arr->size)
        {
            printExpr(arr->size.get());
        }

        std::cout << "]";

        if (!arr->value.empty())
        {
            std::cout << " = {";

            for (size_t i = 0; i < arr->value.size(); i++)
            {
                if (i > 0)
                {
                    std::cout << ", ";
                }

                printExpr(arr->value[i].get());
            }

            std::cout << "}";
        }

        std::cout << ")";
    }
    else
    {
        std::cout << "UnknownStmt";
    }
}