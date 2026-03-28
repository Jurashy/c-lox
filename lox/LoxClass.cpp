//
// Created by hetto on 17.03.2026.
//

#include "LoxClass.hpp"

#include "LoxInstance.hpp"

auto LoxClass::toString() -> std::string { return m_name; }

auto LoxClass::call(Interpreter& interpreter, std::vector<Value> arguments) -> Value {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(shared_from_this());

    return instance;
}

auto LoxClass::findMethod(const std::string &name)  -> std::shared_ptr<LoxFunction> {
    if (m_methods.contains(name)) return m_methods[name];

    return nullptr;
}
