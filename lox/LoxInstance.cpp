#include "LoxInstance.hpp"
#include "LoxClass.hpp"      // ✅ FULL definition here
#include "LoxFunction.hpp"
#include "LoxCallable.hpp"

LoxInstance::LoxInstance(std::shared_ptr<LoxClass> klass)
    : m_klass{klass} {}

std::string LoxInstance::toString() {
    return m_klass->m_name + " instance";
}

Value LoxInstance::get__(const std::string& name) {
    if (fields.contains(name)) return fields[name];

    auto method = m_klass->findMethod(name);

    if (method != nullptr) {
        return method->bind(shared_from_this());
    }

    throw std::runtime_error("No such field: " + name);
}

void LoxInstance::set(Token name, Value value) {
    fields[name.getLexeme()] = value;
}//
// Created by hetto on 20.03.2026.
//