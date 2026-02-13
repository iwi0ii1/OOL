#pragma once

#include "./object.hpp"
#include <cstdint>

namespace ool::base {
    // Contract for stream-based objects
    // @note Implement `read` and `write`
    class stream : public object {
    public:
        virtual size_t read(char* buffer, size_t count) = 0;
        virtual size_t write(char* texts, size_t count) = 0;
    };
}