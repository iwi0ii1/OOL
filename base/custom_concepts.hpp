/*
Just custom concepts
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

        // Is Move-constructible, Copy-constructible, and Destructible
        template<typename T> concept storage_compatible = 
            std::is_move_constructible_v<T> &&
            std::is_copy_constructible_v<T> &&
            std::is_destructible_v<T>;
    }
}