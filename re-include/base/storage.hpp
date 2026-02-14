#pragma once

#include "./base/iterators.hpp"
#include <cstring>

namespace asl::base {
    // Base class for raw memory and used memory management.
    // @note Provides allocation, resizing, etc.
    class storage : public base::object {
    protected:
        size_t size_ = 0;
        size_t capacity_ = 0;
        void* memory_ = nullptr;

    public:
        // Get raw memory (constant).
        const void* const mem() const noexcept {
            return memory_;
        }

        // Get size.
        size_t size() const noexcept {
            return size_;
        }

        // Check if empty.
        bool empty() const noexcept {
            return size_ == 0;
        }

        // Get allocated memory.
        size_t capacity() const noexcept {
            return capacity_;
        }

        // Allocate more memory.
        void reserve(const size_t add_size) {
            size_t new_capacity = capacity_ + add_size;
            void* new_mem = ::operator new(new_capacity);

            // Copy memory into new memory.
            if (memory_ != nullptr) {
                std::memcpy(new_mem, memory_, capacity_);
                ::operator delete(memory_);
            }
            memory_ = new_mem;
            capacity_ = new_capacity;
        }
        
        // Reallocate memory.
        void resize(const size_t new_size = size_) {
            void* new_mem = ::operator new(new_size);

            // Copy memory into new memory.
            if (memory_ != nullptr) {
                std::memcpy(new_mem, memory_, new_size);
                ::operator delete(memory_);
            }
            memory_ = new_mem;
            capacity = new_size;
       }
}