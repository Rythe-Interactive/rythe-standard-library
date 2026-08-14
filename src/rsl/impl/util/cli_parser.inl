#pragma once

namespace rsl
{
    template <same_as<pair<string_view, string_view>>... Names>
    void cli_parser::add_params(Names... names)
    {
        (add_param(names.first, names.second), ...);
    }
} // namespace rsl
