#pragma once
#include "AstPrinter.hpp"
#include "RuntimeError.hpp"

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <string_view>
#include <chrono>

#include "Environment.hpp"
#include "Lox.hpp"
#include "../output/Stmt.hpp"
#include "../output/Expr.hpp"
//#include "LoxCallable.hpp"
#include "LoxFunction.hpp"
#include "NativeClock.hpp"
#include "Return__.hpp"

/// interpreting function calls 10.1.2
///
///Checking arity // page 152
///

struct LoxFunction;

struct Interpreter :  ExprVisitor, StmtVisitor
{
    std::shared_ptr<Environment<Value>> globals = std::make_shared<Environment<Value>>(nullptr);
    Interpreter() {
        auto val = std::make_shared<NativeClock>();

        globals->define("clock", val);
    }

    auto visitReturnStmt(Return& stmt) -> Value override {
        Value value = std::monostate{};

        if (stmt.value != nullptr) value = evaluate(stmt.value);
        throw Return__(value);
    }

    auto visitFunctionStmt(Function& stmt) -> Value override {
        auto funcDecl = std::make_shared<Function>(stmt); // capture closure
        auto func = std::make_shared<LoxFunction>(funcDecl);

        std::shared_ptr<LoxCallable> callableFunc = std::static_pointer_cast<LoxCallable>(func);
        environment->define(stmt.name.getLexeme(), callableFunc);              // stored as Value automatically

        return std::monostate{};
    }

    auto getGlobals() -> std::shared_ptr<Environment<Value>> {
        return globals;
    }
    auto visitCallExpr(Call& expr) -> Value override {
        auto callee = evaluate(expr.callee);

        std::vector<Value> arguments;

        for (auto& argument : expr.arguments) {
            arguments.push_back(evaluate(argument));
        }

        // check callable
        if (!std::holds_alternative<std::shared_ptr<LoxCallable>>(callee)) {
            throw RuntimeError(expr.paren, "can call only classes and functions");
        }

        // extract callable safely
        auto callable_func = std::get<std::shared_ptr<LoxCallable>>(callee);

        // arity check
        if (std::ssize(arguments) != callable_func->arity()) {
            throw RuntimeError(
                expr.paren,
                std::string("expected ")
                    + std::to_string(callable_func->arity())
                    + " arguments but got "
                    + std::to_string(std::ssize(arguments))
                    + "."
            );
        }

        return callable_func->call(*this, arguments);
    }

    auto visitWhileStmt(While &stmt) -> Value override {
        while (isTruthy(evaluate(stmt.condition))) execute(*stmt.body);

        return {};
    }
    auto visitLogicalExpr(Logical &expr) -> Value override {
        auto left = evaluate(expr.left);

        if (expr.oper.getType() == TokenType::OR) {
            if (isTruthy(left)) return left;
        } else {
            if (!isTruthy(left)) return left;
        }

        return evaluate(expr.right);
    }
    auto visitIFStmt(IF& stmt) -> Value override {
        if (isTruthy(evaluate(stmt.condition))) execute(*stmt.thenBranch);
        else if (stmt.elseBranch != nullptr) execute(*stmt.elseBranch);

        return {};
    }
    auto executeBlock(std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment<Value>> newEnv) -> void {
        auto previous = environment;

        try {
            environment = newEnv;
            for (auto& statement : statements) {
                execute(*statement);
            }
        } catch (...) {
            environment = previous;
            throw;
        }

        environment = previous;
    }

    auto visitBlockStmt(Block& stmt) -> Value override {
        auto newEnv = std::make_shared<Environment<Value>>(environment);
        executeBlock(stmt.statements, newEnv);

        return {};
    }
    auto visitAssignExpr(Assign& expr) -> Value override {
        Value value = evaluate(expr.value);
        environment->assign(expr.name, value);

        return  value;
    }
    Value visitVariableExpr(Variable& expr) override {
        return environment->get(expr.name);
    }

    auto visitVarStmt(Var& stmt) -> Value override {
       Value value= stmt.initializer ? evaluate(stmt.initializer) : std::monostate{};

        environment->define(stmt.name.getLexeme(), value);

        return {};
    }
    auto visitExpressionStmt(Expression &stmt) -> Value override {
        evaluate(stmt.expression);
        return {};
    }
    auto visitPrintStmt(Print &stmt) -> Value override {
        Value value = evaluate(stmt.expression);
        std::cout << stringify(value) << '\n';
        return {};
    }

