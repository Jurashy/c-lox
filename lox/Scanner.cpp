#include "Scanner.hpp"

#include "Token.hpp"
#include "TokenType.hpp"
#include "Lox.hpp"
#include "Parser.hpp"
#include "Value.hpp"


#include <variant>
#include <iostream>
#include <vector>
#include <cctype>

auto Scanner::peek() -> char
{
    if (isAtEnd()) return '\0';
    return m_source.at(current);
}


auto Scanner::scanTokens() -> std::vector<Token>
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::EOF_TOKEN, "", std::monostate{}, line);
    return tokens;
}

auto Scanner::isAtEnd() -> bool
{
    return current >= std::ssize(m_source);
}

auto Scanner::scanToken() -> void
{
    char c = advance();
    switch (c)
    {
        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line++;
            break;
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '*':
        addToken(TokenType::STAR);
        break;
    case '"': 
        String__();
        break;
    case '=':
            addToken(TokenType::EQUAL);
            break;
    // case 'o':
    //     if (peek() == 'r') addToken(TokenType::OR);
    //     break;
    default:
        if (isDigit(c)) number();
        else if (std::isalpha(c)) identifier();
        else error(line, "unexpected character");
    }
}

auto Scanner::advance() -> char
{
    current++;
    return m_source.at(current - 1);
}

auto Scanner::addToken(TokenType type) -> void
{
    addToken(type, std::monostate{});
}



auto Scanner::addToken(TokenType type, Value literal) -> void
{
    std::string text = m_source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

auto Scanner::String__() -> void
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd())
    {
        error(line, "unterminated string");
    }

    advance();

    std::string value = m_source.substr(start + 1, current -start - 2);
    addToken(TokenType::STRING, value);
}

auto Scanner::isDigit(char c) -> bool
{
    return c >= '0' && c <= '9';
}

auto Scanner::number() -> void
{
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext()))
    {
        advance();

        while (isDigit(peek())) advance();
    }

    std::string text = m_source.substr(start, current-start);
    double value = std::stod(text);
    addToken(TokenType::NUMBER, value);
}

auto Scanner::peekNext() -> char
{
    if (current + 1 >= m_source.length()) return '\0';
    return m_source.at(current + 1);
}

auto Scanner::identifier() -> void
{
    while (std::isalnum(peek()) || peek() == '_')
    {
        advance();
    }



    //addToken(TokenType::IDENTIFIER);

    std::string text = m_source.substr(start, current-start);
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    addToken(type);
}
