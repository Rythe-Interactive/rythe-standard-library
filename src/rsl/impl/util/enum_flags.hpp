#pragma once

#include "concepts.hpp"

namespace rsl::enum_flags
{
    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType combine_flags(
            EnumType first,
            EnumType second,
            EnumTypes... rest
            ) noexcept;

    template <enum_type EnumType>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool has_flag(EnumType value, EnumType flag) noexcept
    {
        return (value & flag) != 0;
    }

    template <enum_type EnumType>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool has_all_flags(EnumType value, EnumType flag) noexcept
    {
        return (value & flag) == flag;
    }

    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool has_any_flags(
            EnumType value,
            EnumType first,
            EnumType second,
            EnumTypes... rest
            ) noexcept
    {
        EnumType flag = combine_flags(first, second, rest...);
        return (value & flag) != 0;
    }

    template <enum_type EnumType>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType invert_flag(EnumType flag) noexcept
    {
        return ~flag;
    }

    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType unflag(EnumType value, EnumType first, EnumTypes... rest) noexcept
    {
        return value & ~(first | ... | rest);
    }

    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType combine_flags(EnumType first, EnumType second, EnumTypes... rest) noexcept
    {
        return first | (rest | ... | second);
    }

    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType exclude_flags(EnumType first, EnumType second, EnumTypes... rest) noexcept
    {
        return first & (rest & ... & second);
    }

    template <enum_type EnumType, same_as<EnumType>... EnumTypes>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType toggle_flags(EnumType first, EnumType second, EnumTypes... rest) noexcept
    {
        return first ^ (second ^ ... ^ rest);
    }

    template <enum_type EnumType>
    [[nodiscard]] [[rythe_always_inline]] constexpr EnumType set_flag(EnumType value, EnumType flag, bool isSet = true) noexcept
    {
        if (isSet != has_flag(value, flag))
        {
            return toggle_flags(value, flag);
        }

        return value;
    }
}
