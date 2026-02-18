#pragma once

#include "./iterators_common_logical.hpp"

namespace asl::__internal {
    // Linear forward iterator (contiguous memory)
    // @note All operators are forwarded
    template<typename __p_t>
    requires std::is_pointer_v<__p_t>
    class linear_forward_iterator final :
        public iterators_common_logical< linear_forward_iterator<__p_t> > {
    private:
        using __l_type = linear_forward_iterator<__p_t>;
        using __l_rtype = __l_type&;

    public:
        __p_t base_ptr; // The base pointer

        // @param __p Memory to iterate over
        explicit constexpr linear_forward_iterator(__p_t __p) noexcept : base_ptr(__p) {}



        constexpr __l_rtype operator++() noexcept {
            ++base_ptr;
            return *this;
        }

        constexpr __l_rtype operator--() noexcept {
            --base_ptr;
            return *this;
        }
    };



    // Linear reverse iterator (contiguous memory)
    // @note All operators aren't reversed except for `operator++` and `operator--`
    template<typename __p_t>
    requires std::is_pointer_v<__p_t>
    class linear_reverse_iterator final :
        public iterators_common_logical< linear_reverse_iterator<__p_t> > {
    private:
        using __l_type = linear_reverse_iterator<__p_t>;
        using __l_rtype = __l_type&;

    public:
        __p_t base_ptr;


        // @param __p Memory to iterate over
        explicit constexpr linear_reverse_iterator(__p_t __p) noexcept : base_ptr(__p) {}



        constexpr __l_rtype operator++() noexcept {
            --base_ptr;
            return *this;
        }

        constexpr __l_rtype operator--() noexcept {
            ++base_ptr;
            return *this;
        }
    };
}