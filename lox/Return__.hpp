//
// Created by hetto on 07.03.2026.
//

#ifndef LOX_RETURN_HPP
#define LOX_RETURN_HPP


#include <stdexcept>

#include "Value.hpp"

struct Return__ : public std::runtime_error {
    Value m_value;

        Return__(Value value)
        : std::runtime_error("")
        , m_value { value } {}
};

#endif //LOX_RETURN_HPP