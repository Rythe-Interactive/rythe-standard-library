#pragma once

namespace rsl::math
{
    // sin
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto sin(T&& value) noexcept;

    // asin
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto asin(T&& value) noexcept;
} // namespace rsl::math

#include "sin.inl"
