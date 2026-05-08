#pragma once
#include "multiplication.hpp"

namespace rsl::math::internal
{
    template <typename T>
    struct compute_multiplication;

    template <typename Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
    struct compute_multiplication<matrix<Scalar, RowCount, ColCount, Mode>>
    {
        using mat_type = matrix<Scalar, RowCount, ColCount, Mode>;
        using vec_type = typename mat_type::col_type;

        template <matrix_type OtherType>
        [[nodiscard]] constexpr static auto compute(const mat_type& a, const OtherType& b) noexcept
        {
            static_assert(
                ColCount == OtherType::row_count, "Matrix/matrix multiplication requires the column count of the "
                                                  "first to be the same as the row count of the second."
            );

            using scalar = lowest_precision_t<Scalar, typename OtherType::scalar>;
            using result_type = matrix<scalar, RowCount, OtherType::col_count>;

            result_type result{0};

            for (size_type i = 0; i < RowCount; i++)
            {
                for (size_type j = 0; j < OtherType::col_count; j++)
                {
                    for (size_type k = 0; k < OtherType::row_count; k++)
                    {
                        result[i][j] += a[i][k] * b[k][j];
                    }
                }
            }

            return result;
        }

        [[nodiscard]] constexpr static auto compute(const vec_type& a, const mat_type& b) noexcept
        {
            static_assert(
                ColCount == vec_type::size,
                "Matrix/vector multiplication requires vector to have the same size as the number of columns"
            );
            using scalar =
                lowest_precision_t<::std::remove_cvref_t<Scalar>, ::std::remove_cvref_t<typename vec_type::scalar>>;
            using result_type = vector<scalar, RowCount>;

            result_type result{0};

            for (size_type i = 0; i < RowCount; i++)
            {
                for (size_type j = 0; j < ColCount; j++)
                {
                    result[i] += b[j][i] * a[j];
                }
            }

            return result;
        }
    };
} // namespace rsl::math::internal
