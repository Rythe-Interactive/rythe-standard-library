#pragma once
#include "../containers/string.hpp"

namespace rsl::rfl
{
    class namespace_builder;

    class reflection_registry
    {
    public:
        namespace_builder add_namespace(string_view name);

    };
}
