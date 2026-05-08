#pragma once
#include "equality.hpp"

#include "equality_matrix.inl"
#include "equality_quaternion.inl"
#include "equality_vector.inl"

namespace rsl::math
{
    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    constexpr auto equal(A&& a, B&& b) noexcept
    {
        if constexpr ((is_vector_v<A> && is_vector_v<B>) || (is_matrix_v<A> && is_matrix_v<B>) ||
                      (is_quat_v<A> && is_quat_v<B>))
        {
            return internal::compute_equality<elevated_t<A, B>>::compute(forward<A>(a), forward<B>(b));
        }
        else if constexpr (is_vector_v<A> || is_vector_v<B>)
        {
            return internal::compute_equality<select_vector_type_t<A, B>>::compute(forward<A>(a), forward<B>(b));
        }
        else if constexpr ((is_matrix_v<A> || is_matrix_v<B>) &&
                           (is_vector_v<A> || is_vector_v<B> || is_arithmetic_v<A> || is_arithmetic_v<B>))
        {
            return internal::compute_equality<select_matrix_type_t<A, B>>::compute(forward<A>(a), forward<B>(b));
        }
        else
        {
            rsl_assert_unreachable();
        }
    }

    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    constexpr auto operator==(A&& a, B&& b) noexcept
    {
        return equal(a, b);
    }

    template <typename A, typename B>
        requires internal::equal_comparable<A, B>
    constexpr auto operator!=(A&& a, B&& b) noexcept
    {
        return !(a == b);
    }
} // namespace rsl::math
