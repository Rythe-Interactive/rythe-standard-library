#pragma once
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 4, 4, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type row_count = 4;
        static constexpr size_type col_count = 4;
        static constexpr size_type size = row_count * col_count;
        static constexpr storage_mode mode = Mode;

        using col_type = vector<scalar, row_count, Mode>;

        template <size_type Idx>
        using row_type = row<scalar, row_count, col_count, Idx, Mode>;

        union
        {
            col_type columns[col_count];

            struct
            {
                col_type col0, col1, col2, col3;
            };

            scalar data[size];
            row_type<0> row0;
            row_type<1> row1;
            row_type<2> row2;
            row_type<3> row3;
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;
        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(
            scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
            scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
        ) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(col_type c0, col_type c1, col_type c2, col_type c3) noexcept;

        template <
            arithmetic_type Scal0, math::storage_mode M0, arithmetic_type Scal1 = Scalar, math::storage_mode M1 = Mode>
        [[rythe_always_inline]] explicit constexpr matrix(
            const quaternion<Scal0, M0>& orientation,
            const vector<Scal1, 3, M1>& position = vector<Scal1, 3, M1>{0, 0, 0}
        ) noexcept;

        template <typename MatType>
            requires not_same_as<Scalar, typename MatType::scalar> || (MatType::row_count != 4) ||
                     (MatType::col_count != 4)
        [[rythe_always_inline]] constexpr matrix(const MatType& other) noexcept;

        [[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;
    };

    template <storage_mode Mode>
    struct matrix<bool, 4, 4, Mode>
    {
        using scalar = bool;
        static constexpr size_type row_count = 4;
        static constexpr size_type col_count = 4;
        static constexpr size_type size = row_count * col_count;
        static constexpr storage_mode mode = Mode;

        using col_type = vector<scalar, row_count, Mode>;

        template <size_type Idx>
        using row_type = row<scalar, row_count, col_count, Idx, Mode>;

        union
        {
            col_type columns[col_count];

            struct
            {
                col_type col0, col1, col2, col3;
            };

            scalar data[size];
            row_type<0> row0;
            row_type<1> row1;
            row_type<2> row2;
            row_type<3> row3;
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;
        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(
            scalar s00, scalar s01, scalar s02, scalar s03, scalar s10, scalar s11, scalar s12, scalar s13, scalar s20,
            scalar s21, scalar s22, scalar s23, scalar s30, scalar s31, scalar s32, scalar s33
        ) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(col_type c0, col_type c1, col_type c2, col_type c3) noexcept;

        template <arithmetic_type Scal0, math::storage_mode M0, arithmetic_type Scal1, math::storage_mode M1>
        [[rythe_always_inline]] explicit constexpr matrix(
            const quaternion<Scal0, M0>& orientation, const vector<Scal1, 3, M1>& position = vector<Scal1, 3, M1>::zero
        ) noexcept;

        template <typename MatType>
            requires not_same_as<bool, typename MatType::scalar> || (MatType::row_count != 4) ||
                     (MatType::col_count != 4)
        [[rythe_always_inline]] constexpr matrix(const MatType& other) noexcept;

        [[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept
        {
            bool result = true;
            for (size_type i = 0; i < size; ++i)
            {
                result &= data[i];
            }
            return result;
        }
    };

    using bool4x4 = matrix<bool, 4, 4>;
    using float4x4 = matrix<float32, 4, 4>;
    using double4x4 = matrix<float64, 4, 4>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
