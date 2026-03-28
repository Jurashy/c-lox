#pragma once
#include <string>
#include <vector>
#include <map>

#include "Token.hpp"
#include "TokenType.hpp"

class Scanner
{
private:
    auto match(char expected) -> bool;
    auto peek() -> char;
    const std::string m_source{};
    std::vector<Token> tokens{};

    int start = 0;
    int current { 0 };
    int line { 1 };

    auto scanToken() -> void;
    auto advance() -> char;

    auto addToken(TokenType type) -> void;
    auto addToken(TokenType type, Value literal) -> void;

    auto String__() -> void;

    auto isDigit(char c) -> bool;

    auto number() -> void;

    auto peekNext() -> char;
    auto identifier() -> void;

    static inline const std::map<std::string, TokenType> keywords
    {
        { "and",    TokenType::AND },
        { "class",  TokenType::CLASS },
        { "else",   TokenType::ELSE },
        { "false",  TokenType::FALSE },
        { "for",    TokenType::FOR },
        { "fun",    TokenType::FUN },
        { "if",     TokenType::IF },
        { "nil",    TokenType::NIL },
        { "or",     TokenType::OR },
        { "print",  TokenType::PRINT },
        { "return", TokenType::RETURN },
        { "super",  TokenType::SUPER },
        { "this",   TokenType::THIS },
        { "true",   TokenType::TRUE },
        { "var",    TokenType::VAR },
        { "while",  TokenType::WHILE }
    };
    
public:

    Scanner(std::string source) : m_source { source } {}

    auto scanTokens() -> std::vector<Token>;

    auto isAtEnd() -> bool;
};
