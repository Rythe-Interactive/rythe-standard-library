#pragma once
#include "predefined.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 1, 1, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type row_count = 1;
        static constexpr size_type col_count = 1;
        static constexpr size_type size = row_count * col_count;
        static constexpr storage_mode mode = Mode;

        using col_type = vector<scalar, row_count, Mode>;

        union
        {
            col_type columns[col_count];
            scalar data[size];
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;

        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;

        template <typename MatType>
            requires not_same_as<Scalar, typename MatType::scalar> || (MatType::row_count != 1) ||
                     (MatType::col_count != 1)
        [[rythe_always_inline]] constexpr matrix(const MatType& other) noexcept;

        [[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;

        [[rythe_always_inline]] constexpr operator scalar() const noexcept { return data[0]; }
    };

    template <storage_mode Mode>
    struct matrix<bool, 1, 1, Mode>
    {
        using scalar = bool;
        static constexpr size_type row_count = 1;
        static constexpr size_type col_count = 1;
        static constexpr size_type size = row_count * col_count;
        static constexpr storage_mode mode = Mode;

        using col_type = vector<scalar, row_count, Mode>;

        union
        {
            col_type columns[col_count];
            scalar data[size];
        };

        static const matrix identity;
        static const matrix zero;

        [[rythe_always_inline]] constexpr matrix() noexcept;

        [[rythe_always_inline]] constexpr matrix(const matrix&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, uniform_matrix_signal) noexcept;
        [[rythe_always_inline]] explicit constexpr matrix(scalar s, identity_matrix_signal) noexcept;

        template <typename MatType>
            requires not_same_as<bool, typename MatType::scalar> || (MatType::row_count != 1) ||
                     (MatType::col_count != 1)
        [[rythe_always_inline]] constexpr matrix(const MatType& other) noexcept;

        [[rythe_always_inline]] constexpr matrix& operator=(const matrix&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr col_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const col_type& operator[](size_type i) const noexcept;

        [[rythe_always_inline]] constexpr operator scalar() const noexcept { return data[0]; }
    };

    using bool1x1 = matrix<bool, 1, 1>;
    using float1x1 = matrix<float32, 1, 1>;
    using double1x1 = matrix<float64, 1, 1>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
