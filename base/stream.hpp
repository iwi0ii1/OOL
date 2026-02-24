#pragma once

#include "./object.hpp"
#include <cstdint>

namespace asl::base {
    // Contract for stream-based objects
    // @note Implement `size_t read(char*, size_t) const` and `size_t write(const char*, size_t) const`
    class stream : public object {
    public:
        virtual size_t read(char* buffer, size_t count) const = 0;
        virtual size_t write(const char* texts, size_t count) const = 0;
    };
}