//
// Created by hetto on 18.02.2026.
//

#ifndef LOX_ENVIRONMENT_HPP
#define LOX_ENVIRONMENT_HPP

#include <map>
#include <string>
#include <memory>

#include "RuntimeError.hpp"
#include "Token.hpp"

template <class T>
class Environment : public std::enable_shared_from_this<Environment<T>>{
private:
    std::shared_ptr<Environment> enclosing;
    std::map<std::string, T> values;

public:
    Environment() = default;
    Environment(std::shared_ptr<Environment> enclosing) : enclosing(enclosing) {}

    void
    assign(Token name, T value) {
        if (values.find(name.getLexeme()) != values.end()) {
            values[name.getLexeme()] = value;
            return;
        }

        if (enclosing != nullptr) {
            enclosing->assign(name, value);
            return;
        }
        throw RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'");
    }
    void
    define(std::string name, T value) {
        values[name] = value;
    }

    T getAt(int distance, const std::string& name) {
        std::shared_ptr<Environment> env = ancestor(distance);
        auto it = env->values.find(name);

        if (it == env->values.end())
            throw std::runtime_error("Variable '" + name + "' does not exist.");

        return it->second;
    }
    auto ancestor(int distance) -> std::shared_ptr<Environment> {
        std::shared_ptr<Environment> environment = this->shared_from_this();

        for (int i = 0; i < distance; i++) {
            if (environment->enclosing == nullptr) throw::std::runtime_error("invalid environment distance");

            environment = environment->enclosing;
        }

        return environment;
    }

    auto assignAt(int distance, Token name, auto value) {
        ancestor(distance)->values[name.getLexeme()] = value;
    }

    T get(Token name) {
        if (values.find(name.getLexeme()) != values.end()) {
            return values[name.getLexeme()];
        }

        if (enclosing != nullptr) {
            return enclosing->get(name);
        }
        throw RuntimeError(name, "Undefined variable '" + name.getLexeme() + "'");
    }
};

#endif //LOX_ENVIRONMENT_HPP