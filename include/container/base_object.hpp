#ifndef CONTAINER_BASE_OBJECT_HPP
#define CONTAINER_BASE_OBJECT_HPP

#include <cstddef>
#include <cstdint>

inline namespace ool {
    namespace container {

        // Base object where all containers inherit from (exclude primitives)
        class object {
            virtual ~object() = default;

            virtual size_t size() = 0;
            virtual bool empty() = 0;
        };
    }
}

#endif