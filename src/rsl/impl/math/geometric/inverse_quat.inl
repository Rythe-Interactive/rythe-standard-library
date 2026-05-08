#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_inverse;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct compute_inverse<quaternion<Scalar, Mode>>
    {
        using quat_type = quaternion<Scalar, Mode>;

        [[nodiscard]] [[rythe_always_inline]] static quat_type compute(const quat_type& q) noexcept
        {
            // Assuming we never work with non-normalized quaternions.
            return quat_type(q.w, -q.vec);
        }
    };
} // namespace rsl::math::internal
