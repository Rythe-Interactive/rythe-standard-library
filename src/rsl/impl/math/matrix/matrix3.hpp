#pragma once
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 3, 3, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type row_count = 3;
        static constexpr size_type col_count = 3;
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
                col_type col0, col1, col2;
            };

            scalar data[size];
            row_type<0> row0;
            row_type<1> row1;
            row_type<2> row2;
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;
        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(
            scalar s00, scalar s01, scalar s02, scalar s10, scalar s11, scalar s12, scalar s20, scalar s21, scalar s22
        ) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(col_type c0, col_type c1, col_type c2) noexcept;

        template <arithmetic_type Scal, math::storage_mode M>
        [[rythe_always_inline]] explicit constexpr matrix(const quaternion<Scal, M>& orientation) noexcept;

        template <typename MatType>
            requires not_same_as<Scalar, typename MatType::scalar> || (MatType::row_count != 3) ||
                     (MatType::col_count != 3)
        [[rythe_always_inline]] constexpr matrix(const MatType& other) noexcept;

        [[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;
    };

    template <storage_mode Mode>
    struct matrix<bool, 3, 3, Mode>
    {
        using scalar = bool;
        static constexpr size_type row_count = 3;
        static constexpr size_type col_count = 3;
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
                col_type col0, col1, col2;
            };

            scalar data[size];
            row_type<0> row0;
            row_type<1> row1;
            row_type<2> row2;
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;
        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(
            scalar s00, scalar s01, scalar s02, scalar s10, scalar s11, scalar s12, scalar s20, scalar s21, scalar s22
        ) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(col_type c0, col_type c1, col_type c2) noexcept;

        template <arithmetic_type Scal, math::storage_mode M>
        [[rythe_always_inline]] explicit constexpr matrix(const quaternion<Scal, M>& orientation) noexcept;

        template <typename MatType>
            requires not_same_as<bool, typename MatType::scalar> || (MatType::row_count != 3) ||
                     (MatType::col_count != 3)
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

    using bool3x3 = matrix<bool, 3, 3>;
    using float3x3 = matrix<float32, 3, 3>;
    using double3x3 = matrix<float64, 3, 3>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
