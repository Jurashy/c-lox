//
// Created by hetto on 08.03.2026.
//

#ifndef LOX_RESOLVER_HPP
#define LOX_RESOLVER_HPP

#include "Lox.hpp"
#include "Value.hpp"
#include "Interpreter.hpp"
#include "../output/Stmt.hpp"
#include "../output/Expr.hpp"

#include <map>
#include <memory>
#include <vector>
#include <string>
#include <variant>

struct Interpreter;

using VectaStmt = std::vector<std::shared_ptr<Stmt>>;
using sharedStmt = std::shared_ptr<Stmt>;
using sharedExpr = std::shared_ptr<Expr>;

class Resolver : public StmtVisitor, public ExprVisitor {
public:
    auto resolve(const VectaStmt& statements) -> void;
    auto visitBlockStmt(Block& stmt) -> Value override;

    Resolver(std::shared_ptr<Interpreter> interpreter)
        : m_interpreter { interpreter } {}

private:
    std::shared_ptr<Interpreter> m_interpreter;
    std::vector<std::map<std::string, bool>> scopes;

    enum class FunctionType {
        NONE,
        FUNCTION
    };

    FunctionType currentFunction = FunctionType::NONE;

    auto resolve(sharedStmt stmt) -> void;
    auto resolve(sharedExpr expr) -> void;
    auto beginScope() -> void;
    auto endScope() -> void;

    auto visitVarStmt( Var& stmt) -> Value override;

    auto declare(Token name) -> void;

    auto define(const Token name) -> void;

    auto visitVariableExpr( Variable& expr) -> Value override;

    auto resolveLocal(Expr& expr, Token name) -> void;

    auto visitAssignExpr(Assign& expr) -> Value override;

    auto visitFunctionStmt(Function& stmt) -> Value override;

    auto resolveFunction(Function& function, FunctionType type);

    auto visitExpressionStmt(Expression& stmt) -> Value override;

    auto visitIFStmt(IF& stmt) -> Value override;

    auto visitPrintStmt(Print& stmt) -> Value override;

    auto visitReturnStmt(Return& stmt) -> Value override;

    auto visitWhileStmt(While& stmt) -> Value override;

    auto visitBinaryExpr(Binary& expr) -> Value override;

    auto visitCallExpr(Call& expr) -> Value override;

    auto visitGroupingExpr(Grouping& expr) -> Value override;

    auto visitLLiteralExpr(LLiteral& expr) -> Value override;

    auto visitLogicalExpr(Logical& expr) -> Value override;

    auto visitUnaryExpr(Unary& expr) -> Value override;

    auto visitClassStmt(Class& stmt) -> Value override;
};

#endif //LOX_RESOLVER_HPP