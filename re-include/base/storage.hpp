#pragma once

#include "./object.hpp"
#include <cstring>
#include <memory>

namespace asl::base {
    // Base class for raw memory and used memory management.
    // @note Provides allocation, resizing, etc.
    template<a_pointer _memory_type>
    class storage : public base::object {
    private:
        _memory_type* memory_ = nullptr;

    protected:
        size_t slot_ = 0;
        size_t unused_slot_ = 0;

        // Only moves trivial / non-trivial memory to a specific destination.
        // @warning Already freed old memory.
        void move_memory(_memory_type* _dest) const noexcept {
            if constexpr (std::is_trivial_v<_memory_type>)
                std::memmove(_dest, memory_, unused_slot_);

            else if constexpr (std::is_move_constructible_v<_memory_type>)
                std::uninitialized_move_n(memory_, unused_slot_, _dest);

            else if constexpr (std::is_copy_constructible_v<_memory_type>)
                std::uninitialized_copy_n(memory_, unused_slot_, _dest);

            else
                static_assert(false, "Non-trivial type doesn't have copyable or movable constructor...");

            std::destroy_n(memory_, unused_slot_);
            ::operator delete(memory_);
        }

    public:
        // Get unused slots / size.
        constexpr size_t size() const noexcept {
            return unused_slot_;
        }

        // Check if empty slots and size
        constexpr bool empty() const noexcept {
            return slot_ == 0 && unused_slot_ == 0;
        }

        // Get total slots
        constexpr size_t slot() const noexcept {
            return slot_;
        }

        // Get total raw bytes allocated
        constexpr size_t total_byte() const noexcept {
            return slot_ * sizeof(_memory_type);
        }

        // Reserve more slots
        void reserve(const size_t add_slots) {
            const size_t latest_total_bytes = (slot_ + add_slots) * sizeof(_memory_type);
            
            _memory_type* new_temporary_memory = ::operator new(latest_total_bytes);

            move_memory(new_temporary_memory);

            memory_ = new_temporary_memory;
            slot_ = latest_total_bytes / sizeof(_memory_type);
        }

        // Set specific amount of slots
        // @note Default to the total used slots (size)
        void resize(const size_t this_slots) {
            _memory_type* new_temporary_memory = ::operator new(this_slots * sizeof(_memory_type));

            move_memory(new_temporary_memory);

            memory_ = new_temporary_memory;
            slot_ = this_slots;
        }

        // Cancel extra reserved slots
        void cancel_extra_slots() {
            _memory_type* new_temporary_memory = ::operator new(unused_slot_ * sizeof(_memory_type));

            move_memory(new_temporary_memory);

            memory_ = new_temporary_memory;
            slot_ = this_slots;
        }

        
        // Get raw memory
        const _memory_type* raw_memory() const noexcept {
            return memory_;
        }
        
        // Check if slots are unexpectedly lesser than unused slots.
        bool inaccurate() const noexcept {
            return slot_ < unused_slot_;
        }
    };
}