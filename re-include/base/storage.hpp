#pragma once

#include "./object.hpp"
#include <cstring>
#include <memory>

namespace asl::base {
    // Base class for raw memory and used memory management.
    // @note Provides allocation, resizing, etc.
    // @param _memory_type A regular value.
    template<a_regular_value _memory_type>
    class storage : public base::object {
    private:
        _memory_type* memory_ = nullptr;

    protected:
        size_t slots_ = 0;
        size_t used_slots_ = 0;

    public:

        #pragma region Setup

        storage() = default;

        storage(const size_t add_slots) {
            reserve(add_slots);
        }

        ~storage() {
            if (memory_ != nullptr) {
                std::destroy_n(memory_, used_slots_);
                ::operator delete(memory_);
            }
        }
        






        #pragma endregion



        #pragma region Check

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
        const _memory_type* raw_memory() const noexcept {
            return memory_;
        }
        
        // Check if slots are unexpectedly lesser than unused slots.
        bool inaccurate() const noexcept {
            return slots_ < used_slots_;
        }

        #pragma endregion




        #pragma region Modifiers

        // Re-allocate by specific number of slots (slots will automatically be changed)
        // @note `Trim if less, extra slots if more` than used slots.
        void reallocate(const size_t slots_number) {
            _memory_type* new_memory = static_cast<_memory_type*>(
                ::operator new(slots_number * sizeof(_memory_type))
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
            else
                static_assert(false, "Type must be move / copy constructible!");
            
            std::destroy_n(memory_, elements_to_transfer);
            ::operator delete(memory_);

            memory_ = new_memory;
            slots_ = slots_number;
            used_slots_ = elements_to_transfer;
        }

        // Reserve more slots
        void reserve(const size_t add_slots) {
            reallocate(slots_ + add_slots);
        }

        // Set specific amount of slots
        // @note Default to the total used slots (size)
        void resize(const size_t this_slots) {
            reallocate(this_slots);
        }

        // Cancel extra reserved slots
        void cancel_extra_slot() {
            reallcoate(used_slots_);
        }

        #pragma endregion
    };
}