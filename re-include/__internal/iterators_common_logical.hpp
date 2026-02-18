#pragma once

#include <type_traits>

namespace asl::__internal {
    // Inherit this, and ur iterator will have common logical operators.
    // @attention Derived only needs to implement `operator++` and `operator--` itself.
    // @note Derived must have public `.base_ptr` as pointer
    template<typename __derived>
    class iterators_common_logical {
    private:
        using __cr_drv = const __derived&; // Const ref derived

        // Returns the derived
        constexpr __derived& self() noexcept {
            return static_cast<__derived&>(*this);
        }

    public:
        constexpr explicit operator bool() noexcept {
            return self().base_ptr != nullptr;
        }





        constexpr bool operator==(__cr_drv ___o) noexcept {
            return self().base_ptr == ___o.base_ptr;
        }

        constexpr bool operator!=(__cr_drv ___o) noexcept {
            return self().base_ptr != ___o.base_ptr;
        }





        constexpr bool operator<(__cr_drv ___o) noexcept {
            return self().base_ptr < ___o.base_ptr;
        }

        constexpr bool operator<=(__cr_drv ___o) noexcept {
            return self().base_ptr <= ___o.base_ptr;
        }

        constexpr bool operator>(__cr_drv ___o) noexcept {
            return self().base_ptr > ___o.base_ptr;
        }

        constexpr bool operator>=(__cr_drv ___o) noexcept {
            return self().base_ptr >= ___o.base_ptr;
        }





        // Forward
        constexpr __derived operator+(const std::ptrdiff_t __n) noexcept {
            return __derived(self().base_ptr + __n);
        }

        // Forward assign
        constexpr __derived& operator+=(const std::ptrdiff_t __n) noexcept {
            self().base_ptr += __n;
            return self();
        }

        // Backward
        constexpr __derived operator-(const std::ptrdiff_t __n) noexcept {
            return __derived(self().base_ptr - __n);
        }

        // Difference
        constexpr std::ptrdiff_t operator-(__cr_drv ___o) noexcept {
            return self().base_ptr - ___o.base_ptr;
        }

        // Backward assign
        constexpr __derived operator-=(const std::ptrdiff_t __n) noexcept {
            self().base_ptr -= __n;
            return self();
        }






        // Dereference
        constexpr auto& operator*() noexcept {
            return *(self().base_ptr);
        }
    };
}