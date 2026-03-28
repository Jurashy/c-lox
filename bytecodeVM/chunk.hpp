//
// Created by hetto on 25.03.2026.
//

#ifndef LOXINTERPRETER_CHUNK_HPP
#define LOXINTERPRETER_CHUNK_HPP

#include <cstdint>
#include <vector>
#include <string_view>
#include <print>
#include <memory>

// one  byte
enum class OpCode : uint8_t{
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN
};

struct Chunk {
    std::vector<uint8_t> m_code;
    std::vector<int> m_lines;
    std::vector<double> m_constants;

    Chunk(std::vector<uint8_t>& code, std::vector<int>& lines, std::vector<double>& constants)
        : m_code(code), m_lines(lines), m_constants(constants)
    {}
    Chunk() = default;


    int addConstants(double value);

    static void printValue(double value);

     int
    constantInstruction(std::string_view& name, int offset);

     int
    simpleInstruction(std::string_view& name, int offset);

    void
    writeChunk(uint8_t byte, int line);


    int
    disassembleInstruction(int offset);

    void
    disassembleChunk(std::string_view name);
};


#endif //LOXINTERPRETER_CHUNK_HPP