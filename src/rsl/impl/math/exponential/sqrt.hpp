#pragma once

namespace rsl::math
{
    template <arithmetic_type Scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr Scalar sqrt(Scalar value) noexcept;

    template <floating_point_type Scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr Scalar rcp_sqrt(Scalar value) noexcept;

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto sqrt(const VecType& v) noexcept;

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto rcp_sqrt(const VecType& v) noexcept;
} // namespace rsl::math

#include "sqrt.inl"
