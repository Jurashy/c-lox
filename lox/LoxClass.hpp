//
// Created by hetto on 17.03.2026.
//

#ifndef LOX_LOXCLASS_HPP
#define LOX_LOXCLASS_HPP
#include <string>


class LoxClass{
public:
    std::string m_name;

    LoxClass(std::string name) : m_name(name) {}


    auto toString() const -> std::string;
};

#endif //LOX_LOXCLASS_HPP