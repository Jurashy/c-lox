#pragma once
#include "TokenType.hpp"
#include "Value.hpp"
#include <string>
#include <sstream>



class Token
{
private:
    
    const TokenType type;
    const std::string lexeme;
    const Value literal;
    const int line;

public:
    Token(TokenType type, std::string lexeme, Value literal, int line)
    : type { type }
    , lexeme { lexeme }
    , literal { literal }
    , line { line }
    {}

    auto to_string() const -> std::string;
    auto getLexeme() const -> const std::string&;
    auto getType() const -> const TokenType&;
    auto getLiteral() const -> const Value&;
    auto getLine() const -> const int&;
};

auto operator<<(std::ostream& os, const Token& token) -> std::ostream&;