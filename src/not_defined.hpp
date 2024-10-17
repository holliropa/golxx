#pragma once

#include <exception>

namespace exceptions {

    class not_defined : std::exception {
    public:
        const char *what() const noexcept override {
            return "Function is not defined";
        }
    };
}