#pragma once
#include "../../../defines.hpp"
#include "../../../util/assert.hpp"
#include "../predefined.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type... Args>
    struct swizzle;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0>
    struct swizzle<Scalar, Size, Mode, S0>
    {
        using scalar = Scalar;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 1, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x) noexcept;

        [[rythe_always_inline]] constexpr operator scalar() const noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(scalar value) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[S0];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[S0];
        }
    };

    template <size_type Size, storage_mode Mode, size_type S0>
    struct swizzle<bool, Size, Mode, S0>
    {
        using scalar = bool;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 1, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[rythe_always_inline]] constexpr operator scalar() const noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(scalar value) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[S0];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[S0];
        }
    };

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1>
    struct swizzle<Scalar, Size, Mode, S0, S1>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1};

    public:
        using scalar = Scalar;
        static constexpr size_type size = 2;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 2, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x, scalar y) noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1>
    struct swizzle<bool, Size, Mode, S0, S1>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1};

    public:
        using scalar = bool;
        static constexpr size_type size = 2;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 2, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x, scalar y) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    struct swizzle<Scalar, Size, Mode, S0, S1, S2>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1, S2};

    public:
        using scalar = Scalar;
        static constexpr size_type size = 3;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 3, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z) noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2>
    struct swizzle<bool, Size, Mode, S0, S1, S2>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1, S2};

    public:
        using scalar = bool;
        static constexpr size_type size = 3;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 3, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(bool x, bool y, bool z) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };

    template <
        arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2,
        size_type S3>
    struct swizzle<Scalar, Size, Mode, S0, S1, S2, S3>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1, S2, S3};

    public:
        using scalar = Scalar;
        static constexpr size_type size = 4;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 4, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z, scalar w) noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };

    template <size_type Size, storage_mode Mode, size_type S0, size_type S1, size_type S2, size_type S3>
    struct swizzle<bool, Size, Mode, S0, S1, S2, S3>
    {
    private:
        constexpr static size_type lookup[] = {S0, S1, S2, S3};

    public:
        using scalar = bool;
        static constexpr size_type size = 4;
        static constexpr storage_mode mode = Mode;
        using conv_type = vector<scalar, 4, Mode>;
        using storage_type = storage_t<scalar, Size, Mode>;

        storage_type data;

        [[rythe_always_inline]] constexpr swizzle() noexcept = default;
        COPY_FUNCS_DECO([[rythe_always_inline]] constexpr, swizzle, noexcept);

        [[rythe_always_inline]] constexpr swizzle(scalar x, scalar y, scalar z, scalar w) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept;

        [[rythe_always_inline]] constexpr operator conv_type() const noexcept;

        [[rythe_always_inline]] constexpr swizzle& operator=(const conv_type& other) noexcept;

        [[rythe_always_inline]] constexpr scalar& operator[](const size_type i) noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }

        [[rythe_always_inline]] constexpr const scalar& operator[](const size_type i) const noexcept
        {
            rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
            return data[lookup[i]];
        }
    };
} // namespace rsl::math
