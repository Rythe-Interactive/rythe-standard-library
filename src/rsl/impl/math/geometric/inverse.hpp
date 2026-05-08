#pragma once
#include "../../util/primitives.hpp"
#include "../arithmetic/division.hpp"
#include "../arithmetic/negate.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] auto inverse(const VecType& v) noexcept;

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] auto inverse(const QuatType& q) noexcept;

    template <matrix_type MatType>
    [[nodiscard]] [[rythe_always_inline]] auto inverse(const MatType& m) noexcept;
} // namespace rsl::math

#include "inverse.inl"
