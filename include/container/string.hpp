#ifndef CONTAINER_STRING_HPP
#define CONTAINER_STRING_HPP

#include "base_object.hpp"
#include <type_traits>

inline namespace ool {
    namespace container {

        // Basic string
        template<typename T>
        class basic_string final : private object {
        private:
            size_t size;

        public:
            // TODO: Implement first container
        };
    }
}

#endif