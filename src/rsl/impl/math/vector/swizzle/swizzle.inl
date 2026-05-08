#pragma once
#include "swizzle_base.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<Scalar, Size, Mode, S0>::swizzle(Scalar x) noexcept
    {
        data[S0] = x;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<Scalar, Size, Mode, S0>::operator Scalar() const noexcept
    {
        return data[S0];
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<Scalar, Size, Mode, S0>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        return result;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<Scalar, Size, Mode, S0>& swizzle<Scalar, Size, Mode, S0>::operator=(const conv_type& other
    ) noexcept
    {
        data[S0] = other[0];
        return *this;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<Scalar, Size, Mode, S0>& swizzle<Scalar, Size, Mode, S0>::operator=(Scalar value) noexcept
    {
        data[S0] = value;
        return *this;
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<bool, Size, Mode, S0>::swizzle(bool x) noexcept
    {
        data[S0] = x;
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr void swizzle<bool, Size, Mode, S0>::set_mask(const bitfield8 mask) noexcept
    {
        data[S0] = mask & 1;
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr bitfield8 swizzle<bool, Size, Mode, S0>::mask() const noexcept
    {
        return static_cast<bitfield8>(data[S0]);
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<bool, Size, Mode, S0>::operator scalar() const noexcept
    {
        return data[S0];
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<bool, Size, Mode, S0>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        return result;
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<bool, Size, Mode, S0>& swizzle<bool, Size, Mode, S0>::operator=(const conv_type& other) noexcept
    {
        data[S0] = other[0];
        return *this;
    }

    template <size_type Size, storage_mode Mode, size_type S0>
    constexpr swizzle<bool, Size, Mode, S0>& swizzle<bool, Size, Mode, S0>::operator=(scalar value) noexcept
    {
        data[S0] = value;
        return *this;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<Scalar, Size, Mode, S0, S1>::swizzle(Scalar x, Scalar y) noexcept
    {
        data[S0] = x;
        data[S1] = y;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<Scalar, Size, Mode, S0, S1>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        return result;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<Scalar, Size, Mode, S0, S1>& swizzle<Scalar, Size, Mode, S0, S1>::operator=(const conv_type& other
    ) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        return *this;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<bool, Size, Mode, S0, S1>::swizzle(bool x, bool y) noexcept
    {
        data[S0] = x;
        data[S1] = y;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr void swizzle<bool, Size, Mode, S0, S1>::set_mask(const bitfield8 mask) noexcept
    {
        data[S0] = mask & 1;
        data[S1] = mask & 2;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr bitfield8 swizzle<bool, Size, Mode, S0, S1>::mask() const noexcept
    {
        return static_cast<bitfield8>(data[S0] | data[S0] << 1u);
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<bool, Size, Mode, S0, S1>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        return result;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    constexpr swizzle<bool, Size, Mode, S0, S1>& swizzle<bool, Size, Mode, S0, S1>::operator=(const conv_type& other
    ) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        return *this;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2>::swizzle(Scalar x, Scalar y, Scalar z) noexcept
    {
        data[S0] = x;
        data[S1] = y;
        data[S2] = z;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        result.data[2] = data[S2];
        return result;
    }

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2>&
    swizzle<Scalar, Size, Mode, S0, S1, S2>::operator=(const conv_type& other) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        data[S2] = other[2];
        return *this;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2>::swizzle(bool x, bool y, bool z) noexcept
    {
        data[S0] = x;
        data[S1] = y;
        data[S2] = z;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr void swizzle<bool, Size, Mode, S0, S1, S2>::set_mask(const bitfield8 mask) noexcept
    {
        data[S0] = mask & 1;
        data[S1] = mask & 2;
        data[S2] = mask & 4;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr bitfield8 swizzle<bool, Size, Mode, S0, S1, S2>::mask() const noexcept
    {
        return static_cast<bitfield8>(data[S0] | data[S1] << 1u | data[S2] << 2u);
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        result.data[2] = data[S2];
        return result;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2>&
    swizzle<bool, Size, Mode, S0, S1, S2>::operator=(const conv_type& other) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        data[S2] = other[2];
        return *this;
    }

    template <
        arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2,
        size_type S3>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2, S3>::swizzle(Scalar x, Scalar y, Scalar z, Scalar w) noexcept
    {
        data[S0] = x;
        data[S1] = y;
        data[S2] = z;
        data[S3] = w;
    }

    template <
        arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2,
        size_type S3>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2, S3>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        result.data[2] = data[S2];
        result.data[3] = data[S3];
        return result;
    }

    template <
        arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2,
        size_type S3>
    constexpr swizzle<Scalar, Size, Mode, S0, S1, S2, S3>&
    swizzle<Scalar, Size, Mode, S0, S1, S2, S3>::operator=(const conv_type& other) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        data[S2] = other[2];
        data[S3] = other[3];
        return *this;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2, S3>::swizzle(bool x, bool y, bool z, bool w) noexcept
    {
        data[S0] = x;
        data[S1] = y;
        data[S2] = z;
        data[S3] = w;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    constexpr void swizzle<bool, Size, Mode, S0, S1, S2, S3>::set_mask(const bitfield8 mask) noexcept
    {
        data[S0] = mask & 1;
        data[S1] = mask & 2;
        data[S2] = mask & 4;
        data[S3] = mask & 8;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    constexpr bitfield8 swizzle<bool, Size, Mode, S0, S1, S2, S3>::mask() const noexcept
    {
        return static_cast<bitfield8>(data[S0] | data[S1] << 1u | data[S2] << 2u | data[S3] << 3u);
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2, S3>::operator conv_type() const noexcept
    {
        conv_type result;
        result.data[0] = data[S0];
        result.data[1] = data[S1];
        result.data[2] = data[S2];
        result.data[3] = data[S3];
        return result;
    }

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    constexpr swizzle<bool, Size, Mode, S0, S1, S2, S3>&
    swizzle<bool, Size, Mode, S0, S1, S2, S3>::operator=(const conv_type& other) noexcept
    {
        data[S0] = other[0];
        data[S1] = other[1];
        data[S2] = other[2];
        data[S3] = other[3];
        return *this;
    }
} // namespace rsl::math
