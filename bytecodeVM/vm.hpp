//
// Created by hetto on 25.03.2026.
//

#ifndef LOXINTERPRETER_VM_HPP
#define LOXINTERPRETER_VM_HPP

#include <cstdint>
#include <vector>
#include "chunk.hpp"

constexpr int STACK_MAX = 256;


enum class InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

struct Vm {

    Chunk* m_chunk;
    uint8_t *m_ip;
    double stack[STACK_MAX];
    double* stackTop;

    Vm (Chunk* chunk)
    : m_chunk(chunk)
    {}
    Vm () = default;
    ~Vm () = default;

    void initVM();
    double Pop();

    void Push(double value);
    void resetStack();

    InterpretResult run();

    static InterpretResult interpret(const char* source);

};

#endif //LOXINTERPRETER_VM_HPP