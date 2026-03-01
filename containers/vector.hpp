#pragma once

#include "../base/contiguous_storage.hpp"

namespace asl::containers {
    // Vector / Dynamic array
    template<typename T>
    class vector final : public base::contiguous_storage<T> {
    private:
        using __l_base_type = base::contiguous_storage<T>;
        using __l_self_type = vector<T>;
        using __l_self_rtype = __l_self_type&;
        using __l_self_crtype = const __l_self_type&;

    public:
        using typename __l_base_type::iterator;
        using typename __l_base_type::const_iterator;
        using typename __l_base_type::reversed_iterator;
        using typename __l_base_type::const_reversed_iterator;


        #pragma region Setup
        // Default constructor
        constexpr vector() noexcept {}


        // Reserve slots
        // @param n Slots required
        vector(size_t n) {
            this->__l_fn_realloc(n);
        }






        // Move ctor
        vector(vector&&) = default;

        // Move assign
        __l_self_rtype operator=(vector&&) = default;






        // Construct by another one
        vector(__l_self_crtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
        }


        // Assign by another one
        __l_self_rtype operator=(__l_self_rtype other) {
            this->__l_fn_realloc(other.used_slots_);
            std::copy_n(other.data_, other.used_slots_, this->data_);
            return *this;
        }







        // Construct by iterators
        // @param start Start iterator (.begin)
        // @param end End iterator (.end)
        explicit vector(const_iterator first, const_iterator last) {
            const size_t count = first - last;
            this->__l_fn_realloc(count);
            std::uninitialized_copy(first, last, this->data_);
            this->used_slots_ = count;
        }


        


        // Fill in with `one_element`
        // @param one_element The element to spawn in this container
        // @param count How many times to spawn it
        vector(const T& one_element, const size_t count = 1) {
            this->__l_fn_realloc(count);
            std::uninitialized_fill_n(this->data_, count, one_element);
            this->used_slots_ = count;
        }






        // Construct by initializer-list
        vector(const std::initializer_list<T>& il) {
            this->__l_fn_realloc(il.size());
            std::uninitialized_copy(il.begin(), il.end(), this->data_);
            this->used_slots_ = il.size();
        }

        // Assign by initializer-list
        __l_self_rtype operator=(const std::initializer_list<T>& il) {
            this->__l_fn_realloc(il.size());
            std::uninitialized_copy(il.begin(), il.end(), this->data_);
            this->used_slots_ = il.size();
            return *this;
        }

        #pragma endregion
        
        // No mutators or methods specifically for `vector<T>`
    };
}