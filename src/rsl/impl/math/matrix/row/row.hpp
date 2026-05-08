#pragma once
#include "../predefined.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    struct row
    {
        using scalar = Scalar;
        static constexpr size_type size = ColCount;
        static constexpr storage_mode mode = Mode;
        static constexpr size_type row_count = RowCount;
        static constexpr size_type col_count = ColCount;
        static constexpr size_type row_idx = RowIdx;
        using conv_type = vector<scalar, size, Mode>;
        using storage_type = vector<scalar, row_count, Mode>;

        storage_type mx[col_count];

        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, row, noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr row& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }
    };

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    struct row<bool, RowCount, ColCount, RowIdx, Mode>
    {
        using scalar = bool;
        static constexpr size_type size = ColCount;
        static constexpr storage_mode mode = Mode;
        static constexpr size_type row_count = RowCount;
        static constexpr size_type col_count = ColCount;
        static constexpr size_type row_idx = RowIdx;
        using conv_type = vector<scalar, size, Mode>;
        using storage_type = vector<scalar, row_count, Mode>;

        storage_type mx[col_count];

        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, row, noexcept);

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr row& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }
    };

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    struct row<Scalar, RowCount, 1, RowIdx, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        static constexpr size_type row_count = RowCount;
        static constexpr size_type col_count = 1;
        static constexpr size_type row_idx = RowIdx;
        using conv_type = vector<scalar, size, Mode>;
        using storage_type = vector<scalar, row_count, Mode>;

        storage_type mx[col_count];

        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, row, noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr row& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr row& operator=(scalar value) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }
    };

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    struct row<bool, RowCount, 1, RowIdx, Mode>
    {
        using scalar = bool;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        static constexpr size_type row_count = RowCount;
        static constexpr size_type col_count = 1;
        static constexpr size_type row_idx = RowIdx;
        using conv_type = vector<scalar, size, Mode>;
        using storage_type = vector<scalar, row_count, Mode>;

        storage_type mx[col_count];

        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, row, noexcept);

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr row& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr row& operator=(scalar value) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return mx[i][RowIdx];
        }
    };
} // namespace rsl::math
