//
// Created by hetto on 25.03.2026.
//

#include "chunk.hpp"

int Chunk::addConstants(double value) {
    m_constants.push_back(value);

    return m_constants.size() - 1;
}


void Chunk::printValue(double value) {
    std::print("{}", value);
}

int
Chunk::constantInstruction(std::string_view& name, int offset) {
    uint8_t constant = m_code[offset + 1];

    //std::print("{:<16} {:4} '", name, constant);
    std::print("{:<16} {:4} '", name, constant);

    printValue(m_constants[constant]);
    std::println("'");

    return offset + 2;
}

//disassemble
int
Chunk::simpleInstruction(std::string_view& name, int offset) {
    std::print("{}\n", name);

    return offset + 1;
}


void
Chunk::writeChunk(uint8_t byte, int line) {
    m_code.push_back(byte);
    m_lines.push_back(line);
}

int
Chunk::disassembleInstruction(int offset) {
    std::print("{:04} ", offset);

    uint8_t instruction = m_code[offset];
    std::string_view op_return = "OP_RETURN";
    std::string_view op_constant = "OP_CONSTANT";
    std::string_view op_negate = "OP_NEGATE";
    std::string_view op_subtract = "OP_SUBTRACT";
    std::string_view op_add = "OP_ADD";
    std::string_view op_multiply = "OP_MULTIPLY";
    std::string_view op_divide = "OP_DIVIDE";

    if (offset > 0 && m_lines[offset] == m_lines[offset -1])
        std::print("    |");
    else std::print("{:4} ", m_lines[offset]);
    switch (instruction) {
        case (uint8_t)OpCode::OP_CONSTANT:
            return constantInstruction(op_constant, offset);
        case static_cast<uint8_t>(OpCode::OP_ADD):
            return simpleInstruction(op_add, offset);
        case static_cast<uint8_t>(OpCode::OP_SUBTRACT):
            return simpleInstruction(op_subtract, offset);
        case static_cast<uint8_t>(OpCode::OP_MULTIPLY):
            return simpleInstruction(op_multiply, offset);
        case static_cast<uint8_t>(OpCode::OP_DIVIDE):
            return simpleInstruction(op_divide, offset);
        case (uint8_t)OpCode::OP_NEGATE:
            return simpleInstruction(op_negate, offset);
        case (uint8_t)OpCode::OP_RETURN:
            return simpleInstruction(op_return, offset);
        default:
            std::println("UNKNOWN opcode {}", instruction);
            return offset + 1;
    }
}

void
Chunk::disassembleChunk(std::string_view name)
{
    std::println("== {} ==", name);

    for (int offset = 0; offset < m_code.size();)
        offset = disassembleInstruction(offset);
}