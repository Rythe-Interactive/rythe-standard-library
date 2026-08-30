#pragma once
#include "../../rsl_core.hpp"

#include "sqrt.hpp"
#include <emmintrin.h>
#include <xmmintrin.h>

namespace rsl::math
{
    namespace internal
    {
        [[nodiscard]] [[rythe_always_inline]] inline float32 sse_sqrtf(float32 value) noexcept
        {
            _mm_store_ss(&value, _mm_sqrt_ss(_mm_set_ss(value)));
            return value;
        }

        [[nodiscard]] [[rythe_always_inline]] inline float64 sse_sqrtf(float64 value) noexcept
        {
            _mm_store_sd(&value, _mm_sqrt_pd(_mm_set_sd(value)));
            return value;
        }

#ifdef RYTHE_FMA_ENABLED
        [[nodiscard]] [[rythe_always_inline]] inline float32 sse_avx_rsqrtf(float32 value) noexcept
        {
            const __m128 x = _mm_set_ss(value);

            const __m128 nr = _mm_rsqrt_ss(x);

            const __m128 xnr = _mm_mul_ss(x, nr);                                  // dep on nr
            const __m128 halfNr = _mm_mul_ss(_mm_set_ss(0.5f), nr);                // dep on nr

            const __m128 threeMinusMuls = _mm_fnmadd_ss(xnr, nr, _mm_set_ss(3.f)); // -(xnr*nr) + 3

            const __m128 result = _mm_mul_ss(halfNr, threeMinusMuls);              // dep on threeMinusMuls
            _mm_store_ss(&value, result);
            return value;
        }

        [[nodiscard]] [[rythe_always_inline]] inline float64 sse_avx_rsqrtf(float64 value) noexcept
        {
            const __m128d x = _mm_set_sd(value);

            const __m128d nr = _mm_castps_pd(_mm_rsqrt_ss(_mm_castpd_ps(x)));       // avoid _mm_rsqrt14_pd

            const __m128d xnr = _mm_mul_sd(x, nr);                                  // dep on nr
            const __m128d halfNr = _mm_mul_sd(_mm_set_sd(0.5f), nr);                // dep on nr

            const __m128d threeMinusMuls = _mm_fnmadd_sd(xnr, nr, _mm_set_sd(3.f)); // -(xnr*nr) + 3

            const __m128d result = _mm_mul_sd(halfNr, threeMinusMuls);              // dep on threeMinusMuls
            _mm_store_sd(&value, result);
            return value;
        }
#else
        [[nodiscard]] [[rythe_always_inline]] inline float32 sse_rsqrtf(float32 value) noexcept
        {
            const __m128 x = _mm_set_ss(value);
            const __m128 nr = _mm_rsqrt_ss(x);
            const __m128 xnr = _mm_mul_ss(x, nr);                            // dep on nr
            const __m128 halfNr = _mm_mul_ss(_mm_set_ss(0.5f), nr);          // dep on nr
            const __m128 muls = _mm_mul_ss(xnr, nr);                         // dep on xnr
            const __m128 threeMinusMuls = _mm_sub_ss(_mm_set_ss(3.f), muls); // dep on muls
            const __m128 result = _mm_mul_ss(halfNr, threeMinusMuls);        // dep on threeMinusMuls
            _mm_store_ss(&value, result);

            return value;
        }

        [[nodiscard]] [[rythe_always_inline]] inline float64 sse_rsqrtf(float64 value) noexcept
        {
            const __m128d x = _mm_set_sd(value);
            const __m128d nr = _mm_castps_pd(_mm_rsqrt_ss(_mm_castpd_ps(x))); // avoid _mm_rsqrt14_pd
            const __m128d xnr = _mm_mul_sd(x, nr);                            // dep on nr
            const __m128d halfNr = _mm_mul_sd(_mm_set_sd(0.5), nr);           // dep on nr
            const __m128d muls = _mm_mul_sd(xnr, nr);                         // dep on xnr
            const __m128d threeMinusMuls = _mm_sub_sd(_mm_set_sd(3.0), muls); // dep on muls
            const __m128d result = _mm_mul_sd(halfNr, threeMinusMuls);        // dep on threeMinusMuls
            _mm_store_sd(&value, result);

            return value;
        }
#endif

