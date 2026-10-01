#pragma once
#include "../containers/views.hpp"

namespace rsl::rfl
{
    template<typename T>
    class field_builder;
    template <typename T>
    class function_builder;

    enum struct access_spec_type
    {
        public_access,
        private_access,
        protected_access,
    };

    class type_builder
    {
    public:
        template<typename T>
        type_builder add_attribute(T value = {});

        template<typename T>
        type_builder add_field(string_view name, field_builder<T> field, access_spec_type accessSpec = access_spec_type::public_access);

        template <typename T>
        type_builder add_function(string_view name, function_builder<T> function, access_spec_type accessSpec = access_spec_type::public_access);
    };

    struct attribute_node
    {
        const void*(*get)();
        id_type typeId;
    };

    struct field_node {};
    struct function_node {};

    struct type_node
    {
        array_view<const attribute_node> attributes;
        array_view<const type_node> types;
        array_view<const field_node> fields;
        array_view<const function_node> functions;
    };
}

namespace rsl
{
    template <typename T>
    struct type_info;
}
