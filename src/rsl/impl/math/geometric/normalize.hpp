#pragma once
#include "../arithmetic/division.hpp"

namespace rsl::math
{
    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto normalize(const VecType& v) noexcept;

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto normalize(const QuatType& q) noexcept;
} // namespace rsl::math

#include "normalize.inl"
