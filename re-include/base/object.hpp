/*
Where all objects inherit from.
Implicitly though other base classes or directly.
*/

#pragma once

#include <type_traits>
#include <concepts>
#include <stdexcept>

namespace asl::base {
    inline namespace custom_concepts {
        // Pointer concept
        template<typename T> concept a_pointer = std::is_pointer_v<T>;

        // Regular value concept
        template<typename T> concept a_regular_value = !std::is_pointer_v<T> && !std::is_reference_v<T>;

        // Is char-like
        template<typename T> concept char_like = 
            std::same_as<T, char> ||
            std::same_as<T, signed char> ||
            std::same_as<T, unsigned char> ||
            std::same_as<T, wchar_t> ||
            std::same_as<T, char8_t> ||
            std::same_as<T, char16_t> ||
            std::same_as<T, char32_t>;
    }

    // Where all objects inherit from.
    // Implicitly though other base classes or directly.
    class object {
    public:
        object() = default;

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