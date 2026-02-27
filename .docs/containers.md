# About containers
 - Best to `const` whenever possible as it can be compatible to both `const` and `non-const` contexts.
 - Avoid rewriting the same code for same kind of container. Add a new base class and let derived inherit from it. (e.g., `asl::base::contiguous_storage<T>`)