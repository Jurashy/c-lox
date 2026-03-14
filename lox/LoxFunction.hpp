//
// Created by hetto on 03.03.2026.
//

#ifndef LOX_LOXFUNCTION_HPP
#define LOX_LOXFUNCTION_HPP

#include <memory>

#include "Value.hpp"

#include "LoxCallable.hpp"
#include "Interpreter.hpp"

struct LoxFunction : public LoxCallable{
private:
    std::shared_ptr<Function> m_declaration;
    std::shared_ptr<Environment<Value>> m_closure;

public:
    LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment<Value>> closure = nullptr) :
    m_declaration { declaration }
    , m_closure { closure }
    {}

    auto call(Interpreter& interpreter, std::vector<Value> arguments) -> Value override;
    auto arity() -> int override;
    auto toString() -> std::string override;
};
#endif //LOX_LOXFUNCTION_HPP