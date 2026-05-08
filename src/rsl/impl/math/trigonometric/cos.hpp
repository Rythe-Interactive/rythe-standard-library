#pragma once
#include "../../util/primitives.hpp"
#include "../basic/constraint.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

#include <cmath>

namespace rsl::math
{
    // cos
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto cos(T&& value) noexcept;

    // acos
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto acos(T&& value) noexcept;
} // namespace rsl::math

#include "cos.inl"
