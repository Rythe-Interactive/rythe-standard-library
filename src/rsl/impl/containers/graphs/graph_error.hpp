#include "../../util/error_handling.hpp"

namespace rsl
{
    enum struct [[rythe_closed_enum]] graph_error : errc
    {
        no_error = no_error_code,
        no_root,
    };

    template <>
    constexpr string_view default_error_message<graph_error>(const graph_error err)
    {
        switch (err)
        {
            case graph_error::no_error: return "No error."_sv;
            case graph_error::no_root: return "No root found."_sv;
            default: return "Unknown graph_error.";
        }
    }
}
