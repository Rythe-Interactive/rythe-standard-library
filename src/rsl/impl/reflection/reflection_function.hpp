#pragma once

namespace rsl::rfl
{
    template <typename FuncType>
    class function_builder
    {
    public:
        template <typename AttribType>
        function_builder add_attribute(AttribType value = {});
    };
} // namespace rsl::rfl
