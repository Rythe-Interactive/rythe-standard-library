#pragma once
#include "../containers/string.hpp"

namespace rsl::rfl
{
    class type_builder;
    template <typename T>
    class field_builder;
    template <typename T>
    class function_builder;

    class namespace_builder
    {
    public:
        namespace_builder add_type(string_view name, type_builder type);
        namespace_builder add_namespace(string_view name);

        template <typename T>
        type_builder add_field(string_view name, field_builder<T> field);

        template <typename T>
        type_builder add_function(string_view name, function_builder<T> function);

    };
}
