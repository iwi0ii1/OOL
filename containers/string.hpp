#pragma once

#include "../base/contiguous_storage.hpp"
#include <algorithm>

namespace asl::containers {
    #pragma region Basic string
    // String
    template<base::char_like _char_type>
    class basic_string final : public base::contiguous_storage<_char_type> {
    private:
        using __l_base_type = base::contiguous_storage<_char_type>;
        using __l_self_type = basic_string<_char_type>;
        using __l_self_rtype = __l_self_type&;
        using __l_self_crtype = const __l_self_type&;

    public:
        using typename __l_base_type::iterator;
        using typename __l_base_type::const_iterator;
        using typename __l_base_type::reversed_iterator;
        using typename __l_base_type::const_reversed_iterator;
        

        #pragma region Setup
        basic_string() {
            __l_fn_realloc(1);
            used_slots_ = 1;
            data_[0] = _char_type{}; // Null-termination
        }






        // @param c_str A string literal
        basic_string(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            __l_fn_realloc(n + 1);
            std::copy_n(c_str, n, data_);
            used_slots_ = n;
            data_[n] = _char_type{};
        }

        // You know what this does if you know `std::string::operator=()`
        basic_string<_char_type>& operator=(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            __l_fn_realloc(n + 1);
            std::copy_n(c_str, n, data_);
            used_slots_ = n;
            data_[n] = _char_type{};
            return *this;
        }






        // Move ctor.
        basic_string(basic_string&&) = default;

        // Move assign
        basic_string<_char_type>& operator=(basic_string&&) = default;






        // Construct by copy.
        // @param other Copy is expensive... but whatever
        basic_string(__l_rtype other) {
            __l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
        }

        // You know what this does if you know `std::string::operator=()`
        basic_string<_char_type>& operator=(const basic_string<_char_type>& other) {
            __l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
            return *this;
        }





        // Construct by iterator
        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        template<typename it_t = iterator>
        basic_string(it_t start, it_t end) {
            const size_t count = end - start;
            __l_fn_realloc(count);
            std::copy(start, end, data_);
            used_slots_ = count;
        }






        // @param one_char The char to spawn in this string
        // @param count How many times to spawn it
        basic_string(_char_type one_char, size_t count = 1) {
            __l_fn_realloc(count);
            for (size_t i = 0; i < count; i++)
                data_[i] = one_char;
            
            used_slots_ = count;
            data_[count] = _char_type{};
        }
        #pragma endregion



        #pragma region Check
        inline bool operator==(__l_self_crtype other) const noexcept {
            if (this->used_slots_ != other.used_slots_ )
                return false;

            for (size_t i = 0; i < used_slots_; ++i) {
                if (this->memory_[i] != other.memory_[i])
                    return false;
            }

            return true;
        }
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