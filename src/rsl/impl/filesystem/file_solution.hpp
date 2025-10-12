#pragma once
#include "../util/error_handling.hpp"

#include "archive.hpp"
#include "filesystem_error.hpp"
#include "traits.hpp"

namespace rsl::filesystem
{
    class view;

    class file_solution
    {
        friend class archive;

    public:
        VIRTUAL_RULE_OF_5(file_solution)

        [[nodiscard]] [[rythe_always_inline]] const archive* get_provider() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] archive* get_provider() noexcept;
        [[rythe_always_inline]] void release();

        [[nodiscard]] file_traits file_info() const;

        [[nodiscard]] virtual bool is_file() const = 0;
        [[nodiscard]] virtual bool is_directory() const = 0;
        [[nodiscard]] virtual bool is_valid_path() const = 0;
        [[nodiscard]] virtual bool can_be_written() const = 0;
        [[nodiscard]] virtual bool can_be_read() const = 0;
        [[nodiscard]] virtual bool can_be_created() const = 0;
        [[nodiscard]] virtual bool exists() const = 0;

        [[nodiscard]] [[rythe_always_inline]] filesystem_traits filesystem_info() const;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;

        [[nodiscard]] virtual result<byte_view> read() const = 0;
        [[nodiscard]] virtual result<void> write(byte_view data) = 0;
        [[nodiscard]] virtual result<void> append(byte_view data) = 0;
        [[nodiscard]] virtual result<void> flush() const = 0;

    protected:
        [[nodiscard]] result<void> open_file_for_read() const;
        [[nodiscard]] result<void> open_file_for_write();
        [[nodiscard]] result<void> close_file() const;
        [[nodiscard]] result<void> flush_file() const;

        archive* m_provider;
    };
}

#include "file_solution.inl"
