#pragma once

#include "./custom_concepts.hpp"
//#include "../__internal/_memory.hpp"
#include <memory>
#include <iterator>

namespace asl::base {
    // Contiguous container engine
    // @note Provided common methods for contiguous containers
    template<a_regular_value T>
    requires storage_compatible<T>
    class contiguous_storage {
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
        using reversed_iterator = std::reverse_iterator<iterator>;
        using const_reversed_iterator = std::reverse_iterator<const_iterator>;


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

        inline const T* c_data() const noexcept {
            return data_;
        }

        inline T& front() noexcept {
            return data_[0];
        }

        inline T& back() noexcept {
            return data_[used_slots_ - 1];
        }

        inline bool starts_with(const T& v) const noexcept {
            return !empty() && front() == v;
        }

        inline bool ends_with(const T& v) const noexcept {
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






        inline iterator begin() noexcept {
            return data_;
        }

        inline iterator end() noexcept {
            return data_ + used_slots_;
        }

        inline const_iterator begin() const noexcept { // Overload for range-based for loop
            return data_;
        }

        inline const_iterator end() const noexcept { // Overload for range-based for loop
            return data_ + used_slots_;
        }

        inline const_iterator cbegin() const noexcept {
            return data_;
        }

        inline const_iterator cend() const noexcept {
            return data_ + used_slots_;
        }

        inline reversed_iterator rbegin() noexcept {
            return reversed_iterator(end());
        }

        inline reversed_iterator rend() noexcept {
            return reversed_iterator(begin());
        }

        inline const_reversed_iterator crbegin() const noexcept {
            return const_reversed_iterator(cend());
        }

        inline const_reversed_iterator crend() const noexcept {
            return const_reversed_iterator(cbegin());
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





        // Insert an element
        // @param pos Where to insert
        // @param val What to insert
        // @note Will allocate only 1 more slot if insufficient (no extra growth)
        inline iterator insert(const_iterator pos, const T& val) {
            if (used_slots_ >= slots_ || slots_ == 0)
                reserve(1);

            iterator* insert_pos = begin() + (pos - begin());

            if (insert_pos != end()) {
                std::move(insert_pos, end(), insert_pos + 1);
            }
            new (insert_pos) T(val);
            
            used_slots_ += 1;
            return insert_pos;
        }

        // Insert an element to the back
        // @param val Value to add
        // @param rep Repetition
        inline iterator push_back(const T& val, size_t rep = 1) {
            if (rep < 1)
                throw std::invalid_argument("asl::base::contiguous_storage<T>::push_back(...): Second parameter `rep` cannot be less than 1.");

            if (used_slots_ + rep > slots_)
                reserve(rep);

            const auto first_iterator = insert(end(), val);
            for (; rep - 1 > 1; --rep)
                insert(end(), val);

            return first_iterator;
        }

        // Insert a range of elements
        // @param pos Where to insert
        // @param first Start range of elements
        // @param last End range of elements
        // @note Will allocate more memory if insufficient (no extra growth)
        iterator insert(iterator pos, const_iterator first, const_iterator last);

        // Remove elements
        // @param first The first element to remove
        // @param last Until the last element to remove
        // @note Doesn't reduce slots
        inline void erase(const_iterator first, const_iterator last) {
            if (first == last)
                throw std::invalid_argument("asl::base::contiguous_storage<T>::erase(...): `first` iterator the same as `last` iterator.");
            
            const size_t idx_first = first - begin();
            const size_t idx_last = last - end();

            // Bring the "chosen ones" to overwrite starting from `first` (third parameter)
            // Returns the start of the ghosts (old objects whom dtors aren't called yet)
            auto ghosts_pos = std::move(begin() + idx_last, end(), begin() + idx_first);
            std::destroy(ghosts_pos, end()); // Runs dtor of ghosts

            used_slots_ -= last - first;
        }

        // Remove the last element
        // @param rep Removal repetition
        inline void pop_back(const size_t rep = 1) {
            erase(end() - rep, end());
        }

        // Clear elements
        // @note Doesn't reduce slots
        inline void clear() {
            std::destroy_n(data_, used_slots_);
            used_slots_ = 0;
        }
        #pragma endregion        
    };







    template<a_regular_value T>
    requires storage_compatible<T>
    void contiguous_storage<T>::__l_fn_realloc(const size_t slots_number) {
        // Does nothing if asked is the same as the current
        if (slots_number == slots_)
            return;

        // This is smart (🗿)
        // - Transfers either amount of used slots or asked slots.
        const size_t elements_to_transfer = std::min(used_slots_, slots_number);

        T* new_memory = static_cast<T*>(
            ::operator new(slots_number * sizeof(T)) // Here causes the problem
        );

        if constexpr (std::is_move_constructible_v<T>) {
            std::uninitialized_move_n(data_, elements_to_transfer, new_memory);
        } else {
            std::uninitialized_copy_n(data_, elements_to_transfer, new_memory);
            std::destroy_n(data_, elements_to_transfer); // Only call dtors if copied.
        }
        ::operator delete(data_);

        data_ = new_memory;
        slots_ = slots_number;
        used_slots_ = elements_to_transfer;
    }


    template<a_regular_value T>
    requires storage_compatible<T>
    contiguous_storage<T>::iterator contiguous_storage<T>::insert(iterator pos, const_iterator first, const_iterator last) {
        const size_t idx_first = first - begin();
        const size_t idx_last = last - begin();

        const size_t offset = pos - begin();
        const size_t range_diff = idx_last - idx_first;

        if (used_slots_ + range_diff > slots_ || slots_ == 0) {
            reserve(range_diff);
            pos = begin() + offset;
        }

        for (size_t i = 0; i < range_diff; i++)
            new (end() + i) T;

        std::move_backward(pos, end(), end() + range_diff);

        // first / last are const -> both const / non-const will work
        std::uninitialized_copy(begin() + idx_first, begin() + idx_last, pos);

        used_slots_ += range_diff;
        return pos;
    }
}