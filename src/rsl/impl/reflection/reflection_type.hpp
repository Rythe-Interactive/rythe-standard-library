#pragma once

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
}
