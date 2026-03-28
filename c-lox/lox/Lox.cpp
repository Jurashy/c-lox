#include <iostream>

#include <vector>
#include <fstream>
#include <string>
#include <print>
#include <limits>
#include <memory>

#include "AstPrinter.hpp"
#include "Parser.hpp"
#include "Lox.hpp"
#include "Token.hpp"
#include "TokenType.hpp"
#include "Scanner.hpp"
#include "RuntimeError.hpp"
#include "Interpreter.hpp"
#include "../output/Stmt.hpp"
#include "Environment.hpp"
#include "Value.hpp"
#include "Resolver.hpp"

static const auto interpreter = std::make_shared<Interpreter>();
static_assert(!std::is_abstract_v<Interpreter>,
              "Interpreter is still abstract!");
namespace some_vars
{
    static bool hadError = false;
    static bool hadRuntimeError = false;    
} // namespace some_vars


auto runtime_error(RuntimeError& error) -> void
{
    std::cerr << error.what() << "\n[line " << error.getToken().getLine() << "]\n";
    some_vars::hadRuntimeError = true;
}



auto main(int argc, char**argv) -> int
{
    if (argc > 2)
    {
        puts("usage: c++lox [script]");
        return EXIT_FAILURE;
    } 
    else if (argc == 2)
    {
        runFile(argv[0]);
    }
    else 
    {
        runPrompt();
    }
}

static auto runFile(std::string path) -> void
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "no no no you are so stupid\n";
        std::exit(EXIT_FAILURE); 
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    run(source);

    if(some_vars::hadError) std::exit(EXIT_FAILURE);
}

static auto runPrompt() -> void
{
    std::string buffer;
    int openBraces = 0;

    for(;;)
    {
        std::cout << (openBraces > 0 ? "| " : ">");
        std::string line;
        if (!std::getline(std::cin, line))
        {
            break;
        }
        for (char c : line) {
            if (c == '{') openBraces++;
            if (c == '}') openBraces--;
        }
        buffer += line + "\n";
        if (openBraces <= 0) {
            run(buffer);
            buffer.clear();
            openBraces = 0;
            some_vars::hadError = false;
        }
    }
}


static auto run(std::string source) -> void
{
    Scanner scanner{source};

    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser(tokens);
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

    if (some_vars::hadError) return;
    auto resolver = std::make_shared<Resolver>(interpreter);
    resolver->resolve(statements);

    interpreter->interpret(statements);

    if (some_vars::hadRuntimeError) std::exit(EXIT_FAILURE);
}

auto error(Token token, std::string message) -> void
{
    if (token.getType() == TokenType::EOF_TOKEN) report(token.getLine(), " at end", message);
    else report(token.getLine(), " at '" + token.getLexeme() + "'", message);
}
auto error(int line, std::string message) -> void
{
    report(line, "", message);
}

static auto report(int line, std::string where, std::string message) -> void
{
    std::println("[line {} ] Error {} : {}", line, where, message);
    some_vars::hadError = true;
}
/*
    std::ifstream
    std::stringstream
    file.rdbuf()
    str()

*/