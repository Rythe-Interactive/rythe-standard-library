#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_length;

    template <typename Scalar, storage_mode Mode>
    struct compute_length<quaternion<Scalar, Mode>>
    {
        using quat_type = quaternion<Scalar, Mode>;
        using vec_type = vector<Scalar, 4, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(const quat_type& q) noexcept
        {
            vec_type v(q.w, q.i, q.j, q.k);
            return compute_length<vec_type>::compute(v);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_rcp(const quat_type& q) noexcept
        {
            vec_type v(q.w, q.i, q.j, q.k);
            return compute_length<vec_type>::compute_rcp(v);
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute2(const quat_type& q) noexcept
        {
            vec_type v(q.w, q.i, q.j, q.k);
            return compute_length<vec_type>::compute2(v);
        }
    };
} // namespace rsl::math::internal
