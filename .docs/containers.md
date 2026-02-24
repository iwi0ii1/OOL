# About containers
 - All containers must have a unique name and best to inherit from `asl::base::storage` to get the common storage interface.
 - Containers can only alias `__l_type` / `__l_rtype` / `__l_ctype` / `__l_crtype` to avoid boilerplate code.
 - Containers must not exposed anything from `asl::__internal` namespace.
 - Containers must have iterators. And so `#include "asl/__internal/*_iterators.hpp"` in the container header file.
 - Containers do not need to free memory if inheriting from `asl::base::storage`.