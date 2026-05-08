#pragma once
#include "../../util/primitives.hpp"
#include "../basic/constraint.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    // map
    template <typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    map(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept;

    template <typename T, typename InMin, typename InMax>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto map01(T&& value, InMin&& inMin, InMax&& inMax) noexcept;
} // namespace rsl::math

#include "map.inl"
