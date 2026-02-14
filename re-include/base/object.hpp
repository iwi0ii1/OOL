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

        // Downcast to derived.
        // @note Throws if downcast invalid.
        template<typename _derived>
        _derived& as() const {
            static_assert(!std::is_pointer_v<_derived> && !std::is_reference_v<_derived>, "_derived cannot be a pointer or a reference.");
            return dynamic_cast<_derived&>(*this);
        }
    };
}