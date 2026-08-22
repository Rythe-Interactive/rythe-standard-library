#pragma once
#include "../filesystem/filesystem_error.hpp"
#include "../time/time_span.hpp"

namespace rsl
{
    struct file_info
    {
        time_span lastWriteTimestamp;
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

    [[nodiscard]] [[rythe_always_inline]] constexpr bool mode_available_for_write(file_access_mode mode) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr bool mode_available_for_append(file_access_mode mode) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr bool mode_available_for_read(file_access_mode mode) noexcept;

    RYTHE_DECLARE_NATIVE_API_TYPE(file)
    namespace internal
    {
        [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_mode(file&, file_access_mode) noexcept;
        [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_flags(file&, file_access_flags) noexcept;
    } // namespace internal

    class file
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(file);

        [[nodiscard]] bool operator==(const file& other) const;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const file& other) const;

        [[nodiscard]] [[rythe_always_inline]] file_access_mode get_mode() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] file_access_flags get_flags() const noexcept;
        void close();

        [[nodiscard]] [[rythe_always_inline]] operator bool() const noexcept;

    private:
        RYTHE_NATIVE_API_TYPE_PRIVATE_MEMBERS(file)
        friend void ::rsl::internal::set_file_access_mode(file&, file_access_mode) noexcept;
        friend void ::rsl::internal::set_file_access_flags(file&, file_access_flags) noexcept;

        file_access_mode m_accessMode;
        file_access_flags m_accessFlags;
    };

    RYTHE_DECLARE_NATIVE_API_TYPE(file_mapping)
    namespace internal
    {
        [[maybe_unused]] [[rythe_always_inline]] static void set_file_mapping_file(file_mapping&, file) noexcept;
        [[maybe_unused]] [[rythe_always_inline]] static void set_file_mapping_view(file_mapping&, mutable_byte_view) noexcept;
    }

    class file_mapping
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(file_mapping);

        [[nodiscard]] bool operator==(const file_mapping& other) const;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const file_mapping& other) const;

        [[nodiscard]] [[rythe_always_inline]] file_access_mode get_mode() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] file_access_flags get_flags() const noexcept;
        void release();

        [[nodiscard]] [[rythe_always_inline]] result<byte_view> view_read() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] result<mutable_byte_view> view_write() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] operator bool() const noexcept;

    private:
        RYTHE_NATIVE_API_TYPE_PRIVATE_MEMBERS(file_mapping)
        friend void ::rsl::internal::set_file_mapping_file(file_mapping&, file) noexcept;
        friend void ::rsl::internal::set_file_mapping_view(file_mapping&, mutable_byte_view) noexcept;

        mutable_byte_view m_view;
        file m_file;
    };
}

#include "file.inl"
