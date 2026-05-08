#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_equality;

    template <matrix_type MatrixType>
    struct compute_equality<MatrixType>
    {
        using result_type = matrix<bool, MatrixType::row_count, MatrixType::col_count, MatrixType::mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const MatrixType& a, const MatrixType& b) noexcept
        {
            result_type result;

            for (size_type i = 0; i < MatrixType::size; i++)
            {
                result.data[i] = close_enough(a.data[i], b.data[i]);
            }

            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const MatrixType& a, const typename MatrixType::col_type& b) noexcept
        {
            result_type result;
            for (size_type i = 0; i < MatrixType::col_count; i++)
            {
                result[i] = compute_equality<typename MatrixType::col_type>(a[i], b);
            }
            return result;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const MatrixType& a, typename MatrixType::scalar b) noexcept
        {
            result_type result;
            for (size_type i = 0; i < MatrixType::size; i++)
            {
                result.data[i] = close_enough(a.data[i], b);
            }
            return result;
        }
    };

    template <arithmetic_type Scalar, storage_mode Mode>
    struct compute_equality<matrix<Scalar, 1, 1, Mode>>
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(Scalar a, Scalar b) noexcept
        {
            return close_enough(a, b);
        }
    };
} // namespace rsl::math::internal
