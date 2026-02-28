/*
std::optional<T> equivalent
*/

#pragma once
#include "../base/custom_concepts.hpp"

namespace asl::value_wrappers {

    // Just a type that says smth doesn't exist
    class nulling final {};

    // Just smth to represent smth absence
    const nulling null;



    // A type, but might not contain any values
    template<typename T>
    class nullable final {
    private:
        T value_;
        bool has_value_;

        using __l_self_rtype = nullable<T>&;

    public:
        #pragma region Setup
        // Does nothing bruh...
        inline explicit nullable(nulling = null) noexcept : value_(), has_value_(false) {}

        // Constructor for <T>
        template<typename... Args>
        requires std::is_constructible_v<T, Args&&...>
        inline explicit nullable(Args&&... args) : value_(std::forward<Args>(args)...), has_value_(true) {}


        // No more value
        constexpr __l_self_rtype operator=(nulling) noexcept {
            has_value_ = false;
            return *this;
        }

        // Guess value will be back
        inline __l_self_rtype operator=(const T& new_value) noexcept {
            has_value_ = true;
            value_ = new_value;
            return *this;
        }
        #pragma endregion



        #pragma region Access
        inline bool has_value() const noexcept {
            return has_value_;
        }

        /** @example
         * nullable<string> huh{"Bruh"};
         * string hello = huh; // Implicitly return `const string&` 
         * 
        **/
        // Implicit converted access
        // @note Throws if no value
        inline operator const T&() const {
            if (!has_value_)
                throw std::runtime_error("asl::value_wrappers::nullable<T>:: operator const T&(): Has no value... cannot access.");

            return value_;
        }

        // Direct access
        // @note Throws if no value
        inline T& operator*() {
            if (!has_value_)
                throw std::runtime_error("asl::value_wrappers::nullable<T>::operator*(): Has no value... cannot access.");

            return value_;
        }

        // Direct access
        // @note Throws if no value
        inline T& value() {
            if (!has_value_)
                throw std::runtime_error("asl::value_wrappers::nullable<T>::value(): Has no value... cannot access.");

            return value_;
        }



        // Access OR other value
        // @note Returns a copy, slightly more expensive
        inline T value_or(const T& other) const& noexcept { // That weird const& means this method can only be called on an lvalue
            return !has_value_ ? other : value_;
        }

        // Access OR other value
        // @note Returns a copy, slightly more expensive
        inline T value_or(T&& move_other) && noexcept { // This weird syntax means this method can only be called on an rvalue
            return !has_value_ ? std::move(move_other) : value_;
        }
        #pragma endregion
    };
}