#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_normalize;

    template <typename Scalar, size_type Size, storage_mode Mode>
    struct compute_normalize<vector<Scalar, Size, Mode>>
    {
        using vec_type = vector<Scalar, Size, Mode>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute(const vec_type& v) noexcept
        {
            return v * rcp_length(v);
        }
    };

    template <typename Scalar>
    struct compute_normalize<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute([[maybe_unused]] Scalar v) noexcept
        {
            return static_cast<Scalar>(1);
        }
    };
} // namespace rsl::math::internal
