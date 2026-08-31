#pragma once
#include "../platform/file.hpp"
#include "../util/error_handling.hpp"

#include "filesystem_error.hpp"
#include "traits.hpp"
#include "archive.hpp"

namespace rsl::fs
{
    class view;
    class archive;

    class file_solution final
    {
        friend class archive_registry;
        friend class archive;

    public:
        RULE_OF_5(file_solution)

        [[nodiscard]] [[rythe_always_inline]] pointer<const archive> get_provider() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] pointer<archive> get_provider() noexcept;
        [[rythe_always_inline]] void release();

        [[nodiscard]] [[rythe_always_inline]] bool is_valid() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] operator bool() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] file_traits file_info() const;

        [[nodiscard]] [[rythe_always_inline]] bool is_file() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_directory() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_empty() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_valid_path() const;
        [[nodiscard]] [[rythe_always_inline]] bool can_be_written() const;
        [[nodiscard]] [[rythe_always_inline]] bool can_be_read() const;
        [[nodiscard]] [[rythe_always_inline]] bool can_be_created() const;
        [[nodiscard]] [[rythe_always_inline]] bool exists() const;

        [[nodiscard]] [[rythe_always_inline]] filesystem_traits filesystem_info() const;
        [[nodiscard]] result<dynamic_array<view>> ls() const;

        [[rythe_always_inline]] void set_access_hint(file_access_flags flags);
        [[nodiscard]] [[rythe_always_inline]] result<void> create() const;
        [[nodiscard]] [[rythe_always_inline]] result<void> delete_entry(file_delete_flags flags) const;
        [[nodiscard]] [[rythe_always_inline]] result<byte_view> read() const;
        [[nodiscard]] [[rythe_always_inline]] result<void> write(byte_view data);
        [[nodiscard]] [[rythe_always_inline]] result<void> append(byte_view data);
        [[nodiscard]] [[rythe_always_inline]] result<void> flush() const;

    protected:
        file_solution_handle m_handle = invalid_file_solution_handle;
        pointer<archive> m_provider = { nullptr };
    };
} // namespace rsl::fs

#include "file_solution.inl"
