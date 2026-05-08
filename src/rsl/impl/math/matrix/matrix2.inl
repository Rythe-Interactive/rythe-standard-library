#pragma once
#include "matrix2.hpp"

#define ROW_COUNT_SPECIALIZATION 2
#define COL_COUNT_SPECIALIZATION 2
#define SCALAR_SPECIALIZATION Scalar
#define MATRIX_TEMPLATE template <arithmetic_type Scalar, storage_mode Mode>

#include "matrix_shared.inl"

#define ROW_COUNT_SPECIALIZATION 2
#define COL_COUNT_SPECIALIZATION 2
#define SCALAR_SPECIALIZATION bool
#define MATRIX_TEMPLATE template <storage_mode Mode>

#include "matrix_shared.inl"

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    constexpr matrix<Scalar, 2, 2, Mode>::matrix(scalar s00, scalar s01, scalar s10, scalar s11) noexcept
        : col0(s00, s10),
          col1(s01, s11)
    {
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    constexpr matrix<Scalar, 2, 2, Mode>::matrix(col_type c0, col_type c1) noexcept
        : col0(c0),
          col1(c1)
    {
    }

    template <storage_mode Mode>
    constexpr matrix<bool, 2, 2, Mode>::matrix(scalar s00, scalar s01, scalar s10, scalar s11) noexcept
        : col0(s00, s10),
          col1(s01, s11)
    {
    }

    template <storage_mode Mode>
    constexpr matrix<bool, 2, 2, Mode>::matrix(col_type c0, col_type c1) noexcept
        : col0(c0),
          col1(c1)
    {
    }
} // namespace rsl::math
