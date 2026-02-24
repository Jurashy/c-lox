#include "Parser.hpp"
#include "TokenType.hpp"
#include "Lox.hpp"
#include "Interpreter.hpp"
#include "../output/Stmt.hpp"

#include <vector>
#include <memory>
auto Parser::logic_and() -> std::unique_ptr<Expr> {
    std::unique_ptr<Expr> expr = equality();

    while (match({TokenType::AND})) {
        Token oper = previous();

        std::unique_ptr<Expr> right = equality();

        expr = std::make_unique<Logical>(std::move(expr), oper, std::move(right));
    }

    return expr;
}
auto Parser::logic_or() -> std::unique_ptr<Expr> {
    std::unique_ptr<Expr> expr = logic_and();

    while (match({TokenType::OR})) {
        Token oper = previous();

        std::unique_ptr<Expr> right = logic_and();

        expr = std::make_unique<Logical>(std::move(expr), oper, std::move(right));
    }

    return expr;
}
auto Parser::ifStatement() -> std::shared_ptr<Stmt> {
    consume(TokenType::LEFT_PAREN, "expect '(' after a 'if'.");

    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "expect ')' after 'if' condition");

    auto thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;


    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_shared<IF>(std::move(condition), thenBranch, elseBranch);
}

auto Parser::block() -> std::vector<std::shared_ptr<Stmt>> {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.emplace_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expcet '}' after a block");

    return statements;
}

auto Parser::assignment() -> std::unique_ptr<Expr> {
    std::unique_ptr<Expr> expr = logic_or();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();

        if (auto var = dynamic_cast<Variable*>(expr.get())) {
            Token name = var->name;
            return std::make_unique<Assign>(name, std::move(value));
        }

        perror(equals, "invalid assignment target.");
    }
    return expr;
}

auto Parser::varDeclaration() -> std::shared_ptr<Stmt> {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(name, initializer);
}

auto Parser::declaration() -> std::shared_ptr<Stmt> {
    try {
        if (match({TokenType::VAR})) return varDeclaration();
        return statement();
    } catch (ParseError& e) {
        synchronize();
        return nullptr;
    }
}
auto Parser::expressionStatement() -> std::shared_ptr<Stmt> {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");

    return std::make_shared<Expression>(std::move(expr));
}

auto Parser::printStatement() -> std::shared_ptr<Stmt> {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value");

    return std::make_shared<Print>(std::move(value));
}

auto Parser::statement() -> std::shared_ptr<Stmt> {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_shared<Block>(block());

    return expressionStatement();
}

auto Parser::parse() -> std::vector<std::shared_ptr<Stmt>>
{
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.emplace_back(declaration());
    }
    return statements;
}

auto Parser::synchronize() -> void
{
    advance();

    while (!isAtEnd())
    {
        if (previous().getType() == TokenType::SEMICOLON) return;

        switch (peek().getType())
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
        }

        advance();
    }
}

auto Parser::perror(Token token, const std::string& message) -> ParseError
{
    error(token, message);
    return ParseError();
}

auto Parser::consume(TokenType type, std::string message) -> Token
{
    if (check(type)) return advance();

    throw perror(peek(), message);
}

auto Parser::primary() -> std::unique_ptr<Expr>
{
    if (match({TokenType::FALSE})) return std::make_unique<LLiteral>(false);
    if (match({TokenType::TRUE})) return std::make_unique<LLiteral>(true);
    if (match({TokenType::NIL})) return std::make_unique<LLiteral>(std::monostate{});

    if (match({TokenType::NUMBER, TokenType::STRING}))
        return std::make_unique<LLiteral>(previous().getLiteral());

    if (match({TokenType::IDENTIFIER})) return std::make_unique<Variable>(previous());
    if (match({TokenType::LEFT_PAREN}))
    {
        auto expr= expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");

        return std::make_unique<Grouping>(std::move(expr));
    }
    throw perror(peek(), "expect expression");

}

auto Parser::unary() -> std::unique_ptr<Expr>
{
    if (match({TokenType::BANG, TokenType::MINUS}))
    {
        auto oper = previous();
        auto right = unary();

        return std::make_unique<Unary>(oper, std::move(right));
    }

    return primary();
}

auto Parser::factor() -> std::unique_ptr<Expr>
{
    auto expr = unary(); // <-

    while (match({TokenType::SLASH, TokenType::STAR}))
    {
        Token oper = previous();
        auto right = unary(); //<-

        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }
    return expr;
}

auto Parser::term() -> std::unique_ptr<Expr>
{
    auto expr = factor(); //<- this is not created yet;

    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token oper = previous();
        auto right = factor(); //<-
        
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

auto Parser::comparison() -> std::unique_ptr<Expr>
{
    auto expr = term(); // <- we don't have this yet

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token oper = previous();
        auto right = term();
        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

auto Parser::previous() -> Token
{
    return tokens.at(current-1);
}

auto Parser::peek() -> Token
{
    return tokens.at(current);
}


auto Parser::isAtEnd() -> bool
{
    return peek().getType() == TokenType::EOF_TOKEN;
}

auto Parser::advance() -> Token
{
    if (!isAtEnd()) current++;
    return previous();
}

auto Parser::check(TokenType type) -> bool
{
    if (isAtEnd()) return false;
    return peek().getType() == type;
}



auto Parser::match(std::initializer_list<TokenType> types) -> bool
{
    // Parser* parser;
    for (TokenType type : types)
    {
        
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

auto Parser::equality() -> std::unique_ptr<Expr>
{
    auto expr = comparison();
    std::unique_ptr<Binary> binary;

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token oper = previous();
        auto right = comparison();

        expr = std::make_unique<Binary>(std::move(expr), oper, std::move(right));
    }

    return expr;
}

auto Parser::expression() -> std::unique_ptr<Expr>
{
    return assignment();
}

// 82
// 9 errors
