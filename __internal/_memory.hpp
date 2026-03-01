/*
Internal memory utilities
*/

#pragma once
#include <type_traits>
#include <cstring>
#include <memory>


namespace asl::__internal {
    inline namespace _memory {
        // Copy trivial / non-trivial elements to an uninitialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter>
        requires std::is_pointer_v<Ptr>
        Ptr uninitialized_copy_elements(Iter __f, Iter __l, Ptr __dest) {
            using rm_ptr_t = std::remove_pointer_t<Ptr>;
            if constexpr (std::is_trivial_v<rm_ptr_t>)
                std::memmove(__dest, __f, (__l - __f) * sizeof(rm_ptr_t));
            else if constexpr (std::is_copy_constructible_v<rm_ptr_t>)
                std::uninitialized_copy(__f, __l, __dest);
            else
                static_assert(false, "Ptr underlying type must be either trivial or copy constructible.");

            return __dest;
        }

        // Copy trivial / non-trivial elements to uninitialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter, typename Size>
        Ptr uninitialized_copy_elements_n(Iter __f, Size __n, Ptr __dest) {
            return uninitialized_copy_elements(__f, __f + __n, __dest);
        }





        // Move trivial / non-trivial elements to uninitialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter>
        requires std::is_pointer_v<Ptr>
        Ptr uninitialized_move_elements(Iter __f, Iter __l, Ptr __dest) {
            using rm_ptr_t = std::remove_pointer_t<Ptr>;
            if constexpr (std::is_trivial_v<rm_ptr_t>)
                std::memmove(__dest, __f, (__l - __f) * sizeof(rm_ptr_t));
            else if constexpr (std::is_move_constructible_v<rm_ptr_t>)
                std::uninitialized_move(__f, __l, __dest);
            else
                static_assert(false, "Ptr underlying type must be either trivial or move constructible.");

            return __dest;
        }

        // Move trivial / non-trivial elements to uninitialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter, typename Size>
        Ptr uninitialized_move_elements_n(Iter __f, Size __n, Ptr __dest) {
            return uninitialized_move_elements(__f, __f + __n, __dest);
        }

        



        // Copy trivial / non-trivial elements to initialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter>
        requires std::is_pointer_v<Ptr>
        Ptr copy_elements(Iter __f, Iter __l, Ptr __dest) {
            using rm_ptr_t = std::remove_pointer_t<Ptr>;
            if constexpr (std::is_trivial_v<rm_ptr_t>)
                std::memmove(__dest, __f, (__l - __f) * sizeof(rm_ptr_t));
            else if constexpr (std::is_copy_constructible_v<rm_ptr_t>)
                std::copy(__f, __l, __dest);
            
            return __dest;
        }

        // Copy trivial / non-trivial elements to initialized memory.
        // @return The pointer to the destination
        // @note Will throw if smth failed
        template<typename Ptr, typename Iter, typename Size>
        Ptr copy_elements_n(Iter __f, Size __n, Ptr __dest) {
            return copy_elements(__f, __f + __n, __dest);
        }
    }
}