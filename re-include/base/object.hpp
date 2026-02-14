/*
Where all objects inherit from.
Implicitly though other base classes or directly.
*/

#pragma once

#include <type_traits>
#include <concepts>
#include <stdexcept>

namespace asl::base {
    // Where all objects inherit from.
    // Implicitly though other base classes or directly.
    class object {
    public:
        // Just destructor, nothing lol.
        virtual ~object() = default;

        // Downcast by specified type.
        // @note Throws if downcast invalid.
        // @return The reference to downcasted type.
        template<typename T>
        T& as() const {
            static_assert(!std::is_pointer_v<T> && !std::is_reference_v<T>, "asl::base::object::as<T>(): Only accepts direct type.");
            const auto x = dynamic_cast<T*>(this);
            if (x == nullptr)
                throw std::bad_cast;
            else
                return *x;
        }
    };
}