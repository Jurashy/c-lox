//
// Created by hetto on 27.03.2026.
//

#include "scanner.hpp"

struct Scanner {
    const char* start;
    const char* current;
    int line;
};

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

