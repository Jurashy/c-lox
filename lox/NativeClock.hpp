//
// Created by hetto on 07.03.2026.
//

#ifndef LOX_NATIVECLOCK_HPP
#define LOX_NATIVECLOCK_HPP
#include  "LoxCallable.hpp"
#include <chrono>

class NativeClock : public LoxCallable {
public:
    auto arity() -> int override { return 0; }

    auto call(Interpreter& /*interpreter*/, std::vector<Value> /*args*/) -> Value override {
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return static_cast<double>(duration.count()) / 1000.0;
    }

    auto toString() -> std::string override {
        return "<native fn>";
    }
};

#endif //LOX_NATIVECLOCK_HPP