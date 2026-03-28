//
// Created by hetto on 17.03.2026.
//

#ifndef LOX_LOXCLASS_HPP
#define LOX_LOXCLASS_HPP
#include <map>
#include <string>

#include "LoxCallable.hpp"
#include "LoxFunction.hpp"

struct LoxFunction;
struct LoxClass : public LoxCallable, std::enable_shared_from_this<LoxClass> {
public:
    std::string m_name;

    LoxClass(std::string name, std::map<std::string, std::shared_ptr<LoxFunction>> methods)
    : m_name(name)
    , m_methods(methods)
    {}


    auto toString() -> std::string override;

    auto arity() -> int override { return 0; }

    auto call(Interpreter& interpreter, std::vector<Value> arguments) -> Value override;

    auto findMethod(const std::string &name)  -> std::shared_ptr<LoxFunction>;

private:
    std::map<std::string, std::shared_ptr<LoxFunction>> m_methods;
};

#endif //LOX_LOXCLASS_HPP