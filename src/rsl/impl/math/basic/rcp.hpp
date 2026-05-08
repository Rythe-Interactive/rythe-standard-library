#pragma once

namespace rsl::math
{
    template <floating_point_type Scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr Scalar rcp(Scalar value) noexcept;
}

#include "rcp.inl"
