//
// Created by hetto on 21.02.2026.
//

#ifndef LOX_VALUE_HPP
#define LOX_VALUE_HPP
#include <variant>
#include <string>
#include <memory>

struct LoxCallable;
struct LoxClass;

using CallablePtr = std::shared_ptr<LoxCallable>;

using Value = std::variant<
    std::monostate, // nil
    double,
    bool,
    std::string,
    CallablePtr,
    std::nullptr_t,
    std::shared_ptr<LoxClass>
>;
#endif //LOX_VALUE_HPP