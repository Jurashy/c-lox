//
// Created by hetto on 08.03.2026.
//


#include "Resolver.hpp"

#include <complex>

auto Resolver::visitThisExpr( This &expr) -> Value {
    if (currentClas == classType::NONE) {
        error(expr.keyword, "can't use this outside of the class"); return std::monostate{};
    }

    resolveLocal(expr, expr.keyword);

    return std::monostate{};
}

auto Resolver::visitSetExpr(Set& expr) -> Value {
    resolve(expr.value);
    resolve(expr.object);

    return std::monostate{};
}

auto Resolver::visitGetExpr(Get& expr) -> Value {
    resolve(expr.object);
    return std::monostate{};
}

auto Resolver::visitClassStmt(Class& stmt) -> Value {
    classType enclosingClass = currentClas;
    currentClas = classType::CLASS;
    declare(stmt.name);
    define(stmt.name);

    beginScope();
    scopes.front()["this"] = true;

    for (std::shared_ptr<Function>& method : stmt.methods) {
        FunctionType declaration = FunctionType::METHOD;
        resolveFunction(*method, declaration);;
    }

    endScope();

    currentClas = enclosingClass;

    return std::monostate{};
}

auto Resolver::visitUnaryExpr(Unary& expr) -> Value {
    resolve(expr.right);
    return std::monostate{};
}

auto Resolver::visitLogicalExpr(Logical& expr) -> Value {
    resolve(expr.left);
    resolve(expr.right);

    return std::monostate{};
}

auto Resolver::visitLLiteralExpr(LLiteral& expr) -> Value {
    return std::monostate{};
}

auto Resolver::visitGroupingExpr(Grouping& expr) -> Value {
    resolve(expr.expression);

    return std::monostate{};
}

auto Resolver::visitCallExpr(Call& expr) -> Value {
    for (auto& argument : expr.arguments)
        resolve(argument);

    return std::monostate{};
}

auto Resolver::visitBinaryExpr(Binary& expr) -> Value {
    resolve(expr.left);
    resolve(expr.right);

    return std::monostate{};
}

auto Resolver::visitWhileStmt(While& stmt) -> Value {
    resolve(stmt.condition);
    resolve(stmt.body);
    return std::monostate{};
}

auto Resolver::visitReturnStmt(Return& stmt) -> Value {
    if (currentFunction == FunctionType::NONE)
        error(stmt.keyword, "can't return from top level code");

    if (stmt.value != nullptr) resolve(stmt.value);

    return std::monostate{};
}

auto Resolver::visitPrintStmt(Print& stmt) -> Value {
    resolve(stmt.expression);

    return std::monostate{};
}

auto Resolver::define(const Token name)-> void {
    if (scopes.empty()) return;
    scopes.back()[name.getLexeme()] = true;
}

auto Resolver::visitIFStmt(IF& stmt) -> Value {
    resolve(stmt.condition);
    resolve(stmt.thenBranch);

    if (stmt.elseBranch != nullptr) {
        resolve(stmt.elseBranch);
    }

    return std::monostate{};
}

auto Resolver::visitExpressionStmt(Expression& stmt) -> Value {
    resolve(stmt.expression);

    return std::monostate{};
}



auto Resolver::resolveFunction(Function& function, FunctionType type) -> void {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();

    for (Token& param : function.params) {
        declare(param);
        define(param);
    }

    resolve(function.body);
    endScope();
    currentFunction = enclosingFunction;
}

auto Resolver::visitFunctionStmt(Function& stmt) -> Value {
    declare(stmt.name);
    define(stmt.name);
    resolveFunction(stmt, FunctionType::FUNCTION);

    return std::monostate{};
}

auto Resolver::visitAssignExpr(Assign& expr) -> Value {
    resolve(expr.value);
    resolveLocal(expr, expr.name);

    return std::monostate{};
}

auto Resolver::resolveLocal(Expr& expr, Token name) -> void {
    for (int i { static_cast<int>(std::ssize(scopes)) -1 }; i >= 0; i--) {
        if (scopes[i].contains(name.getLexeme())) {
            m_interpreter->resolve(&expr, std::ssize(scopes) -1 - i);
            return;
        }
    }
}

auto Resolver::visitVariableExpr( Variable& expr) -> Value {
    if (!scopes.empty() && scopes.back().contains(expr.name.getLexeme()) && scopes.back()[expr.name.getLexeme()] == false)
        error(expr.name, "Can't read local variable in its own initializer.");
    resolveLocal(expr, expr.name);
    return std::monostate{};
}

auto Resolver::declare(Token name) -> void {
    if (scopes.empty()) return;

    auto& scope = scopes.back();

    if (scope.contains(name.getLexeme())) error(name, "Already a variable with this name in this scope.");

    scope[name.getLexeme()] = false;
}

auto Resolver::visitVarStmt( Var& stmt) -> Value {
    declare(stmt.name);
    if (stmt.initializer != nullptr) resolve(stmt.initializer);

    define(stmt.name);

    return std::monostate{};
}

auto Resolver::endScope() -> void {
    scopes.pop_back();
}

auto Resolver::beginScope() -> void {
    scopes.push_back(std::map<std::string, bool>());
}

auto Resolver::resolve(sharedExpr expr) -> void {
    expr->accept(*this);
}

auto Resolver::resolve(sharedStmt stmt) -> void {
    stmt->accept(*this);
}

auto Resolver::resolve(const VectaStmt& statements) -> void {
    for (auto& statement : statements) {
        resolve(statement);
    }
}

auto Resolver::visitBlockStmt(Block& stmt) -> Value {
    beginScope();
    resolve(stmt.statements);
    endScope();

    return std::monostate{};
}