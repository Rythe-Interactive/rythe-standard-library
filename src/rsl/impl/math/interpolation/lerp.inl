#include "lerp.hpp"
#pragma once

namespace rsl::math::internal
{
    template <typename V, typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto _lerp_impl_(V&& a, V&& b, T&& t) noexcept
    {
        return a + (b - a) * t;
    }
} // namespace rsl::math::internal

#include "lerp_vector.inl"
// #include "lerp_matrix.inl"
// #include "lerp_quaternion.inl"

namespace rsl::math
{
    // lerp
    template <typename TypeA, typename TypeB, typename InterpType>
    [[nodiscard]] constexpr auto lerp(TypeA&& a, TypeB&& b, InterpType&& t) noexcept
    {
        using A = ::std::remove_cvref_t<TypeA>;
        using B = ::std::remove_cvref_t<TypeB>;

        if constexpr (is_quat_v<A> && is_quat_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return internal::compute_lerp<quaternion<scalar>>::compute(
                forward<TypeA>(a), forward<TypeB>(b), forward<InterpType>(t)
            );
        }
        else if constexpr (is_matrix_v<A> && is_matrix_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return internal::compute_lerp<matrix<scalar, A::row_count, A::col_count>>::compute(
                forward<TypeA>(a), forward<TypeB>(b), forward<InterpType>(t)
            );
        }
        else if constexpr (is_vector_v<A> && is_vector_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            constexpr size_type size = min(A::size, B::size);
            return internal::compute_lerp<vector<scalar, size>>::compute(
                forward<TypeA>(a), forward<TypeB>(b), forward<InterpType>(t)
            );
        }
        else
        {
            return internal::_lerp_impl_(forward<TypeA>(a), forward<TypeB>(b), forward<InterpType>(t));
        }
    }
} // namespace rsl::math
