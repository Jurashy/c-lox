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
    std::shared_ptr<Environment<Value>> closure;

public:
    LoxFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment<Value>> env = nullptr) :
    m_declaration { declaration }
    , closure { env }
    {}

    auto call(Interpreter& interpreter, std::vector<Value> arguments) -> Value override;
    auto arity() -> int override;
    auto toString() -> std::string override;
};
#endif //LOX_LOXFUNCTION_HPP