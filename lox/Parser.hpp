#pragma once
#include "Parser.hpp"
#include "TokenType.hpp"
#include "Token.hpp"
#include "../output/Expr.hpp"
#include "../output/Stmt.hpp"

#include <vector>
#include <type_traits>
#include <concepts>
#include <array>

struct Parser
{
    Parser(std::vector<Token> tokens) : tokens { tokens } {}
    auto parse() -> std::vector<std::shared_ptr<Stmt>>;

private:
    auto returnStatement() -> std::shared_ptr<Stmt>;
    auto __function__(const std::string& kind) -> std::shared_ptr<Stmt>;
    auto finishCall(std::unique_ptr<Expr>& callee) -> std::unique_ptr<Expr>;
    auto call() -> std::unique_ptr<Expr>;
    auto forStatement() -> std::shared_ptr<Stmt>;
    auto whileStatement() -> std::shared_ptr<Stmt>;
    auto logic_and() -> std::unique_ptr<Expr>;
    auto logic_or() -> std::unique_ptr<Expr>;
    auto ifStatement() -> std::shared_ptr<Stmt>;
    auto block() -> std::vector<std::shared_ptr<Stmt>>;
    auto assignment() -> std::unique_ptr<Expr>;
    auto varDeclaration() -> std::shared_ptr<Stmt>;
    auto declaration() -> std::shared_ptr<Stmt>;
    auto expressionStatement() -> std::shared_ptr<Stmt>;
    auto printStatement() -> std::shared_ptr<Stmt>;
    auto statement() -> std::shared_ptr<Stmt>;
    auto synchronize() -> void;
    class  ParseError : public std::runtime_error
    {
    public:
        ParseError() : std::runtime_error("Parse error") {}
    };

    auto perror(Token token, const std::string& message) -> ParseError;
    auto consume(TokenType type, std::string message) -> Token;

    auto primary() -> std::unique_ptr<Expr>;

    auto unary() -> std::unique_ptr<Expr>;
    auto factor() -> std::unique_ptr<Expr>;
    auto term() -> std::unique_ptr<Expr>;

    auto comparison() -> std::unique_ptr<Expr>;

    auto previous() -> Token;
    auto peek() -> Token;
    auto isAtEnd() -> bool;
    auto advance() -> Token;
    auto check(TokenType type) -> bool;



    auto match(std::initializer_list<TokenType> types) -> bool;
    
    auto equality() -> std::unique_ptr<Expr>;
    auto expression() -> std::unique_ptr<Expr>;
    
    const std::vector<Token> tokens;
    int current =0;
};
