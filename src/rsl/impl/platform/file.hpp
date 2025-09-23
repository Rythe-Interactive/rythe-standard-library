#pragma once
#include "../time/date_time.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    struct file_info
    {
        time::date lastWriteTimestamp;
        size_type size;
        bool isWritable;
        bool isDirectory;
        bool isFile;
    };

    enum struct [[rythe_closed_enum]] file_access_mode : uint8
    {
        read,
        exclusive_read,
        write_shared_read,
        write,
        append,
        read_write_append,
    };

    enum struct [[rythe_open_enum]] file_access_flags : uint8
    {
        no_preference = 0,
        async = 1 << 0,
        random = 1 << 1,
        sequential = 1 << 2,
    };
    RYTHE_BIT_FLAG_OPERATORS(file_access_flags)

    enum struct [[rythe_open_enum]] file_delete_flags : uint8
    {
        none      = 0,
        force     = 1 << 0,
        recursive = 1 << 1,
    };
    RYTHE_BIT_FLAG_OPERATORS(file_delete_flags)

    #if !defined(RYTHE_NATIVE_FILE_HANDLE_IMPL)
    #define RYTHE_NATIVE_FILE_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE)
    #define RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class file
    {
        using platform_specific_handle = RYTHE_NATIVE_FILE_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE;

        friend class platform;

    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(file);

        bool operator==(const file& other) const;
        bool operator!=(const file& other) const { return !(*this == other); }

        [[nodiscard]] [[rythe_always_inline]] file_access_mode get_mode() const noexcept { return m_accessMode; }
        [[nodiscard]] [[rythe_always_inline]] file_access_flags get_flags() const noexcept { return m_accessFlags; }

    private:
        file_access_mode m_accessMode;
        file_access_flags m_accessFlags;
    };
}
