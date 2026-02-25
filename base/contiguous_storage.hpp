#pragma once

#include "./object.hpp"
#include <cstring>
#include <memory>
#include <iterator>

namespace asl::base {
    // Contiguous container engine
    // @note Provided common methods for contiguous containers
    template<a_regular_value T>
    requires storage_compatible<T>
    class contiguous_storage : protected object {
    protected:
        size_t used_slots_ = 0;
        size_t slots_ = 0;

        T* data_ = nullptr;

        inline contiguous_storage() = default;

        inline ~contiguous_storage() {
            std::destroy_n(data_, used_slots_);
            ::operator delete(data_);
            data_ = nullptr;
        }

        void __l_fn_realloc(const size_t slots_number);


    public:
        using iterator = T*;
        using const_iterator = const T*;
        using reversed_iterator = std::reverse_iterator<T*>;
        using const_reversed_iterator = std::reverse_iterator<const T*>;


        #pragma region Details
        inline size_t size() const noexcept {
            return used_slots_;
        }

        inline size_t slot() const noexcept {
            return slots_;
        }

        inline bool empty() const noexcept {
            return used_slots_ == 0;
        }

        inline T* data() noexcept {
            return data_;
        }

        inline T& front() noexcept {
            return data_[0];
        }

        inline T& back() noexcept {
            return data_[used_slots_ - 1];
        }

        inline bool starts_with(T v) const noexcept {
            return !empty() && front() == v;
        }

        inline bool ends_with(T v) const noexcept {
            return !empty() && back() == v;
        }



        inline T& operator[](const size_t index) noexcept {
            return data_[index];
        }

        inline T& at(const size_t index) {
            if (index >= used_slots_)
                throw std::out_of_range("Out of range: Index: " + std::to_string(index));

            return data_[index];
        }






        iterator begin() noexcept {
            return data_;
        }

        iterator end() noexcept {
            return data_ + used_slots_;
        }

        const_iterator cbegin() const noexcept {
            return data_;
        }

        const_iterator cend() const noexcept {
            return data_ + used_slots_;
        }

        reversed_iterator rbegin() noexcept {
            return reversed_iterator(data_);
        }

        reverse_iterator rend() noexcept {
            return reversed_iterator(data_ + used_slots_);
        }

        const_reversed_iterator crbegin() const noexcept {
            return const_reversed_iterator(data_);
        }

        const_reversed_iterator crend() const noexcept {
            return const_reversed_iterator(data_ + used_slots_);
        }
        #pragma endregion


        #pragma region Mutators
        inline void reserve(const size_t n) {
            __l_fn_realloc(slots_ + n);
        }

        inline void resize(const size_t n) {
            __l_fn_realloc(n);
        }

        inline void cancel_extra_slot() {
            __l_fn_realloc(used_slots_);
        }






        inline iterator insert(const size_t pos, const T& val) {
            if (used_slots_ >= slots_ || slots_ == 0)
                reserve(1);

            std::move_backward(data_ + start, data_ + used_slots_, data_ + used_slots_ + 1);

            data_[start] = value;
            used_slots_ += 1;
            return &data_[start];
        }

        inline void erase(const size_t index, const bool left_shift = true) {
            if (index >= used_slots_)
                    throw std::out_of_range("Index out of range...");

            if (left_shift)
                std::move(data_ + index + 1, data_ + used_slots_, data_ + index);

            used_slots_ -= 1;
            if constexpr (!std::is_trivially_destructible_v<_data_type>)
                data_[used_slots_].~_data_type();
        }

        inline void clear() {
            std::destroy_n(data_, used_slots_);
            used_slots_ = 0;
        }
        #pragma endregion        
    };







    template<a_regular_value T>
    requires storage_compatible<T>
    void contiguous_storage<T>::__l_fn_realloc(const size_t slots_number) {
        T* new_memory = static_cast<T*>(
            ::operator new(slots_number * sizeof(T)) // Here causes the problem
        );

        // This is smart (🗿)
        // - Transfers either amount of used slots or asked slots.
        const size_t elements_to_transfer = std::min(used_slots_, slots_number);

        if constexpr (std::is_trivial_v<T>)
            std::memmove(new_memory, data_, elements_to_transfer * sizeof(T));
        else if constexpr (std::is_move_constructible_v<T>)
            std::uninitialized_move_n(data_, elements_to_transfer, new_memory);
        else if constexpr (std::is_copy_constructible_v<T>)
            std::uninitialized_copy_n(data_, elements_to_transfer, new_memory);            

        std::destroy_n(data_, elements_to_transfer);
        ::operator delete(data_);

        data_ = new_memory;
        slots_ = slots_number;
        used_slots_ = elements_to_transfer;
    }
}