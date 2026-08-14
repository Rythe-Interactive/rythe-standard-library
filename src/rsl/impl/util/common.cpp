#include "../rsl_core.hpp"

namespace  rsl::internal
{
    void* get_global_empty_type_placeholder_ptr()
    {
        static byte placeholder;
        return &placeholder;
    }
}
