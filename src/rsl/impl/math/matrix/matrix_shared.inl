#ifndef ROW_COUNT_SPECIALIZATION
    #define ROW_COUNT_SPECIALIZATION RowCount
#endif // !ROW_COUNT_SPECIALIZATION

#ifndef COL_COUNT_SPECIALIZATION
    #define COL_COUNT_SPECIALIZATION ColCount
#endif // !COL_COUNT_SPECIALIZATION

#ifndef SCALAR_SPECIALIZATION
    #define SCALAR_SPECIALIZATION Scalar
#endif // !SCALAR_SPECIALIZATION

#ifndef MODE_SPECIALIZATION
    #define MODE_SPECIALIZATION Mode
#endif // !MODE_SPECIALIZATION

#ifndef MATRIX_TEMPLATE
    #define MATRIX_TEMPLATE                                                                                            \
        template <                                                                                                     \
            arithmetic_type SCALAR_SPECIALIZATION, size_type ROW_COUNT_SPECIALIZATION,                                 \
            size_type COL_COUNT_SPECIALIZATION, storage_mode MODE_SPECIALIZATION>
#endif // !MATRIX_TEMPLATE

namespace rsl::math
{
    MATRIX_TEMPLATE
    const matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, Mode>
        matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, Mode>::identity(
            SCALAR_SPECIALIZATION(1)
        );

    MATRIX_TEMPLATE
    const matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>
        matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::zero(
            SCALAR_SPECIALIZATION(0)
        );

    MATRIX_TEMPLATE
    constexpr matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::
        matrix() noexcept
        : columns{}
    {
        for (size_type i = 0; i < row_count; i++)
        {
            for (size_type j = 0; j < col_count; j++)
            {
                columns[j][i] = i == j ? static_cast<scalar>(1) : static_cast<scalar>(0);
            }
        }
    }

    MATRIX_TEMPLATE
    constexpr matrix<
        SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::matrix(scalar s
    ) noexcept
        : matrix(s, identity_matrix_signal{})
    {
    }

    MATRIX_TEMPLATE
    constexpr matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::
        matrix(scalar s, uniform_matrix_signal) noexcept
        : columns{}
    {
        for (size_type i = 0; i < row_count; i++)
        {
            for (size_type j = 0; j < col_count; j++)
            {
                columns[j][i] = s;
            }
        }
    }

    MATRIX_TEMPLATE
    constexpr matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::
        matrix(scalar s, identity_matrix_signal) noexcept
        : columns{}
    {
        for (size_type i = 0; i < row_count; i++)
        {
            for (size_type j = 0; j < col_count; j++)
            {
                columns[j][i] = i == j ? s : static_cast<scalar>(0);
            }
        }
    }

    MATRIX_TEMPLATE
    template <typename MatType>
        requires not_same_as<SCALAR_SPECIALIZATION, typename MatType::scalar> ||
                 (MatType::row_count != ROW_COUNT_SPECIALIZATION) || (MatType::col_count != COL_COUNT_SPECIALIZATION)
    constexpr matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::
        matrix(const MatType& other) noexcept
    {
        if constexpr (row_count == MatType::row_count && col_count == MatType::col_count)
        {
            for (size_type i = 0; i < size; i++)
            {
                data[i] = static_cast<scalar>(other.data[i]);
            }
        }
        else
        {
            constexpr size_type min_row = row_count < MatType::row_count ? row_count : MatType::row_count;
            constexpr size_type min_col = col_count < MatType::col_count ? col_count : MatType::col_count;

            for (size_type i = 0; i < min_row; i++)
            {
                for (size_type j = 0; j < min_col; j++)
                {
                    columns[j][i] = static_cast<scalar>(other[j][i]);
                }
            }
        }
    }

    MATRIX_TEMPLATE
    constexpr typename matrix<
        SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::col_type&
    matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::operator[](
        size_type i
    ) noexcept
    {
        rsl_assert_out_of_range_msg(i < col_count, "matrix subscript out of range");
        return columns[i];
    }

    MATRIX_TEMPLATE
    constexpr const typename matrix<
        SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::col_type&
    matrix<SCALAR_SPECIALIZATION, ROW_COUNT_SPECIALIZATION, COL_COUNT_SPECIALIZATION, MODE_SPECIALIZATION>::operator[](
        size_type i
    ) const noexcept
    {
        rsl_assert_out_of_range_msg(i < col_count, "matrix subscript out of range");
        return columns[i];
    }
} // namespace rsl::math

#undef ROW_COUNT_SPECIALIZATION
#undef COL_COUNT_SPECIALIZATION
#undef SCALAR_SPECIALIZATION
#undef MODE_SPECIALIZATION
#undef MATRIX_TEMPLATE
