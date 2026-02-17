#pragma once

#include "../base/array_iterator.hpp"
#include "../base/storage.hpp"
#include <algorithm>

namespace asl::containers {
    #pragma region Basic string
    // String
    // @param _char_type Char type (e.g., char, char16_t, wchar_t)
    template<base::char_like _char_type>
    requires (!std::is_const_v<_char_type>)
    class basic_string final : public base::array_iterator<_char_type> {
    public:
        #pragma region Setup
        using iterator_t = base::array_iterator<_char_type>::iterator_t;
        using const_iterator_t = base::array_iterator<_char_type>::const_iterator_t;
        using value_type = _char_type;


        // @param c_str A string literal
        basic_string(const _char_type* c_str) noexcept {
            const auto n = std::char_traits<_char_type>::length(c_str);
            this->reallocate(n + 1);
            std::copy_n(c_str, n, this->memory_);
            this->memory_[n] = _char_type{}; // Null-termination
        }

        basic_string(basic_string&&) noexcept = default;

        // @param other Copy is expensive... but whatever
        basic_string(const basic_string& other) {
            this->reallocate(other.used_slots_);
            std::copy_n(other.memory_, other.used_slots_, this->memory_);
        }

        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        basic_string(iterator_t start, iterator_t end) {
            const size_t count = end - start;
            this->reallocate(count);
            std::copy(start, end, this->memory_);
            this->used_slots_ = count;
        }

        basic_string(_char_type one_char, size_t count = 1) {

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