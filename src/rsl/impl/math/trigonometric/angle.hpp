#pragma once
#include "../../rsl_core.hpp"
#include "../constants.hpp"

namespace rsl::math
{
    template <arithmetic_type Precision = float32>
    struct degrees;

    template <arithmetic_type Precision = float32>
    struct angle
    {
        using scalar = Precision;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar degrees() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr scalar radians() const noexcept;

        [[rythe_always_inline]] constexpr operator scalar() const noexcept { return value; }

        constexpr angle() noexcept = default;
        constexpr angle(scalar v) noexcept
            : value(v)
        {
        }
        explicit constexpr angle(rsl::math::degrees<scalar> deg) noexcept;

        // the default is radians!
        scalar value;

        const static angle zero;
        const static angle deg45;
        const static angle deg60;
        const static angle deg90;
        const static angle deg120;
        const static angle deg180;
    };

    template <arithmetic_type Precision>
    angle(Precision p) -> angle<Precision>;

    template <arithmetic_type Precision>
    angle(degrees<Precision> d) -> angle<Precision>;

    template <arithmetic_type Precision = float32>
    using radians = angle<Precision>;

    template <arithmetic_type Precision>
    constexpr typename angle<Precision>::scalar angle<Precision>::degrees() const noexcept
    {
        return rad2deg<Precision> * value;
    }

    template <arithmetic_type Precision>
    constexpr typename angle<Precision>::scalar angle<Precision>::radians() const noexcept
    {
        return value;
    }

    template <arithmetic_type Precision>
    struct degrees
    {
        using scalar = Precision;

        constexpr degrees() noexcept = default;
        constexpr degrees(scalar v) noexcept
            : value(v)
        {
        }
        explicit constexpr degrees(angle<scalar> rad) noexcept;

        scalar value;

        constexpr math::radians<scalar> radians() const noexcept { return {deg2rad<scalar> * value}; }
    };

    template <arithmetic_type Precision>
    degrees(Precision p) -> degrees<Precision>;

    template <arithmetic_type Precision>
    degrees(angle<Precision> a) -> degrees<Precision>;

    template <arithmetic_type Precision>
    constexpr angle<Precision>::angle(rsl::math::degrees<scalar> deg) noexcept
        : value(deg2rad<scalar> * deg.value)
    {
    }

    template <arithmetic_type Precision>
    constexpr degrees<Precision>::degrees(angle<scalar> rad) noexcept
        : value(rad2deg<scalar> * rad.value)
    {
    }

    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::zero = angle(rsl::math::degrees<Precision>{0});
    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::deg45 = angle(rsl::math::degrees<Precision>{45});
    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::deg60 = angle(rsl::math::degrees<Precision>{60});
    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::deg90 = angle(rsl::math::degrees<Precision>{90});
    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::deg120 = angle(rsl::math::degrees<Precision>{120});
    template <arithmetic_type Precision>
    const angle<Precision> angle<Precision>::deg180 = angle(rsl::math::degrees<Precision>{180});

    using angle32 = angle<float32>;
    using angle64 = angle<float64>;

    using radians32 = radians<float32>;
    using radians64 = radians<float64>;

    using degrees32 = degrees<float32>;
    using degrees64 = degrees<float64>;

#ifdef RYTHE_PCH
    template struct angle<float32>;
    template struct angle<float64>;

    template struct degrees<float32>;
    template struct degrees<float64>;
#endif
} // namespace rsl::math
