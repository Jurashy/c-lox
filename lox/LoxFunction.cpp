//
// Created by hetto on 03.03.2026.
//

#include "LoxFunction.hpp"
#include "Return__.hpp"

auto LoxFunction::call(Interpreter& interpreter, std::vector<Value> arguments) -> Value  {
    auto env = std::make_shared<Environment<Value>>(interpreter.globals);
    for (size_t i = 0; i < m_declaration->params.size(); i++) {
        env->define(m_declaration->params[i].getLexeme(), arguments[i]);
    }
    try {
        interpreter.executeBlock(m_declaration->body, env);
    } catch (Return__ & returnValue) {
        return returnValue.m_value;
    }
    return {};
}

auto LoxFunction::arity() -> int {
    if (!m_declaration) {
        throw std::runtime_error("LoxFunction::arity called on null declaration");
    }
    return m_declaration->params.size();
}

auto LoxFunction::toString() -> std::string {
    return "<fn " + m_declaration->name.getLexeme() + ">";
}