#pragma once
#include "container_util.hpp"

#include "../containers/array.hpp"
#include "../containers/string.hpp"
#include "../containers/views.hpp"

namespace rsl
{
    [[nodiscard]] [[rythe_always_inline]] constexpr bool is_white_space(char c) noexcept;

    template <weakly_equality_comparable_with<char> TokenType>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_array<string_view> split_string_at(
            string_view str,
            const TokenType& token
            ) noexcept;

    template <char Token, char... Tokens>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_array<string_view> split_string_at(string_view str) noexcept;

    template <contiguous_container_like Range>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string join_strings_with(
            const Range& elements,
            string_view delimiters
            ) noexcept;
    template <const char* const Delim, contiguous_container_like Range>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string join_strings_with(const Range& elements) noexcept;
    template <contiguous_container_like Range>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string join_strings_with(
            const Range& elements,
            char delimiter
            ) noexcept;
    template <const char Delimiter, contiguous_container_like Range>
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string join_strings_with(const Range& elements) noexcept;

    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_left(string_view str) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_right(string_view str) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim(string_view str) noexcept;

    template <weakly_equality_comparable_with<char> C>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_left(string_view str, const C& key) noexcept;
    template <weakly_equality_comparable_with<char> C>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_right(string_view str, const C& key) noexcept;
    template <weakly_equality_comparable_with<char> C>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim(string_view str, const C& key) noexcept;

    template <invocable<bool(char)> Func>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_left(string_view str, Func&& comparer) noexcept;
    template <invocable<bool(char)> Func>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim_right(string_view str, Func&& comparer) noexcept;
    template <invocable<bool(char)> Func>
    [[nodiscard]] [[rythe_always_inline]] constexpr string_view trim(string_view str, Func&& comparer) noexcept;
} // namespace rsl

#include "string_util.inl"
