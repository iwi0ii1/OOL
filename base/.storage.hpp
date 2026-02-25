#pragma once

#include "./object.hpp"
#include <cstring>
#include <memory>
#include <iostream>

namespace asl::base {
    // Base class for raw memory and used memory management.
    // @note Provides allocation, resizing, etc.
    // @param _memory_type Give me regular type, I wrap it in a ptr (don't gimme ptr or ref)
    template<a_regular_value _memory_type>
    requires storage_compatible<_memory_type>
    class storage : protected base::object {
    protected:
        size_t slots_ = 0;
        size_t used_slots_ = 0;
        _memory_type* memory_ = nullptr;

        inline storage() = default; // Constructible by derived

        // Just frees memory so containers don't need to free.
        inline ~storage() {
            this->free();
        }


        /** @internal **/
        // Re-allocate by specific number of slots (slots will automatically be changed)
        // @note `Trim if less, extra slots if more` than used slots.
        // @warning When used for allocation, make sure to change `used_slots_` manually.
        void __l_fn_realloc(const size_t slots_number);

        /** @internal **/
        // Insert element at specific slot
        // @note Will not grow extra
        inline _memory_type* __l_fn_insert_at(const size_t start, const _memory_type& value) {
            if (used_slots_ >= slots_ || slots_ == 0)
                reserve(1);

            std::move_backward(memory_ + start, memory_ + used_slots_, memory_ + used_slots_ + 1);

            memory_[start] = value;
            used_slots_ += 1;
            return &memory_[start];
        }

        /** @internal **/
        // Remove element at specific position (will left shift all elements)
        // @note Doesn't reduce slots
        inline void __l_fn_erase_at(const size_t index, const bool no_left_shift = false) {
            if (index >= used_slots_)
                    throw std::out_of_range("Index out of range...");

            if (!no_left_shift)
                std::move(memory_ + index + 1, memory_ + used_slots_, memory_ + index);

            used_slots_ -= 1;
            if constexpr (!std::is_trivially_destructible_v<_memory_type>)
                memory_[used_slots_].~_memory_type();
        }

    public:

        #pragma region Detail

        using memory_type = decltype(memory_);

        // Get used slots / size.
        inline size_t size() const noexcept {
            return used_slots_;
        }

        // Check if empty slots and size
        inline bool empty() const noexcept {
            return slots_ == 0 && used_slots_ == 0;
        }

        // Get total slots
        inline size_t slot() const noexcept {
            return slots_;
        }

        // Get total raw bytes allocated
        inline size_t total_byte() const noexcept {
            return slots_ * sizeof(_memory_type);
        }

        // Get raw memory
        inline _memory_type* data() noexcept {
            return memory_;
        }

        // First element
        // @note Do not dereference if memory empty
        inline _memory_type& front() noexcept {
            return memory_[0];
        }

        // Last element
        // @note Do not dereference if memory empty
        inline _memory_type& back() noexcept {
            return memory_[used_slots_ - 1];
        }

        // The name already tells you what this does, lol
        inline bool starts_with(_memory_type __m) noexcept {
            return used_slots_ != 0 && memory_[0] == __m;
        }

        // The name already tells you what this does, lol
        inline bool ends_with(_memory_type __m) noexcept {
            return used_slots_ != 0 && memory_[used_slots_ - 1] == __m;
        }

        #pragma endregion




        #pragma region Mutators






        // Free memory and assign `nullptr`.
        // @note Clears slots
        inline void free() {
            std::destroy_n(memory_, used_slots_);
            ::operator delete(memory_);
            memory_ = nullptr;
            slots_ = 0;
            used_slots_ = 0;
        }

        // Clears elements
        // @note Does not clear slots (only calls destructors)
        inline void clear() noexcept {
            std::destroy_n(memory_, used_slots_);
            used_slots_ = 0;
        }






        // Reserve more slots
        inline void reserve(const size_t add_slots) {
            __l_fn_realloc(slots_ + add_slots);
        }

        // Set specific amount of slots
        // @note Default to the total used slots (size)
        inline void resize(const size_t this_slots) {
            __l_fn_realloc(this_slots);
        }

        // Cancel extra reserved slots
        inline void cancel_extra_slot() {
            __l_fn_realloc(used_slots_);
        }

        #pragma endregion
    };




    template<a_regular_value _memory_type>
    requires storage_compatible<_memory_type>
    void storage<_memory_type>::__l_fn_realloc(const size_t slots_number) {
        _memory_type* new_memory = static_cast<_memory_type*>(
            ::operator new(slots_number * sizeof(_memory_type)) // Here causes the problem
        );

        // This is smart (🗿)
        // - Transfers either amount of used slots or asked slots.
        const size_t elements_to_transfer = std::min(used_slots_, slots_number);

        if constexpr (std::is_trivial_v<_memory_type>)
            std::memmove(new_memory, memory_, elements_to_transfer * sizeof(_memory_type));
        else if constexpr (std::is_move_constructible_v<_memory_type>)
            std::uninitialized_move_n(memory_, elements_to_transfer, new_memory);
        else if constexpr (std::is_copy_constructible_v<_memory_type>)
            std::uninitialized_copy_n(memory_, elements_to_transfer, new_memory);            

        std::destroy_n(memory_, elements_to_transfer);
        ::operator delete(memory_);

        memory_ = new_memory;
        slots_ = slots_number;
        used_slots_ = elements_to_transfer;
    }
}
