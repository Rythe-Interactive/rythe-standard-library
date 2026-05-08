#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_length;

    template <typename Scalar, size_type Size, storage_mode Mode>
    struct compute_length<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(const vec_type& v) noexcept
        {
            return rsl::math::sqrt(dot(v, v));
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_rcp(const vec_type& v) noexcept
        {
            return rcp_sqrt(dot(v, v));
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute2(const vec_type& v) noexcept
        {
            return dot(v, v);
        }
    };

    template <typename Scalar, storage_mode Mode>
    struct compute_length<vector<Scalar, 1u, Mode>>
    {
        using vec_type = vector<Scalar, 1u, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar v) noexcept { return v; }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_rcp(Scalar v) noexcept { return rcp(v); }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute2(Scalar v) noexcept { return v * v; }
    };

    template <typename Scalar, size_type Size, storage_mode Mode, size_type... Args>
    struct compute_length<swizzle<Scalar, Size, Mode, Args...>>
    {
        using swizzle_type = swizzle<Scalar, Size, Mode, Args...>;
        using vec_type = typename swizzle_type::conv_type;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(const vec_type& v) noexcept
        {
            return compute_length<vec_type>::compute(v);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_rcp(const vec_type& v) noexcept
        {
            return compute_length<vec_type>::compute_rcp(v);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute2(const vec_type& v) noexcept
        {
            return compute_length<vec_type>::compute2(v);
        }
    };
} // namespace rsl::math::internal
