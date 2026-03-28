#include "RuntimeError.hpp"
#include "Token.hpp"

auto RuntimeError::getToken() const -> const Token&
{
    return token;
}