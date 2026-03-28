
#include <cstdint>
#include <vector>
#include <string_view>
#include <print>
#include <cstdlib>
#include <iostream>
#include <string>

#include "vm.hpp"
#include "chunk.hpp"

static void repl() {
    Vm vm {};

    char line[1024];
    for (;;) {
        std::print("> ");

        if (!std::fgets(line, sizeof(line), stdin)) {
            std::println("");
            break;
        }
        vm.interpret(line);
    }
}

static void runFile(const char* path) {
    char* source = readFile(path);

    auto result = Vm::interpret(source);
    delete[] source;

    if (result == InterpretResult::INTERPRET_COMPILE_ERROR)
        std::exit(EXIT_FAILURE);
    if (result == InterpretResult::INTERPRET_RUNTIME_ERROR)
        std::exit(EXIT_FAILURE);
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    std::fseek(file, 0L, SEEK_END);

    size_t fileSize = std::ftell(file);
    std::rewind(file);

    char* buffer = new char[fileSize + 1];
    if (buffer == nullptr) std::cerr << "Out of memory! " << path << std::endl; std::exit(EXIT_FAILURE);

    size_t bytesRead = std::fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) std::cerr << "could not read file " << path << std::endl; std::exit(EXIT_FAILURE);
    buffer[bytesRead] = '\0';

    fclose(file);

    return buffer;
}


int main(int argc, const char * argv[]) {
    Vm vm{};
    vm.initVM();

    if (argc == 1) repl();
    else if (argc == 2) runFile(argv[1]);
    else {
        std::cerr << "usage c++lox[path]\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
