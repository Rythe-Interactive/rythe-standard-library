#pragma once

namespace rsl::math::internal
{
    template <typename T>
    struct compute_equality;

    template <quat_type QuatType>
    struct compute_equality<QuatType>
    {
        using result_type = bool;

        [[nodiscard]] [[rythe_always_inline]] constexpr static result_type
        compute(const QuatType& a, const QuatType& b) noexcept
        {
            return close_enough(a.i, b.i) && close_enough(a.j, b.j) && close_enough(a.k, b.k) && close_enough(a.w, b.w);
        }
    };

} // namespace rsl::math::internal
