#pragma once

#include "../base/iterators.hpp"
#include "../base/storage.hpp"
#include <algorithm>

namespace asl::containers {
    #pragma region Basic string
    // String
    // @param _char_type Char type (e.g., char, char16_t, wchar_t)
    template<base::char_like _char_type>
    requires !std::is_const_v<_char_type>
    class basic_string final : public base::iterator_common<_char_type*, basic_string<_char_type>>, base::storage<_char_type> {
    public:
        #pragma region Setup
        // @param c_str A string literal
        basic_string(const _char_type* c_str) noexcept {
            const auto n = std::char_traits<_char_type>::length(c_str);
            resize(n);
            std::copy_n(c_str, n, memory_);
        }

        basic_string(basic_string&&) noexcept = default;

        // @param other Copy is expensive... but whatever
        basic_string(const basic_string& other) {
            this->reallocate(other.used_slots_);
            std::copy_n(other.memory_, other.used_slots_, this->memory_);
        }

        #pragma endregion




        #pragma region Methods


        #pragma endregion
    };


    #pragma endregion


    #pragma region Type aliases
    using string = basic_string<char>;
    using wstring = basic_string<wchar_t>;
    using u8string = basic_string<char8_t>;
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;
    #pragma endregion
}