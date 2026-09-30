#pragma once

namespace rsl::rfl
{
    template <typename FieldType>
    class field_builder
    {
    public:
        template <typename AttribType>
        field_builder add_attribute(AttribType value = {});
    };
} // namespace rsl::rfl
