#include "Parser.hpp"
#include "TokenType.hpp"
#include "Lox.hpp"
#include "Interpreter.hpp"
#include "../output/Stmt.hpp"

#include <vector>
#include <memory>

// function objects 156

auto Parser::returnStatement() -> std::shared_ptr<Stmt> {
    auto keyword = previous();
    std::shared_ptr<Expr> value = nullptr;

    if (!check(TokenType::SEMICOLON)) value =  expression();

    consume(TokenType::SEMICOLON, "expect ';' after return.");
    return std::make_shared<Return>(keyword, value);
}

auto Parser::__function__(const std::string& kind) -> std::shared_ptr<Stmt> {
    const Token& name = consume(TokenType::IDENTIFIER, "expect " + kind + " name.");

    consume(TokenType::LEFT_PAREN, "expect '(' after " + kind + " name.");
    std::vector<Token> parameters {};

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                perror(peek(), "can not have more than 255 parameters");
            }

            parameters.push_back(consume(TokenType::IDENTIFIER, "expect parameter name"));
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_PAREN, "expect ')' after parameters");

    consume(TokenType::LEFT_BRACE, "expect '{' before " + kind + " body");
    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<Function>(name, parameters, body);
}

auto Parser::finishCall(std::unique_ptr<Expr>& callee)
    -> std::unique_ptr<Expr> {
    std::vector<std::shared_ptr<Expr>> arguments;

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (std::ssize(arguments) >= 255) {
                perror(peek(), "can't have more then 255 arguments");
            }
            arguments.push_back(std::move(expression()));
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_unique<Call>(std::move(callee), paren, arguments);
}

auto Parser::call() -> std::unique_ptr<Expr> {
    std::unique_ptr<Expr> expr = primary();

    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

auto Parser::forStatement() -> std::shared_ptr<Stmt> {
    consume(TokenType::LEFT_PAREN, "Expect '(' after a 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) condition = expression();
    consume(TokenType::SEMICOLON, "Exect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) increment = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses");

    std::shared_ptr<Stmt> body = statement();


    if (increment != nullptr)
        body = std::make_shared<Block>(std::vector<std::shared_ptr<Stmt>> { body, std::make_shared<Expression>(increment) });

    if (condition == nullptr) condition = std::make_shared<LLiteral>(true);
    body = std::make_shared<While>(std::move(condition), body);

    if (initializer != nullptr)
        body = std::make_shared<Block>(std::vector<std::shared_ptr<Stmt>> {initializer, body});
    return body;
}

auto Parser::whileStatement() -> std::shared_ptr<Stmt> {
    consume(TokenType::LEFT_PAREN, "expect '(' after the 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "expect ')' after the condition.");

    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(std::move(condition), body);
}

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
        auto value = assignment();

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
    const std::string& lF = "function";
    try {
        if (match({TokenType::FUN})) return __function__(lF);
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
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::WHILE})) return whileStatement();
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
[[maybe_unused]]
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

    return call();
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
    auto expr = term();
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
