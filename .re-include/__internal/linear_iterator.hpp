#pragma once

#include <type_traits>

namespace asl::__internal {
    enum class direction : bool { forward = false, reversed = true };

    // Linear iterator (contiguous memory)
    // @param _drc Direction (forward / reversed)
    // @param _pointer The ptr to iterate over
    template<direction _drc, typename _pointer>
    requires std::is_pointer_v<_pointer>
    class linear_iterator final {
    private:
        using __l_type = linear_iterator<_drc, _pointer>;
        using __l_rtype = __l_type&;
        using __l_crtype = const __l_type&;

    public:
        _pointer base_ptr;

        // Make sure to give the correct memory. Else... UB is your fault.
        explicit constexpr linear_iterator(_pointer _memory_to_iterate) noexcept : base_ptr(_memory_to_iterate) {}


        constexpr explicit operator bool() const noexcept {
            return base_ptr != nullptr;
        }



        constexpr bool operator==(__l_crtype _other) const noexcept {
            return base_ptr == _other.base_ptr;
        }

        constexpr bool operator!=(__l_crtype _other) const noexcept {
            return base_ptr != _other.base_ptr;
        }



        constexpr bool operator<(__l_crtype _other) const noexcept {
            return base_ptr < _other.base_ptr;
        }

        constexpr bool operator<=(__l_crtype _other) const noexcept {
            return base_ptr <= _other.base_ptr;
        }

        constexpr bool operator>(__l_crtype _other) const noexcept {
            return base_ptr > _other.base_ptr;
        }

        constexpr bool operator>=(__l_crtype _other) const noexcept {
            return base_ptr >= _other.base_ptr;
        }



        constexpr __l_type operator+(const std::ptrdiff_t _number) const noexcept {
            return __l_type(_drc == direction::forward ? base_ptr + _number : base_ptr - _number);
        }

        constexpr __l_type operator-(const std::ptrdiff_t _number) const noexcept {
            return __l_type(_drc == direction::forward ? base_ptr - _number : base_ptr + _number);
        }

        constexpr std::ptrdiff_t operator-(__l_crtype _other) const noexcept {
            return _drc == direction::forward ? base_ptr - _other.base_ptr : _other.base_ptr - base_ptr;
        }

        constexpr __l_rtype operator+=(const std::ptrdiff_t _number) noexcept {
            direction::forward ? base_ptr += _number : base_ptr -= _number;
            return *this;
        }

        constexpr __l_rtype operator-=(const std::ptrdiff_t _number) noexcept {
            direction::forward ? base_ptr -= _number : base_ptr += _number;
            return *this;
        }


        
        constexpr __l_rtype operator++() noexcept {
            _drc == direction::forward ? base_ptr++ : base_ptr--;
            return *this;
        }

        constexpr __l_rtype operator--() noexcept {
            _drc == direction::forward ? base_ptr-- : base_ptr++;
            return *this;
        }



        constexpr std::remove_pointer_t<_pointer>& operator*() const noexcept {
            return *base_ptr;
        }

        constexpr _pointer operator->() const noexcept {
            return base_ptr;
        }
    };
}