//
// Created by hetto on 28.02.2026.
//

#ifndef LOX_LOXCALLABLE_HPP
#define LOX_LOXCALLABLE_HPP

#include "Value.hpp"

#include <vector>
#include <chrono>
// #include "Interpreter.hpp"
struct Interpreter; // forward declare

struct LoxCallable {
   virtual  auto arity() -> int =0 ;

    virtual auto call(Interpreter& interpreter, std::vector<Value> arguments) -> Value = 0;

    virtual  auto toString() -> std::string = 0;

    virtual ~LoxCallable() = default;
};

#endif //LOX_LOXCALLABLE_HPP