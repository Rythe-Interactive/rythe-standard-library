#pragma once
#include "views.hpp"

namespace rsl
{
    template <size_type N>
    struct string_literal
    {
        const char (&value)[N];

        constexpr string_literal(const char (&str)[N]) noexcept
            : value(str)
        {
        }

        constexpr string_literal(const string_literal& src) noexcept
            : value(src.value)
        {
        }
        constexpr string_literal(string_literal&& src) noexcept
            : value(src.value)
        {
        }

        constexpr string_literal& operator=(const string_literal& src) noexcept { value = src.value; return *this; }
        constexpr string_literal& operator=(string_literal&& src) noexcept { value = src.value; return *this; }

        static constexpr size_type size() noexcept { return N - 1; }

        constexpr operator rsl::cstring() const noexcept { return value; }
        constexpr operator rsl::string_view() const noexcept { return string_view(value, size()); }
    };

    template <size_type N>
    string_literal(const char (&)[N]) -> string_literal<N>;
}
