#pragma once
#include <string>
#include "Token.hpp"
#include "RuntimeError.hpp"

#include "../output/Stmt.hpp"

auto runtime_error(RuntimeError& error) -> void;

static auto runFile(std::string path) -> void;
static auto runPrompt() -> void;

static auto run(std::string source) -> void;

auto error(Token token, std::string message) -> void;
auto error(int line, std::string message) -> void;

static auto report(int line, std::string where, std::string message) -> void;