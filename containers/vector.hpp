#pragma once

#include "../base/contiguous_storage.hpp"

namespace asl::containers {
    // Vector / Dynamic array
    template<typename T>
    class vector final : public base::contiguous_storage<T> {
    private:
        using __l_base_type = base::contiguous_storage<T>;
        using __l_self_type = vector<T>;
        using __l_self_rtype = __l_type&;
        using __l_self_crtype = const __l_type&;

    public:
        using typename __l_base_type::iterator;
        using typename __l_base_type::const_iterator;
        using typename __l_base_type::reversed_iterator;
        using typename __l_base_type::const_reversed_iterator;


        #pragma region Setup
        // Default constructor
        constexpr vector() noexcept {}


        // @param n Reserve slots
        explicit vector(size_t n) {
            reallocate(n);
            used_slots_ = n;
        }






        // Move ctor
        vector(vector&&) = default;

        // Move assign
        __l_rtype operator=(vector&&) = default;






        // Construct
        vector(__l_self_crtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
        }


        // You know what this does if you know `std::vector<T>::operator=()`
        __l_rtype operator=(__l_self_rtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
            return *this;
        }







        // Construct by iterators
        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        template<typename it_t = iterator>
        vector(it_t start, it_t end) {
            const size_t count = end - start;
            reallocate(count);
            std::copy(start, end, memory_);
            used_slots_ = count;
        }


        


        // @param one_element The element to spawn in this container
        // @param count How many times to spawn it
        vector(const T& one_element, size_t count = 1) {
            reallocate(count);
            for (size_t i = 0; i < count; i++)
                memory_[i] = one_element;

            used_slots_ = count;
        }

        #pragma endregion



        #pragma region Iterators

        iterator begin() const noexcept {
            return iterator(memory_);
        }

        iterator end() const noexcept {
            return iterator(memory_ + used_slots_);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(memory_);
        }

        const_iterator cend() const noexcept {
            return const_iterator(memory_ + used_slots_);
        }



        reversed_iterator rbegin() const noexcept {
            return reversed_iterator(memory_ + used_slots_ - 1);
        }

        reversed_iterator rend() const noexcept {
            return reversed_iterator(memory_ - 1);
        }

        const_reversed_iterator crbegin() const noexcept {
            return const_reversed_iterator(memory_ + used_slots_ - 1);
        }

        const_reversed_iterator crend() const noexcept {
            return const_reversed_iterator(memory_ - 1);
        }

        #pragma endregion




        #pragma region Mutators


        #pragma endregion

    };
}