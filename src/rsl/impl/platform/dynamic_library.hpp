#pragma once
#include "../defines.hpp"

namespace rsl
{
    DECLARE_NATIVE_API_TYPE(dynamic_library)

    class dynamic_library
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(dynamic_library);

        [[nodiscard]] bool operator==(const dynamic_library& other) const;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const dynamic_library& other) const { return !(*this == other); }

        template <typename T>
        [[nodiscard]] [[rythe_always_inline]] T get_symbol(cstring symbolName) const;

        [[nodiscard]] [[rythe_always_inline]] operator bool() const { return m_handle != native_dynamic_library::invalid; }

        [[rythe_always_inline]] void release();

    private:
        NATIVE_API_TYPE_PRIVATE_MEMBERS(dynamic_library)
    };
}
