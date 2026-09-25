#pragma once
#include "../containers/string.hpp"

namespace rsl::rfl
{
    class type_builder;

    class namespace_builder
    {
    public:
        type_builder add_type(string_view name);
        namespace_builder add_namespace(string_view name);

    };
}
