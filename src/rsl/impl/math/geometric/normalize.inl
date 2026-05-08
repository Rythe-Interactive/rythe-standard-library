#pragma once
#include "normalize.hpp"

#include "normalize_quaternion.inl"
#include "normalize_vector.inl"

namespace rsl::math
{
    template <vector_type VecType>
    constexpr auto normalize(const VecType& v) noexcept
    {
        return internal::compute_normalize<VecType>::compute(v);
    }

    template <quat_type QuatType>
    constexpr auto normalize(const QuatType& q) noexcept
    {
        return internal::compute_normalize<QuatType>::compute(q);
    }
} // namespace rsl::math
