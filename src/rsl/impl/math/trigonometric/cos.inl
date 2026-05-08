#pragma once
#include "cos.hpp"
#include "sin.hpp"

#include "cos_vector.inl"
// #include "cos_matrix.inl"
// #include "cos_quaternion.inl"

namespace rsl::math
{
    // cos
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    constexpr auto cos(T&& value) noexcept
    {
        using type = remove_cvr_t<T>;

        if constexpr (is_linear_algebraic_construct_v<type>)
        {
            return internal::compute_cos<type>::compute(forward<T>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return sin(value + half_pi<type>);
            }
            else
            {
                return ::std::cos(forward<T>(value));
            }
        }
    }

    // acos
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    constexpr auto acos(T&& value) noexcept
    {
        using type = remove_cvr_t<T>;

        if constexpr (is_linear_algebraic_construct_v<type>)
        {
            return internal::compute_cos<type>::compute_inverse(forward<T>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return -asin(value) + half_pi<type>;
            }
            else
            {
                return ::std::acos(forward<T>(value));
            }
        }
    }
} // namespace rsl::math
