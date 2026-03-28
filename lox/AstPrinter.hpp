#pragma once
#include "../output/Expr.hpp"
#include "Token.hpp"

#include <any>

#include "../output/Stmt.hpp"

struct AstPrinter : public ExprVisitor, StmtVisitor
{
    auto visitExpressionStmt(Expression& stmt) -> Value override;

    auto visitPrintStmt(Print& stmt) -> Value override;
    auto print(Expr& expr) -> std::string;

    auto visitBinaryExpr(Binary& expr) -> Value override;

    auto
    visitGroupingExpr(Grouping& expr) -> Value override;

    auto visitLLiteralExpr(LLiteral& expr) -> Value override;

    auto
    visitUnaryExpr(Unary& expr) -> Value override;

private:
    template <class... Exprs>
    auto parenthesize(std::string name, Exprs&... exprs) -> std::string;
};
