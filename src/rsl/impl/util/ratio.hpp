#pragma once
#include "../rsl_core.hpp"

namespace rsl
{
    namespace internal
    {
        [[nodiscard]] consteval int_max _ratio_abs(const int_max value) noexcept
        {
            return value < 0 ? -value : value;
        }

        [[nodiscard]] consteval int_max _ratio_sign_of(const int_max value) noexcept
        {
            return value < 0 ? -1 : 1;
        }

        [[nodiscard]] consteval int_max _ratio_greatest_common_divisor(int_max a, int_max b) noexcept
        {
            if (a == 0 && b == 0)
            {
                return 1; // contrary to mathematical convention; avoids division by 0 in ratio_less
            }

            a = _ratio_abs(a);
            b = _ratio_abs(b);

            while (b != 0)
            {
                const int_max a2 = a;
                a = b;
                b = a2 % b;
            }

            return a;
        }
    }

    template <int_max Numerator, int_max Denominator = 1>
    struct ratio
    {
        static_assert(Denominator != 0, "zero denominator");
        static_assert(-INTMAX_MAX <= Numerator, "numerator too negative");
        static_assert(-INTMAX_MAX <= Denominator, "denominator too negative");

        static constexpr intmax_t numerator = internal::_ratio_sign_of(Numerator) * internal::_ratio_sign_of(Denominator) *
                internal::_ratio_abs(Numerator) / internal::_ratio_greatest_common_divisor(Numerator, Denominator);
        static constexpr intmax_t denominator =
                internal::_ratio_abs(Denominator) / internal::_ratio_greatest_common_divisor(Numerator, Denominator);
    };

    template <typename T>
    constexpr bool is_ratio_v = false;

    template <int_max Numerator, int_max Denominator>
    constexpr bool is_ratio_v<ratio<Numerator, Denominator>> = true;

    template <typename T>
    struct is_ratio : bool_constant<is_ratio_v<T>>
    {};

    template<typename T>
    concept ratio_type = is_ratio_v<T>;

    using atto  = ratio<1, 1000000000000000000LL>;
    using femto = ratio<1, 1000000000000000LL>;
    using pico  = ratio<1, 1000000000000LL>;
    using nano  = ratio<1, 1000000000>;
    using micro = ratio<1, 1000000>;
    using milli = ratio<1, 1000>;
    using centi = ratio<1, 100>;
    using deci  = ratio<1, 10>;
    using deca  = ratio<10, 1>;
    using hecto = ratio<100, 1>;
    using kilo  = ratio<1000, 1>;
    using mega  = ratio<1000000, 1>;
    using giga  = ratio<1000000000, 1>;
    using tera  = ratio<1000000000000LL, 1>;
    using peta  = ratio<1000000000000000LL, 1>;
    using exa   = ratio<1000000000000000000LL, 1>;
}
