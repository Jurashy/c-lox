#ifndef LOX_LOXINSTANCE_HPP
#define LOX_LOXINSTANCE_HPP

#include <memory>
#include <unordered_map>
#include <string>

#include "Token.hpp"
#include "Value.hpp"

struct LoxClass;
struct LoxFunction;

struct LoxInstance : public std::enable_shared_from_this<LoxInstance> {
public:
    LoxInstance(std::shared_ptr<LoxClass> klass);

    std::string toString();
    Value get__(const std::string& name);
    void set(Token name, Value value);

private:
    std::shared_ptr<LoxClass> m_klass;
    std::unordered_map<std::string, Value> fields;
};

#endif