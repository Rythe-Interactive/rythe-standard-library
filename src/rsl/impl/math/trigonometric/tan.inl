#pragma once
#include "../basic/abs.hpp"
#include "../basic/is_nan.hpp"
#include "../basic/round.hpp"
#include "../../rsl_core.hpp"
#include "tan.hpp"

#include "tan_vector.inl"
// #include "tan_matrix.inl"
// #include "tan_quaternion.inl"

namespace rsl::math
{
    namespace internal
    {
        // https://proofwiki.org/wiki/Power_Series_Expansion_for_Tangent_Function
        template <floating_point_type Scalar>
        constexpr Scalar constexpr_tan_series_expansion(Scalar value) noexcept
        {
            value -= half_pi<Scalar>;

            if (limits<Scalar>::epsilon > abs(value))
            {
                return static_cast<Scalar>(1.633124e+16);
            }

            Scalar value2 = value * value;
            Scalar value3 = value2 * value;
            Scalar value5 = value2 * value3;
            Scalar value7 = value2 * value5;

            return static_cast<Scalar>(-1.0) / value + (
                       value / static_cast<Scalar>(3.0) + (
                           value3 / static_cast<Scalar>(45.0) + (
                               (static_cast<Scalar>(2.0) * value5) / static_cast<Scalar>(945.0) +
                               value7 / static_cast<Scalar>(4725.0)
                           )
                       )
                   );
        }

        // https://proofwiki.org/wiki/Continued_Fraction_for_Tangent_Function
        template <floating_point_type Scalar>
        constexpr Scalar constexpr_tan_continued_fraction(const Scalar value) noexcept
        {
            if (value > static_cast<Scalar>(1.55) && value < static_cast<Scalar>(1.60))
            {
                return constexpr_tan_series_expansion(value);
            }

            size_type steps;
            if (value > static_cast<Scalar>(1.4))
            {
                steps = 45ull;
            }
            else if (value > static_cast<Scalar>(1.0))
            {
                steps = 35ull;
            }
            else
            {
                steps = 25ull;
            }

            const Scalar value2 = value * value;

            size_type step = steps - 1;
            Scalar denominator = static_cast<Scalar>(steps * 2.0 - 1.0);

            while (step != 0ull)
            {
                denominator = static_cast<Scalar>(step * 2.0 - 1.0) - value2 / denominator;
                --step;
            }

            return value / denominator;
        }

        template <floating_point_type Scalar>
        constexpr Scalar constexpr_tan_impl(const Scalar value, const size_type count = 0) noexcept
        {
            if (value > pi<Scalar>)
            {
                if (count > 1ull)
                {
                    return limits<Scalar>::quiet_nan;
                }

                return constexpr_tan_impl(value - pi<Scalar> * floor(value / pi<Scalar>), count + 1ull);
            }

            return constexpr_tan_continued_fraction(value);
        }

        template <floating_point_type Scalar>
        constexpr Scalar constexpr_tan(const Scalar value) noexcept
        {
            if (is_nan(value))
            {
                return limits<Scalar>::quiet_nan;
            }

            if (limits<Scalar>::epsilon > abs(value))
            {
                return static_cast<Scalar>(0.0);
            }

            if (value < static_cast<Scalar>(0.0))
            {
                return -constexpr_tan_impl(-value);
            }

            return constexpr_tan_impl(value);
        }

        // https://en.wikipedia.org/wiki/Arctangent_series
        template <floating_point_type Scalar>
        constexpr Scalar constexpr_atan_series_expansion(const Scalar value) noexcept
        {
            Scalar checkTable[] = {
                static_cast<Scalar>(3.0),
                static_cast<Scalar>(4.0),
                static_cast<Scalar>(5.0),
                static_cast<Scalar>(7.0),
                static_cast<Scalar>(11.0),
                static_cast<Scalar>(25.0),
                static_cast<Scalar>(100.0),
                static_cast<Scalar>(1000.0)
            };

            size_type steps = 2;
            for (size_type i = 0; i < 8; ++i)
            {
                if (value < checkTable[i])
                {
                    steps = 10ull - i;
                    break;
                }
            }

            Scalar result = value;
            Scalar exponentiation = value;
            Scalar value2 = value * value;
            for (size_type step = 1; step != steps; ++step)
            {
                exponentiation *= value2;
                result += (exponentiation / static_cast<Scalar>(step * 2.0 + 1.0)) * static_cast<Scalar>(step % 2ull == 0ull? -1.0 : 1.0);
            }

            return result;
        }

        template <floating_point_type Scalar>
        constexpr Scalar constexpr_atan_continued_fraction(const Scalar value) noexcept
        {
            const size_type steps = 5ull + 10ull * (1ull + static_cast<size_type>(value + static_cast<Scalar>(0.5)));

            const Scalar value2 = value * value;

            size_type step = steps - 1;
            Scalar denominator = static_cast<Scalar>(2.0 * steps - 1.0);

            while (step != 0ull)
            {
                denominator = static_cast<Scalar>(2.0 * step - 1.0) + static_cast<Scalar>(step * step) * value2 /
                              denominator;
                --step;
            }

            return value / denominator;
        }

        template <floating_point_type Scalar>
        constexpr Scalar constexpr_atan_impl(const Scalar value) noexcept
        {
            if (value > static_cast<Scalar>(2.5))
            {
                return constexpr_atan_series_expansion(value);
            }
            return constexpr_atan_continued_fraction(value);
        }

        template <floating_point_type Scalar>
        constexpr Scalar constexpr_atan(const Scalar value) noexcept
        {
            if (is_nan(value))
            {
                return limits<Scalar>::quiet_nan;
            }

            if (limits<Scalar>::epsilon > abs(value))
            {
                return static_cast<Scalar>(0.0);
            }

            if (value < static_cast<Scalar>(0.0))
            {
                return -constexpr_atan_impl(-value);
            }

            return constexpr_atan_impl(value);
        }
    } // namespace internal

    // tan
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    constexpr auto tan(T&& value) noexcept
    {
        if constexpr (is_linear_algebraic_construct_v<T>)
        {
            return internal::compute_tan<remove_cvr_t<T>>::compute(forward<T>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return internal::constexpr_tan<remove_cvr_t<T>>(value);
            }
            else
            {
                return ::std::tan(forward<T>(value));
            }
        }
    }

    // atan
    template <typename T>
        requires floating_point_type<T> || linear_algebraic_construct<T>
    constexpr auto atan(T&& value) noexcept
    {
        if constexpr (is_linear_algebraic_construct_v<T>)
        {
            return internal::compute_tan<remove_cvr_t<T>>::compute_inverse(forward<T>(value));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return internal::constexpr_atan<remove_cvr_t<T>>(value);
            }
            else
            {
                return ::std::atan(forward<T>(value));
            }
        }
    }

    template <typename X, typename Y>
        requires same_as<remove_cvr_t<X>, remove_cvr_t<Y>> && (floating_point_type<X> || linear_algebraic_construct<X>)
    constexpr auto atan2(X&& value, Y&& y) noexcept
    {
        if constexpr (is_linear_algebraic_construct_v<X>)
        {
            return internal::compute_tan<elevated_t<X, Y>>::compute_inverse2(forward<X>(value), forward<Y>(y));
        }
        else
        {
            if (is_constant_evaluated())
            {
                return internal::constexpr_atan<remove_cvr_t<X>>(y / value);
            }
            else
            {
                return ::std::atan2(forward<X>(value), forward<Y>(y));
            }
        }
    }
} // namespace rsl::math
