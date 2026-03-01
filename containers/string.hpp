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
            this->__l_fn_realloc(1);
            this->used_slots_ = 1;
            this->data_[0] = _char_type{}; // Null-termination
        }






        // @param c_str A string literal
        basic_string(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            this->__l_fn_realloc(n + 1);
            std::copy_n(c_str, n, this->data_);
            this->used_slots_ = n;
            this->data_[n] = _char_type{};
        }

        // You know what this does if you know `std::string::operator=()`
        __l_self_rtype operator=(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            this->__l_fn_realloc(n + 1);
            std::copy_n(c_str, n, this->data_);
            this->used_slots_ = n;
            this->data_[n] = _char_type{};
            return *this;
        }






        // Move ctor.
        basic_string(basic_string&&) = default;

        // Move assign
        basic_string<_char_type>& operator=(basic_string&&) = default;






        // Construct.
        basic_string(__l_self_crtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
        }

        // You know what this does if you know `std::string::operator=()`
        __l_self_rtype operator=(__l_self_crtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
            return *this;
        }





        // Construct by iterator
        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        basic_string(const_iterator start, const_iterator end) {
            const size_t count = end - start;
            this->__l_fn_realloc(count);
            std::copy(start, end, this->data_);
            this->used_slots_ = count;
        }






        // Fill in with `one_char`
        // @param one_char The char to spawn in this string
        // @param count How many times to spawn it
        basic_string(_char_type one_char, size_t count = 1) {
            this->__l_fn_realloc(count + 1);
            std::uninitialized_fill_n(this->data_, count, one_char);            
            this->used_slots_ = count;
            this->data_[count] = _char_type{};
        }
        #pragma endregion



        #pragma region Check
        inline bool operator==(__l_self_crtype other) const noexcept {
            if (this->used_slots_ != other.used_slots_ )
                return false;

            for (size_t i = 0; i < this->used_slots_; ++i) {
                if (this->memory_[i] != other.memory_[i])
                    return false;
            }

            return true;
        }
        #pragma endregion




        #pragma region Mutators
        // Append / Concatenate string
        inline iterator append(__l_self_crtype other) {
            return this->insert(this->end(), other.begin(), other.end());
        }





        // Append / Concatenate string
        inline __l_self_rtype operator+=(__l_self_crtype other) {
            append(other);
            return *this;
        }

        // Append character
        inline __l_self_rtype operator+=(const _char_type ch) {
            this->push_back(ch);
            return *this;
        }





        // Append / Concatenate string (temporarily)
        inline __l_self_type operator+(__l_self_crtype other) {
            __l_self_type tmp(this->data_);
            tmp.append(other);
            return tmp;
        }

        // Append character (temporarily)
        inline __l_self_type operator+(const _char_type ch) {
            __l_self_type tmp(this->data_);
            tmp.push_back(ch);
            return tmp;
        }





        // Lowercase string by a range (ASCII only)
        inline __l_self_rtype to_lower(const_iterator first, const_iterator last) noexcept {
            size_t idx_first = first - this->begin();
            const size_t idx_last = last - this->begin();

            for (; idx_first < idx_last; ++idx_first) {
                _char_type& ch = this->data_[idx_first]; // The same as *(this->begin() + idx_first)
                if (ch >= _char_type('A') && ch <= _char_type('Z'))
                    ch += _char_type('a' - 'A');
            }
            return *this;
        }

        // Uppercase string by a range
        inline __l_self_rtype to_upper(const_iterator first, const_iterator last) noexcept {
            size_t idx_first = first - this->begin();
            const size_t idx_last = last - this->begin();

            for (; idx_first < idx_last; ++idx_first) {
                _char_type& ch = this->data_[idx_first]; // The same as *(this->begin() + idx_first)
                if (ch >= _char_type('a') && ch <= _char_type('z'))
                    ch -= _char_type('a' - 'A');
            }
            return *this;
        }

        // Kinda like std::stoi, which returns a number by checking the string
        template<base::numeric T>
        inline T to_number(const_iterator first, const_iterator last) {
            // TODO: Interpret string, and return a number (FPN and Integral)
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