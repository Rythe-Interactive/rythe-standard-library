#pragma once

namespace rsl::math
{
    // tan
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto tan(T&& value) noexcept;

    // atan
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto atan(T&& value) noexcept;

    template <typename X, typename Y>
        requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>> && (floating_point_type<X> || linear_algebraic_construct<X>)
    [[nodiscard]] [[rythe_always_inline]] constexpr auto atan2(X&& x, Y&& y) noexcept;
} // namespace rsl::math

#include "tan.inl"
