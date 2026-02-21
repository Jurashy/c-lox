//
// Created by hetto on 21.02.2026.
//

#ifndef LOX_VALUE_HPP
#define LOX_VALUE_HPP
#include <variant>
#include <string>

using Value = std::variant<
    std::monostate, // nil
    double,
    bool,
    std::string
>;
#endif //LOX_VALUE_HPP