#pragma once
#include "../time/date_time.hpp"

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

    DECLARE_NATIVE_API_TYPE(file)
    [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_mode(file&, file_access_mode) noexcept;
    [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_flags(file&, file_access_flags) noexcept;

    class file
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(file);

        bool operator==(const file& other) const;
        bool operator!=(const file& other) const { return !(*this == other); }

        [[nodiscard]] [[rythe_always_inline]] file_access_mode get_mode() const noexcept { return m_accessMode; }
        [[nodiscard]] [[rythe_always_inline]] file_access_flags get_flags() const noexcept { return m_accessFlags; }

        [[nodiscard]] [[rythe_always_inline]] operator bool() const { return m_handle != native_file::invalid; }

    private:
        NATIVE_API_TYPE_PRIVATE_MEMBERS(file)
        friend void set_file_access_mode(file&, file_access_mode) noexcept;
        friend void set_file_access_flags(file&, file_access_flags) noexcept;

        file_access_mode m_accessMode;
        file_access_flags m_accessFlags;
    };
}
