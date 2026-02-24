/*
The file you will see every objects inherit from.







 Choices:
  - Statically type deduced, is base of object -> accepted.
  - Another way...?
*/

#ifndef TYPES_OBJECT_HPP
#define TYPES_OBJECT_HPP

inline namespace asl {
    namespace types {

        // Type inherited by all objects from this library
        class object {
        public:
            object() {

            }

            object operator=(object obj) noexcept {

            }

            virtual ~object() = default;
        };
    }
}

#endif