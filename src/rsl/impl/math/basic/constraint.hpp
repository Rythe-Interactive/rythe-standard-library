#pragma once
#include "../../defines.hpp"

namespace rsl::math
{
    template <typename A, typename B>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto min(A&& a, B&& b) noexcept;

    template <typename A, typename B>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto max(A&& a, B&& b) noexcept;

    template <typename In, typename Min, typename Max>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto clamp(In&& in, Min&& min, Max&& max) noexcept;

    template <typename In>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto saturate(In&& in) noexcept;
} // namespace rsl::math

#include "constraint.inl"
