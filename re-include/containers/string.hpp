#pragma once

#include "../base/iterators.hpp"
#include "../base/storage.hpp"

namespace asl::containers {
    // String
    // @param _char_type Char type (e.g., char, char16_t, wchar_t)
    template<typename _char_type>
    class string final : public base::iterator_common<_char_type*, string>, base::storage<char*> {
    public:
        
        
    };
}