        template <integer_type Integer>
            requires unsigned_type<Integer>
        [[nodiscard]] constexpr Integer constexpr_sqrt_impl(Integer value, Integer lo, Integer hi)
        {
            if (lo == hi)
            {
                return lo;
            }

            const Integer mid = (lo + hi + 1) / 2;

            if (value / mid < mid)
            {
                return constexpr_sqrt_impl(value, lo, mid - static_cast<Integer>(1));
            }
            else
            {
                return constexpr_sqrt_impl(value, mid, hi);
            }
        }

        template <integer_type Integer>
        [[nodiscard]] constexpr Integer constexpr_sqrt(const Integer value)
        {
            using unsigned_integer = make_unsigned_t<Integer>;
            return static_cast<Integer>(constexpr_sqrt_impl<unsigned_integer>(value, 0, value / 2 + 1));
        }

        template <floating_point_type Scalar>
        [[nodiscard]] constexpr Scalar sqrt_newton_raphson(const Scalar value, const Scalar curr, const Scalar prev)
        {
            return curr == prev ? curr
                                : sqrt_newton_raphson(value, static_cast<Scalar>(0.5) * (curr + value / curr), curr);
        }

        template <floating_point_type Scalar>
        [[nodiscard]] constexpr Scalar constexpr_sqrtf(const Scalar value)
        {
            return value >= 0 && value < limits<Scalar>::infinity ? sqrt_newton_raphson(value, value, Scalar(0))
                                                                  : limits<Scalar>::quiet_nan;
        }

        template <floating_point_type Scalar>
        [[nodiscard]] constexpr Scalar constexpr_rsqrtf(const Scalar value)
        {
            return static_cast<Scalar>(1) / constexpr_sqrtf(value);
        }
    } // namespace internal

    template <arithmetic_type Scalar>
    constexpr Scalar sqrt(const Scalar value) noexcept
    {
        if (is_constant_evaluated())
        {
            if constexpr (is_floating_point_v<Scalar>)
            {
                return internal::constexpr_sqrtf<remove_cvr_t<Scalar>>(value);
            }
            else
            {
                return internal::constexpr_sqrt<remove_cvr_t<Scalar>>(value);
            }
        }
        else
        {
            if constexpr (sizeof(Scalar) > sizeof(float32))
            {
                return static_cast<Scalar>(internal::sse_sqrtf(static_cast<float64>(value)));
            }
            else
            {
                return static_cast<Scalar>(internal::sse_sqrtf(static_cast<float32>(value)));
            }
        }
    }

    template <floating_point_type Scalar>
    constexpr Scalar rcp_sqrt(const Scalar value) noexcept
    {
        if (is_constant_evaluated())
        {
            return internal::constexpr_rsqrtf(value);
        }
        else
        {
            if constexpr (sizeof(Scalar) > sizeof(float32))
            {
#ifdef RYTHE_FMA_ENABLED
                return static_cast<Scalar>(internal::sse_avx_rsqrtf(static_cast<float64>(value)));
#else
                return static_cast<Scalar>(internal::sse_rsqrtf(static_cast<float64>(value)));
#endif
            }
            else
            {
#ifdef RYTHE_FMA_ENABLED
                return static_cast<Scalar>(internal::sse_avx_rsqrtf(static_cast<float32>(value)));
#else
                return static_cast<Scalar>(internal::sse_rsqrtf(static_cast<float32>(value)));
#endif
            }
        }
    }

    namespace internal
    {
        template <typename T>
        struct compute_sqrt;

        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        struct compute_sqrt<vector<Scalar, Size, Mode>>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static value_type compute(const value_type& v) noexcept
            {
                value_type result;
                for (size_type i; i < size; i++)
                {
                    result[i] = rsl::math::sqrt(v[i]);
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static value_type compute_rcp(const value_type& v) noexcept
            {
                value_type result;
                for (size_type i; i < size; i++)
                {
                    result[i] = rsl::math::rcp_sqrt(v[i]);
                }
                return result;
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_sqrt<vector<Scalar, 1u, Mode>>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar v) noexcept
            {
                return rsl::math::sqrt(v);
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute_rcp(Scalar v) noexcept
            {
                return rsl::math::rcp_sqrt(v);
            }
        };
    } // namespace internal

    template <vector_type VecType>
    constexpr auto sqrt(const VecType& v) noexcept
    {
        return internal::compute_sqrt<VecType>::compute(v);
    }

    template <vector_type VecType>
    constexpr auto rcp_sqrt(const VecType& v) noexcept
    {
        return internal::compute_sqrt<VecType>::compute_rcp(v);
    }
} // namespace rsl::math
