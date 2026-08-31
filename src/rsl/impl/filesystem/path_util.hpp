#pragma once
#include "../rsl_core.hpp"

#include "../util/string_util.hpp"

namespace rsl::fs
{
    [[nodiscard]] [[rythe_always_inline]] constexpr char separator() noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr char anti_separator() noexcept;

    struct separator_char
    {
        [[nodiscard]] [[rythe_always_inline]] bool operator==(const separator_char) const noexcept { return true; }
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const separator_char) const noexcept { return false; }

        [[nodiscard]] [[rythe_always_inline]] bool operator==(const char ch) const noexcept
        {
            return ch == '\\' || ch == '/';
        }

        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const char ch) const noexcept
        {
            return !(*this == ch);
        }

        [[nodiscard]] [[rythe_always_inline]] operator char() const noexcept
        {
            return separator();
        }
    };

    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string domain(string_view path);
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view parent(string_view path) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view filename(string_view path) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view extension(string_view path, bool fullExtension = false) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view stem(string_view path) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string subdir(string_view path, string_view sub);
    // Returns a standardized and sanitized path
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string sanitize(string_view path, bool failOnFsLeave = false);
    // Convert to local path conventions
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string localize(string_view path);
    // Convert to rythe standard path conventions
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string standardize(string_view path);
    template <string_like StringType>
    [[rythe_always_inline]] constexpr StringType& subdir(in_place_signal_type, StringType& path, string_view sub);
    template<string_like StringType>
    [[rythe_always_inline]] constexpr StringType& localize(in_place_signal_type, StringType& path) noexcept;
    template<string_like StringType>
    [[rythe_always_inline]] constexpr StringType& standardize(in_place_signal_type, StringType& path) noexcept;

    [[nodiscard]] [[rythe_always_inline]] constexpr string_view strip_domain(string_view path) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string replace_domain(string_view path, string_view replacement) noexcept;
    template<string_like StringType>
    [[rythe_always_inline]] constexpr StringType&
            replace_domain(in_place_signal_type, StringType& path, string_view replacement) noexcept;
}

#include "path_util.inl"
