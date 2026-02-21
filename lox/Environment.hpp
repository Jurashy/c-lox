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
class Environment {
private:
    std::shared_ptr<Environment<T>> enclosing;
    std::map<std::string, T> values;

public:
    Environment() = default;

    Environment(std::shared_ptr<Environment<T>> enclosing) : enclosing(enclosing) {}

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