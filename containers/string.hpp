#pragma once

#include "../base/contiguous_storage.hpp"
#include <algorithm>

namespace asl::containers {
    #pragma region Basic string
    // String
    template<base::char_like _char_type>
    class basic_string final : public base::contiguous_storage<_char_type> {
    private:
        using __l_base_type = base::contiguous_storage<_char_type>;
        using __l_self_type = basic_string<_char_type>;
        using __l_self_rtype = __l_self_type&;
        using __l_self_crtype = const __l_self_type&;

    public:
        

        #pragma region Setup
        


        #pragma endregion
    };

    #pragma endregion
}