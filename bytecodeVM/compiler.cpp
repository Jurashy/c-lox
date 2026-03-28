//
// Created by hetto on 27.03.2026.
//

#include "compiler.hpp"

void compile(const char* source) {
    initScanner(source);

    int line = -1;

    Token token = scanToken();

    if (token.line != line) {
        std::print("{:4}", token.line);
        line = token.line;
    } else std::print("     |");

    std::print("{:2} {} {}\n", toke.type, token.length, token.start);
}