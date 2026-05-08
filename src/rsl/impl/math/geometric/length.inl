#pragma once
#include "length.hpp"

#include "length_quat.inl"
#include "length_vector.inl"

namespace rsl::math
{
    template <vector_type VecType>
    constexpr auto length(const VecType& v) noexcept
    {
        return internal::compute_length<VecType>::compute(v);
    }

    template <vector_type VecType>
    constexpr auto rcp_length(const VecType& v) noexcept
    {
        return internal::compute_length<VecType>::compute_rcp(v);
    }

    template <vector_type VecType>
    constexpr auto length2(const VecType& v) noexcept
    {
        return internal::compute_length<VecType>::compute2(v);
    }

    template <quat_type QuatType>
    constexpr auto length(const QuatType& q) noexcept
    {
        return internal::compute_length<QuatType>::compute(q);
    }

    template <quat_type QuatType>
    constexpr auto rcp_length(const QuatType& q) noexcept
    {
        return internal::compute_length<QuatType>::compute_rcp(q);
    }

    template <quat_type QuatType>
    constexpr auto length2(const QuatType& q) noexcept
    {
        return internal::compute_length<QuatType>::compute2(q);
    }

    template <matrix_type MatType>
    constexpr auto length(const MatType& m) noexcept
    {
        return determinant(m);
    }

    template <matrix_type MatType>
    constexpr auto rcp_length(const MatType& m) noexcept
    {
        return rpc(determinant(m));
    }

    template <matrix_type MatType>
    constexpr auto length2(const MatType& m) noexcept
    {
        auto det = determinant(m);
        return det * det;
    }
} // namespace rsl::math
