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

        storage() = default; // Constructible by derived

        // Just frees memory so containers don't need to free.
        ~storage() {
            this->free();
        }

    public:

        #pragma region Detail

        using memory_type = decltype(memory_);

        // Get used slots / size.
        constexpr size_t size() const noexcept {
            return used_slots_;
        }

        // Check if empty slots and size
        constexpr bool empty() const noexcept {
            return slots_ == 0 && used_slots_ == 0;
        }

        // Get total slots
        constexpr size_t slot() const noexcept {
            return slots_;
        }

        // Get total raw bytes allocated
        constexpr size_t total_byte() const noexcept {
            return slots_ * sizeof(_memory_type);
        }

        // Get raw memory
        constexpr _memory_type* data() noexcept {
            return memory_;
        }
        
        // Access element (doesn't count '\0')
        // @note Might UB 😨
        // @note Use `.at()` if prefer throwing
        constexpr _memory_type& operator[](const size_t index) noexcept {
            return memory_[index];
        }

        // Access element (doesn't count '\0')
        // @note Maybe no segfault, thx to throwing
        inline _memory_type& at(const size_t index) {
            if (index >= used_slots_)
                throw std::out_of_range("Index out of range.\nSize: " + std::to_string(used_slots_) + "\nGiven index: " + std::to_string(index));
            return memory_[index];
        }

        // First element
        // @note Do not dereference if memory empty
        constexpr _memory_type& front() noexcept {
            return memory_[0];
        }

        // Last element
        // @note Do not dereference if memory empty
        constexpr _memory_type& back() noexcept {
            return memory_[used_slots_ - 1];
        }

        // The name already tells you what this does, lol
        constexpr bool starts_with(_memory_type __m) noexcept {
            return used_slots_ != 0 && memory_[0] == __m;
        }

        // The name already tells you what this does, lol
        constexpr bool ends_with(_memory_type __m) noexcept {
            return used_slots_ != 0 && memory_[used_slots_ - 1] == __m;
        }

        #pragma endregion




        #pragma region Mutators

        // Re-allocate by specific number of slots (slots will automatically be changed)
        // @note `Trim if less, extra slots if more` than used slots.
        // @warning When used for allocation, make sure to change `used_slots_` manually.
        inline void reallocate(const size_t slots_number) {
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
            reallocate(slots_ + add_slots);
        }

        // Set specific amount of slots
        // @note Default to the total used slots (size)
        inline void resize(const size_t this_slots) {
            reallocate(this_slots);
        }

        // Cancel extra reserved slots
        inline void cancel_extra_slot() {
            reallocate(used_slots_);
        }






        // Add a new element to the storage
        // @param elem The element to add
        inline void push_back(const _memory_type& elem) {
            if (used_slots_ >= slots_ || slots_ == 0)
                reserve(1);

            memory_[used_slots_++] = elem; // Compiler place each bytes safely
        }

        // Remove the last element (guarded, returns false if storage already empty)
        // @param rep Repetition of removing the last element
        // @note Doesn't remove slots (only calls destructor)
        inline bool pop_back(const size_t rep = 1) noexcept {
            if (rep > used_slots_)
                return false;

            for (size_t i = 0; i < rep && ; i++) {
                back().~_memory_type();
                --used_slots_;
            }
            return true;
        }

        #pragma endregion
    };
}