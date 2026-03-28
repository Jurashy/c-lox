//
// Created by hetto on 03.03.2026.
//

#include "LoxFunction.hpp"
#include "Return__.hpp"
#include "Interpreter.hpp"


auto LoxFunction::bind(std::shared_ptr<LoxInstance> instance) -> std::shared_ptr<LoxFunction> {
    std::shared_ptr<Environment<Value>> environment = std::make_shared<Environment<Value>>(m_closure);

    environment->define("this", instance);

    return std::make_shared<LoxFunction>(m_declaration, environment);
}

auto LoxFunction::call(Interpreter& interpreter, std::vector<Value> arguments) -> Value {
    auto functionEnv = std::make_shared<Environment<Value>>(m_closure);

    for (size_t i = 0; i < m_declaration->params.size(); i++) {
        functionEnv->define(m_declaration->params[i].getLexeme(), arguments[i]);
    }

    // IMPORTANT: use interpreter.executeBlock, passing the functionEnv
    try {
        interpreter.executeBlock(m_declaration->body, functionEnv);
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