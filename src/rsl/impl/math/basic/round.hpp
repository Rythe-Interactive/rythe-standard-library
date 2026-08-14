#pragma once
#include "../../rsl_core.hpp"

namespace rsl::math
{
    enum struct round_mode
    {
        floor,
        round,
        ceil,
        trunc
    };

    template <typename Target, round_mode Mode = round_mode::round, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto adv_round(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto round(T&& val) noexcept;

    template <typename Integer = int, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto iround(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto uround(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto ceil(T&& val) noexcept;

    template <typename Integer = int, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto iceil(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto uceil(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto floor(T&& val) noexcept;

    template <typename Integer = int, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto ifloor(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto ufloor(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto trunc(T&& val) noexcept;

    template <typename Integer = int, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto itrunc(T&& val) noexcept;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto utrunc(T&& val) noexcept;
} // namespace rsl::math

#include "round.inl"
