#pragma once

#include "../__internal/linear_iterator.hpp"
#include "../base/storage.hpp"
#include <algorithm>

namespace asl::containers {
    #pragma region Basic string
    // String
    // @param _char_type Char type (e.g., char, char16_t, wchar_t)
    template<base::char_like _char_type>
    requires (!std::is_const_v<_char_type>)
    class basic_string final : public base::storage<_char_type> {
    private:
        using base::storage<_char_type>::memory_;
        using base::storage<_char_type>::used_slots_;
        using base::storage<_char_type>::reallocate;

        using __l_type = basic_string<_char_type>;
        using __l_rtype = __l_type&;

    public:
        #pragma region Setup

        using iterator = __internal::linear_iterator<__internal::direction::forward, _char_type*>;
        using const_iterator = __internal::linear_iterator<__internal::direction::forward, const _char_type*>;
        using reversed_iterator = __internal::linear_iterator<__internal::direction::reversed, _char_type*>;
        using const_reversed_iterator = __internal::linear_iterator<__internal::direction::reversed, const _char_type*>;





        basic_string() {
            reallocate(1);
            std::copy_n("", 1, memory_);
            used_slots_ = 1;
            memory_[0] = _char_type{}; // Null-termination
        }






        // @param c_str A string literal
        basic_string(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            reallocate(n + 1);
            std::copy_n(c_str, n, memory_);
            used_slots_ = n;
            memory_[n] = _char_type{};
        }

        // You know what this does if you know `std::string::operator=()`
        basic_string<_char_type>& operator=(const _char_type* c_str) {
            const auto n = std::char_traits<_char_type>::length(c_str);
            reallocate(n + 1);
            std::copy_n(c_str, n, memory_);
            used_slots_ = n;
            memory_[n] = _char_type{};
            return *this;
        }






        // Move ctor.
        basic_string(basic_string&&) = default;

        // Move assign
        basic_string<_char_type>& operator=(basic_string&&) = default;






        // Construct by copy.
        // @param other Copy is expensive... but whatever
        basic_string(__l_rtype other) {
            reallocate(other.used_slots_);
            std::copy_n(other.memory_, other.used_slots_, this->memory_);
        }

        // You know what this does if you know `std::string::operator=()`
        basic_string<_char_type>& operator=(const basic_string<_char_type>& other) {
            reallocate(other.used_slots_);
            std::copy_n(other.memory_, other.used_slots_, this->memory_);
            return *this;
        }





        // Construct by iterator
        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        template<typename it_t = iterator>
        basic_string(it_t start, it_t end) {
            const size_t count = end - start;
            reallocate(count);
            std::copy(start, end, memory_);
            used_slots_ = count;
        }






        // @param one_char The char to spawn in this string
        // @param count How many times to spawn it
        basic_string(_char_type one_char, size_t count = 1) {
            reallocate(count);
            for (size_t i = 0; i < count; i++)
                memory_[i] = one_char;
            
            used_slots_ = count;
            memory_[count] = _char_type{};
        }

        #pragma endregion


        #pragma region Iterators

        iterator begin() const noexcept {
            return iterator(memory_);
        }

        iterator end() const noexcept {
            return iterator(memory_ + used_slots_);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(memory_);
        }

        const_iterator cend() const noexcept {
            return const_iterator(memory_ + used_slots_);
        }



        reversed_iterator rbegin() const noexcept {
            return reversed_iterator(memory_ + used_slots_ - 1);
        }

        reversed_iterator rend() const noexcept {
            return reversed_iterator(memory_ - 1);
        }

        const_reversed_iterator crbegin() const noexcept {
            return const_reversed_iterator(memory_ + used_slots_ - 1);
        }

        const_reversed_iterator crend() const noexcept {
            return const_reversed_iterator(memory_ - 1);
        }

        #pragma endregion




        #pragma region Mutators

        

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