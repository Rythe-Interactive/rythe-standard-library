#pragma once

namespace rsl
{
    #if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL)
    #define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE)
    #define RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class dynamic_library
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(dynamic_library);

        [[nodiscard]] bool operator==(const dynamic_library& other) const;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const dynamic_library& other) const { return !(*this == other); }

        template <typename T>
        [[nodiscard]] [[rythe_always_inline]] T get_symbol(cstring symbolName) const;

        [[nodiscard]] [[rythe_always_inline]] operator bool() const { return m_handle; }

        [[rythe_always_inline]] void release();

    private:
        using platform_specific_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;

        friend class platform;
    };
}
