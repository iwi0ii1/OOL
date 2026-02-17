#pragma once

#include "./storage.hpp"

namespace asl::base {
    // Array-like iterator
    // @param _underlying_type The underlying type returned by iterator ptr.
    // @note Implemented begin(), end(), rbegin(), rend(), cbegin(), cend(), crbegin(), crend(). No contracts.
    // @warning Inherited `asl::base::storage<_underlying_type>`
    template<a_regular_value _underlying_type>
    requires (!std::is_const_v<_underlying_type>)
    class array_iterator : public storage<_underlying_type> {
    protected:
        array_iterator() = default; // Constructible by derived

    public:
        using iterator_t = _underlying_type*; // Iterator ptr
        using const_iterator_t = const _underlying_type* ; // Iterator const ptr

        // First element
        inline iterator_t begin() const noexcept {
            return this->memory_;
        }

        // Memory after the last element (do not dereference!)
        inline iterator_t end() const noexcept {
            return this->memory_ + this->used_slots_;
        }

        // Last element (dereferenceable)
        inline iterator_t rbegin() const noexcept {
            return this->memory_ + this->used_slots_ - 1;
        }

        // Memory before the first element (do not dereference!)
        inline iterator_t rend() const noexcept {
            return this->memory_ - 1;
        }

        
        // First element
        inline const_iterator_t cbegin() const noexcept {
            return this->memory_;
        }

        // Memory after the last element (do not dereference!)
        inline const_iterator_t cend() const noexcept {
            return this->memory_ + this->used_slots_;
        }

        // Last element (dereferenceable)
        inline const_iterator_t crbegin() const noexcept {
            return this->memory_ + this->used_slots_ - 1;
        }

        // Memory before the first element (do not dereference!)
        inline const_iterator_t crend() const noexcept {
            return this->memory_ - 1;
        }
    };
}