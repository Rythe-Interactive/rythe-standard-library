#pragma once
#include "rcp.hpp"
#include <emmintrin.h>
#include <xmmintrin.h>

namespace rsl::math
{
    namespace internal
    {
        [[nodiscard]] [[rythe_always_inline]] inline float32 sse_rcp(float32 value) noexcept
        {
            _mm_store_ss(&value, _mm_rcp_ss(_mm_set_ss(value)));
            return value;
        }

        [[nodiscard]] [[rythe_always_inline]] inline float64 sse_rcp(float64 value) noexcept
        {
            _mm_store_sd(&value, _mm_div_sd(_mm_set_sd(1.0), _mm_set_sd(value))); // avoid _mm_rcp14_pd
            return value;
        }
    } // namespace internal

    template <floating_point_type Scalar>
    constexpr Scalar rcp(Scalar value) noexcept
    {
        if (is_constant_evaluated())
        {
            return static_cast<Scalar>(1) / value;
        }
        else
        {
            if constexpr (sizeof(Scalar) > sizeof(float64))
            {
                return static_cast<Scalar>(1) / value;
            }
            else
            {
                return internal::sse_rcp(value);
            }
        }
    }
} // namespace rsl::math