    void interpret(std::vector<std::shared_ptr<Stmt>>& statements) {
        try {
            for (auto& stmt : statements) {
                execute(*stmt);
            }
        }
        catch (RuntimeError& e) {
            runtime_error(e);
        }
    }

    Value visitLLiteralExpr(LLiteral& expr) override { return expr.value; }

    Value visitGroupingExpr(Grouping& expr) override { return evaluate(expr.expression); }

    Value visitUnaryExpr(Unary& expr) override
    {
        auto right = evaluate(expr.right);

        switch (expr.my_operator.getType())
        {
            case TokenType::BANG:
                return !isTruthy(right);
            case TokenType::MINUS:
                checkNumberOperand(expr.my_operator, right);
                return -std::any_cast<double>(right);
            default:
                throw RuntimeError(expr.my_operator, "Unhandled unary operator.");
        }

        // unreachable
        return std::monostate{};
    }

    Value visitBinaryExpr(Binary& expr) override
    {
        auto left = evaluate(expr.left);
        auto right = evaluate(expr.right);

        switch (expr.my_operator.getType())
        {
            case TokenType::BANG_EQUAL: return !isEqual(left, right);
            case TokenType::EQUAL_EQUAL: return isEqual(left, right);
            case TokenType::GREATER:
                checkNumberOperands(expr.my_operator, left, right);
                return std::get<double>(left) > std::get<double>(right);//
            case TokenType::GREATER_EQUAL:
                checkNumberOperands(expr.my_operator, left, right);
                return std::get<double>(left) >= std::get<double>(right);
            case TokenType::LESS:
                checkNumberOperands(expr.my_operator, left, right);
                return std::get<double>(left) < std::get<double>(right);
            case TokenType::LESS_EQUAL:
                checkNumberOperands(expr.my_operator, left, right);
                return std::get<double>(left) <= std::get<double>(right);
            case TokenType::MINUS:
                return std::get<double>(left) - std::get<double>(right);
            case TokenType::PLUS:
                if  (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                    return std::get<double>(left) + std::get<double>(right);
                if  (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                    return std::get<std::string>(left) + std::get<std::string>(right);
                throw RuntimeError(expr.my_operator, "operators must be numbers or two strings");//103 hooking up the interpreter
            case TokenType::SLASH:
                checkNumberOperands(expr.my_operator, left, right);
                if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                    return std::get<double>(left) / std::get<double>(right);
            case TokenType::STAR:
                return std::get<double>(left) * std::get<double>(right);
            default:
                throw RuntimeError(expr.my_operator, "Unhandled unary operator.");
        }

        return std::monostate{};
    }
    ~Interpreter() = default;

private:
    std::shared_ptr<Environment<Value>> environment = globals;
    void execute(Stmt& stmt) {
        stmt.accept(*this);
    }
    auto stringify(const Value& value) -> std::string {
        return std::visit([](auto&& v) -> std::string {
            using TA = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<TA, std::monostate>) return "nil";
            else if constexpr (std::is_same_v<TA, double>) return std::to_string(v);
            else if constexpr (std::is_same_v<TA, bool>) return v ? "true" : "false";
            else if constexpr (std::is_same_v<TA, std::string>) return v;
            else if constexpr (std::is_same_v<TA, CallablePtr>) return "<fn>";
            else if constexpr (std::is_same_v<TA, std::shared_ptr<Function>>) return "<function>";
            else if constexpr (std::is_same_v<TA, std::nullptr_t>) return "null";
        }, value);
    }

    auto checkNumberOperands(Token oper, Value left, Value right) -> void
    {
        if  (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) return;
        throw RuntimeError(oper, "operands must be numbers.");
    }
    auto checkNumberOperand(Token oper, Value operand) -> void
    {
        if (std::holds_alternative<double>(operand)) return;
        throw RuntimeError(oper, "operand must be a number");
    }

    Value evaluate(const std::shared_ptr<Expr>& expr) { return expr->accept(*this); }
    
    auto isTruthy(Value object) -> bool
    {
        return std::visit([](auto&& v) -> bool {
            using TA = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<TA, std::monostate>)
                return false;
            else if constexpr (std::is_same_v<TA, bool>)
                return v;
            else return true;
        }, object);
    }

    auto isEqual(Value a, Value b) -> bool
    {
        return a == b;
    }
};


