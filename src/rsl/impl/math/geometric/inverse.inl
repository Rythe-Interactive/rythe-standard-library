#pragma once
#include "inverse.hpp"

#include "inverse_matrix.inl"
#include "inverse_quat.inl"
#include "inverse_vector.inl"

namespace rsl::math
{
    template <vector_type VecType>
    [[nodiscard]] auto inverse(const VecType& v) noexcept
    {
        return internal::compute_inverse<VecType>::compute(v);
    }

    template <quat_type QuatType>
    [[nodiscard]] auto inverse(const QuatType& q) noexcept
    {
        return internal::compute_inverse<QuatType>::compute(q);
    }

    template <matrix_type MatType>
    [[nodiscard]] auto inverse(const MatType& m) noexcept
    {
        return internal::compute_inverse<MatType>::compute(m);
    }
} // namespace rsl::math
