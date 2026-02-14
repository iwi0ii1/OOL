#pragma once

#include "../base/iterators.hpp"

namespace asl::containers {
    // Just hashmap
    template<typename K, typename V>
    class hashmap final : public base::iterator_common<K*, hashmap> {
    public:
        // TODO: Hashmap
    };
}