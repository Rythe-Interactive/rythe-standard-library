#pragma once
#include "../matrix/determinant.hpp"
#include "../exponential/sqrt.hpp"

namespace rsl::math
{
    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length(const VecType& v) noexcept;

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto rcp_length(const VecType& v) noexcept;

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const VecType& v) noexcept;

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length(const QuatType& q) noexcept;

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto rcp_length(const QuatType& q) noexcept;

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const QuatType& q) noexcept;

    template <matrix_type MatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length(const MatType& m) noexcept;

    template <matrix_type MatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto rcp_length(const MatType& m) noexcept;

    template <matrix_type MatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto length2(const MatType& m) noexcept;
} // namespace rsl::math

#include "length.inl"
