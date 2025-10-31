#pragma once
#include "../defines.hpp"

#include "../containers/string.hpp"
#include "../util/error_handling.hpp"

#include "path_util.hpp"
#include "traits.hpp"

namespace rsl::fs
{
    class file_solution;

    class view
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(view)

        constexpr view(string_view path) noexcept;
        constexpr view(dynamic_string&& path) noexcept;

        [[nodiscard]] [[rythe_always_inline]] operator bool() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] bool is_valid(bool deepCheck = false) const;

        [[rythe_always_inline]] result<void> prefetch_solution(bool ignoreMultipleSolutions = false) const;

        [[nodiscard]] file_traits file_info() const;
        [[nodiscard]] filesystem_traits filesystem_info() const;

        [[nodiscard]] [[rythe_always_inline]] dynamic_string domain() const;
        [[nodiscard]] [[rythe_always_inline]] string_view path() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] string_view extension(bool fullExtension = false) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] string_view filename() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] string_view stem() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] view parent() const;
        [[nodiscard]] [[rythe_always_inline]] view subdir(string_view identifier) const;
        [[nodiscard]] [[rythe_always_inline]] view operator/(string_view identifier) const;

        [[nodiscard]] result<dynamic_array<view>> ls() const;

        [[nodiscard]] result<byte_view> read() const;
        [[nodiscard]] result<void> write(byte_view data);
        [[nodiscard]] result<void> append(byte_view data);
        [[nodiscard]] result<void> flush() const;

    protected:
        void set_path(dynamic_string&& path);
        [[nodiscard]] const file_solution* find_solution() const;
        [[nodiscard]] file_solution* find_solution();

    private:

        dynamic_string m_path;
        mutable file_solution* m_solution = nullptr;
    };
}

namespace rsl
{
    constexpr fs::view operator""_fsview(const cstring str, const size_type len) noexcept
    {
        return fs::view(string_view::from_buffer(str, len));
    }
}

#include "view.inl"
