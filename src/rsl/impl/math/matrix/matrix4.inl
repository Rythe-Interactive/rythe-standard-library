#pragma once
#include "matrix4.hpp"

#define ROW_COUNT_SPECIALIZATION 4
#define COL_COUNT_SPECIALIZATION 4
#define MATRIX_TEMPLATE template <arithmetic_type SCALAR_SPECIALIZATION, storage_mode MODE_SPECIALIZATION>

#include "matrix_shared.inl"

#define SCALAR_SPECIALIZATION bool
#define ROW_COUNT_SPECIALIZATION 4
#define COL_COUNT_SPECIALIZATION 4
#define MATRIX_TEMPLATE template <storage_mode MODE_SPECIALIZATION>

#include "matrix_shared.inl"

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    constexpr matrix<Scalar, 4, 4, Mode>::matrix(
        scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
        scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
    ) noexcept
        : columns()
    {
        columns[0] = col_type(s00, s10, s20, s30);
        columns[1] = col_type(s01, s11, s21, s31);
        columns[2] = col_type(s02, s12, s22, s32);
        columns[3] = col_type(s03, s13, s23, s33);
    }

    template <arithmetic_type Scalar, storage_mode Mode>
    constexpr matrix<Scalar, 4, 4, Mode>::matrix(col_type c0, col_type c1, col_type c2, col_type c3) noexcept
        : columns()
    {
        columns[0] = c0;
        columns[1] = c1;
        columns[2] = c2;
        columns[3] = c3;
    }

    template <storage_mode Mode>
    constexpr matrix<bool, 4, 4, Mode>::matrix(
        scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
        scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
    ) noexcept
        : columns()
    {
        columns[0] = col_type(s00, s10, s20, s30);
        columns[1] = col_type(s01, s11, s21, s31);
        columns[2] = col_type(s02, s12, s22, s32);
        columns[3] = col_type(s03, s13, s23, s33);
    }

    template <storage_mode Mode>
    constexpr matrix<bool, 4, 4, Mode>::matrix(col_type c0, col_type c1, col_type c2, col_type c3) noexcept
        : columns()
    {
        columns[0] = c0;
        columns[1] = c1;
        columns[2] = c2;
        columns[3] = c3;
    }
} // namespace rsl::math
