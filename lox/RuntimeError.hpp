#pragma once
#include "Token.hpp"

#include <stdexcept>
#include <string>

struct RuntimeError : public std::runtime_error
{
    RuntimeError(Token token, std::string message) 
    : std::runtime_error(message)
    , token { token } 
    {}

    auto getToken() const -> const Token&;
private:
    const Token token;
};