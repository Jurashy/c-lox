#include "Token.hpp"
#include "TokenType.hpp"
#include "Value.hpp"
#include <iostream>
#include <string>
#include <sstream>



auto Token::getLine() const -> const int& { return line; }
auto Token::getLiteral() const -> const Value& { return literal; }
auto Token::getLexeme() const -> const std::string& { return lexeme; }
auto Token::getType() const -> const TokenType& { return type; }


auto Token::to_string() const -> std::string
{
    std::ostringstream oss;

    oss << static_cast<int>(type) << " " << lexeme << " ";

    std::visit([&oss](auto&& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (!std::is_same_v<T, std::monostate>)
        {
            oss << value;
        };
    }, literal);

    return oss.str();
}

auto operator<<(std::ostream& os, const Token& token) -> std::ostream&
{
    return os << token.to_string();
}