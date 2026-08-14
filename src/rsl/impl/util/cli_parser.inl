#pragma once

namespace rsl
{
    template <string_like... Names>
    void cli_parser::add_params(Names&&... names)
    {
        (add_param(view_from_stringish(rsl::forward<Names>(names))), ...);
    }
} // namespace rsl
