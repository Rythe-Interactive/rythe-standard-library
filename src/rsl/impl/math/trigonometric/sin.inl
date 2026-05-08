#pragma once
#include "../basic/is_nan.hpp"
#include "../constants.hpp"
#include "sin.hpp"
#include "tan.hpp"

#include "sin_vector.inl"

namespace rsl::math
{
    namespace internal
    {
        template <floating_point_type Scalar>
        [[nodiscard]] [[rythe_always_inline]] constexpr Scalar constexpr_sin_impl(const Scalar value) noexcept
        {
            return (static_cast<Scalar>(2) * value) / (static_cast<Scalar>(1) + value * value);
        }

        template <floating_point_type Scalar>
        [[nodiscard]] [[rythe_always_inline]] constexpr Scalar constexpr_sin(const Scalar value) noexcept
        {
            if (is_nan(value))
            {
                return limits<Scalar>::quiet_nan;
            }

            if (limits<Scalar>::min > abs(value))
            {
                return static_cast<Scalar>(0);
            }

            if (limits<Scalar>::min > abs(value - half_pi<Scalar>))
            {
                return static_cast<Scalar>(1);
            }

            if (limits<Scalar>::min > abs(value + half_pi<Scalar>))
            {
                return -static_cast<Scalar>(1);
            }

            if (limits<Scalar>::min > abs(value - pi<Scalar>))
            {
                return static_cast<Scalar>(0);
            }

            if (limits<Scalar>::min > abs(value + pi<Scalar>))
            {
                return -static_cast<Scalar>(0);
            }

            return constexpr_sin_impl(tan(value / static_cast<Scalar>(2)));
        }

        template <floating_point_type Scalar>
        [[nodiscard]] [[rythe_always_inline]] constexpr Scalar constexpr_asin_impl(const Scalar value) noexcept
        {
            if (value > static_cast<Scalar>(1))
            {
                return limits<Scalar>::quiet_nan;
            }

            if (limits<Scalar>::min > abs(value - static_cast<Scalar>(1)))
            {
                return half_pi<Scalar>;
            }

            if (limits<Scalar>::min > value)
            {
                return static_cast<Scalar>(0);
            }

            return atan(value / sqrt(static_cast<Scalar>(1) - value * value));
        }

        template <arithmetic_type Scalar>
        [[nodiscard]] [[rythe_always_inline]] constexpr Scalar constexpr_asin(const Scalar value) noexcept
        {
            if (is_nan(value))
            {
                return limits<Scalar>::quiet_nan;
            }

            if (value < static_cast<Scalar>(0))
            {
                return -constexpr_asin_impl(-value);
            }

            return constexpr_asin_impl(value);
        }
    } // namespace internal

    // sin
    template <typename Scalar>
        requires floating_point_type<Scalar> || linear_algebraic_construct<Scalar>
    constexpr auto sin(Scalar&& value) noexcept
    {
        using type = remove_cvr_t<Scalar>;

        if constexpr (is_linear_algebraic_construct_v<type>)
        {
            return internal::compute_sin<type>::compute(forward<Scalar>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return internal::constexpr_sin<type>(value);
            }
            else
            {
                return ::std::sin(forward<Scalar>(value));
            }
        }
    }

    // asin
    template <typename Scalar>
        requires floating_point_type<Scalar> || linear_algebraic_construct<Scalar>
    constexpr auto asin(Scalar&& value) noexcept
    {
        using type = remove_cvr_t<Scalar>;

        if constexpr (is_linear_algebraic_construct_v<type>)
        {
            return internal::compute_sin<type>::compute_inverse(forward<Scalar>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return internal::constexpr_asin<type>(value);
            }
            else
            {
                return ::std::asin(forward<Scalar>(value));
            }
        }
    }
} // namespace rsl::math
