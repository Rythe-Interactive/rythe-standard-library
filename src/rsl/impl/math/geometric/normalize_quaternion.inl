#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_normalize;

    template <typename Scalar, storage_mode Mode>
    struct compute_normalize<quaternion<Scalar, Mode>>
    {
        using quat_type = quaternion<Scalar, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static quat_type compute(const quat_type& q) noexcept
        {
            return q * rcp_length(q);
        }
    };
} // namespace rsl::math::internal
