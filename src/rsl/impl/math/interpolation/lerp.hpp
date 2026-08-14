#pragma once
#include "../../rsl_core.hpp"
#include "../basic/constraint.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion_base.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    // lerp
    template <typename TypeA, typename TypeB, typename InterpType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto lerp(TypeA&& a, TypeB&& b, InterpType&& t) noexcept;
} // namespace rsl::math

#include "lerp.inl"
