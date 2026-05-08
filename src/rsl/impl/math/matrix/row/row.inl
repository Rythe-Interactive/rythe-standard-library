#pragma once
#include "row.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr row<Scalar, RowCount, ColCount, RowIdx, Mode>::operator conv_type() const noexcept
    {
        conv_type result;

        for (size_type i = 0; i < size; i++)
        {
            result[i] = mx[i][RowIdx];
        }

        return result;
    }

    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr row<Scalar, RowCount, ColCount, RowIdx, Mode>&
    row<Scalar, RowCount, ColCount, RowIdx, Mode>::operator=(const conv_type& other) noexcept
    {
        for (size_type i = 0; i < size; i++)
        {
            mx[i][RowIdx] = other[i];
        }

        return *this;
    }

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr void row<bool, RowCount, ColCount, RowIdx, Mode>::set_mask(const bitfield8 mask) noexcept
    {
        for (size_type i = 0; i < size; i++)
        {
            mx[i][RowIdx] = (mask & (1 << i)) != 0;
        }
    }

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr bitfield8 row<bool, RowCount, ColCount, RowIdx, Mode>::mask() const noexcept
    {
        bitfield8 result = 0;

        for (size_type i = 0; i < size; i++)
        {
            result |= static_cast<bitfield8>(static_cast<uint32>(mx[i][RowIdx]) << i);
        }

        return result;
    }

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr row<bool, RowCount, ColCount, RowIdx, Mode>::operator conv_type() const noexcept
    {
        conv_type result;

        for (size_type i = 0; i < size; i++)
        {
            result[i] = mx[i][RowIdx];
        }

        return result;
    }

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    constexpr row<bool, RowCount, ColCount, RowIdx, Mode>&
    row<bool, RowCount, ColCount, RowIdx, Mode>::operator=(const conv_type& other) noexcept
    {
        for (size_type i = 0; i < size; i++)
        {
            mx[i][RowIdx] = other[i];
        }

        return *this;
    }

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    inline constexpr row<Scalar, RowCount, 1, RowIdx, Mode>::operator scalar() const noexcept
    {
        return mx[0][RowIdx];
    }

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<Scalar, RowCount, 1, RowIdx, Mode>::operator conv_type() const noexcept
    {
        return conv_type(mx[0][RowIdx]);
    }

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<Scalar, RowCount, 1, RowIdx, Mode>&
    row<Scalar, RowCount, 1, RowIdx, Mode>::operator=(const conv_type& other) noexcept
    {
        mx[0][RowIdx] = other[0];
        return *this;
    }

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<Scalar, RowCount, 1, RowIdx, Mode>& row<Scalar, RowCount, 1, RowIdx, Mode>::operator=(scalar value
    ) noexcept
    {
        mx[0][RowIdx] = value;
        return *this;
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr void row<bool, RowCount, 1, RowIdx, Mode>::set_mask(const bitfield8 mask) noexcept
    {
        mx[0][RowIdx] = (mask & 1) != 0;
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr bitfield8 row<bool, RowCount, 1, RowIdx, Mode>::mask() const noexcept
    {
        return static_cast<bitfield8>(mx[0][RowIdx]);
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    inline constexpr row<bool, RowCount, 1, RowIdx, Mode>::operator scalar() const noexcept
    {
        return mx[0][RowIdx];
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<bool, RowCount, 1, RowIdx, Mode>::operator conv_type() const noexcept
    {
        return conv_type(mx[0][RowIdx]);
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<bool, RowCount, 1, RowIdx, Mode>&
    row<bool, RowCount, 1, RowIdx, Mode>::operator=(const conv_type& other) noexcept
    {
        mx[0][RowIdx] = other[0];
        return *this;
    }

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    constexpr row<bool, RowCount, 1, RowIdx, Mode>& row<bool, RowCount, 1, RowIdx, Mode>::operator=(scalar value
    ) noexcept
    {
        mx[0][RowIdx] = value;
        return *this;
    }
} // namespace rsl::math
