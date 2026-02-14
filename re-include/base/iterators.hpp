#pragma once

#include "./object.hpp"

namespace asl::base {
    // Iterators here.
    // @param _derived The class that enforces iterators.
    // @note You will need to manually implement `.begin()`, `.end()`, `.rbegin()` and `.rend()`
    // @param _pointer The pointer type that iterators return.
    template<typename _pointer, typename _derived>
    class iterator : public object {
    public:
        static_assert(std::is_pointer_v<_pointer> && !std::is_const_v<std::remove_pointer_t<_pointer>>, "_pointer must point to a const");
        static_assert(std::is_base_of_v<iterator<_pointer, _derived>, _derived>, "_derived must be the same as derived class.");
        iterator() = default;

    protected:
        _pointer begin() {
            return static_cast<_derived&>(*this).begin();
        }

        _pointer end() {
            return static_cast<_derived&>(*this).end();
        }


        _pointer rbegin() {
            return static_cast<_derived&>(*this).rbegin();
        }

        _pointer rend() {
            return static_cast<_derived&>(*this).rend();
        }
    };



    // Constant iterators here.
    // @param _derived The class that enforces iterators.
    // @note You will need to manually implement `.cbegin()`, `.cend()`, `.crbegin()` and `crend()`
    // @param _pointer The pointer type that iterators return.
    template<typename _pointer, typename _derived>
    class const_iterator : public object {
    public:
        static_assert(std::is_pointer_v<_pointer> && std::is_const_v<std::remove_pointer_t<_pointer>>, "_pointer must point to a non-const");
        static_assert(std::is_base_of_v<iterator<_pointer, _derived>, _derived>, "_derived must be the same as derived class.");
        const_iterator() = default;

    protected:
        _pointer cbegin() const {
            return static_cast<const _derived&>(*this).cbegin();
        }

        _pointer cend() const {
            return static_cast<const _derived&>(*this).cend();
        }


        _pointer crbegin() const {
            return static_cast<const _derived&>(*this).crbegin();
        }

        _pointer crend() const {
            return static_cast<const _derived&>(*this).crend();
        }
    };


    
    // Iterator and Const iterator
    template<typename _pointer, typename _derived>
    class iterator_common :
        public virtual iterator<_pointer, _derived>,
        public virtual const_iterator<const std::remove_pointer_t<_pointer>*, _derived> {};
}