//
// Created by hetto on 25.03.2026.
//

#include "vm.hpp"
#include "chunk.hpp"
#include "common.hpp"
#include "compiler.hpp"

void Vm::initVM() {
    return resetStack();
}

double Vm::Pop() {
    stackTop--;
    return *stackTop;
}

void Vm::Push(double value) {
    *stackTop = value;
    stackTop++;
}

void Vm::resetStack() {
    stackTop = stack;
}

InterpretResult Vm::run() {
#define READ_BYTE() (*m_ip++)
    auto BINARY_OP = [&](auto op) {
        double b = Pop();
        double a = Pop();
        Push(op(a, b));

    };
#define READ_CONSTANT() (m_chunk->m_constants[READ_BYTE()])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        std::print("        ");
        for (double* slot = stack; slot < stackTop; slot++) {
            std::print("[ ");
            Chunk::printValue(*slot);
            std::print(" ]");
        }
        std::println("");
        m_chunk->disassembleInstruction(m_ip- m_chunk->m_code.data());
#endif
        uint8_t instruction {};
        switch (instruction = READ_BYTE()) {
            case (uint8_t)OpCode::OP_CONSTANT: {
                double constant = READ_CONSTANT();
                Push(constant);
                break;
            }
            case static_cast<uint8_t>(OpCode::OP_ADD): {
                BINARY_OP([](const double a, const double b) { return a + b; });
                break;
            }
            case static_cast<uint8_t>(OpCode::OP_SUBTRACT): {
                BINARY_OP([](const double a, const double b) { return a - b; });
                break;
            }
            case static_cast<uint8_t>(OpCode::OP_MULTIPLY): {
                BINARY_OP([](const double a, const double b) { return a * b; });
                break;
            }
            case static_cast<uint8_t>(OpCode::OP_DIVIDE): {
                BINARY_OP([](const double a, const double b) { return a / b; });
                break;
            }
            case (uint8_t)OpCode::OP_NEGATE:
                Push(-Pop());
                break;
            case (uint8_t)OpCode::OP_RETURN: {
                Chunk::printValue(Pop());
                std::println("");
                return InterpretResult::INTERPRET_OK;
            }
            default: std::print("fuck you nigga\n");
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult Vm::interpret(const char* source) {
    // m_chunk = &chunk;
    // m_ip = m_chunk->m_code.data();

    compile(source);

    return InterpretResult::INTERPRET_OK;
}
