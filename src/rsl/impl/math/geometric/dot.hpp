#pragma once
#include "../../rsl_core.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_dot;

        template <typename Scalar, size_type Size, storage_mode Mode>
        struct compute_dot<vector<Scalar, Size, Mode>>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar
            compute(const value_type& a, const value_type& b) noexcept
            {
                Scalar result = 0;
                for (size_type i = 0; i < size; i++)
                {
                    result += a[i] * b[i];
                }
                return result;
            }
        };
    } // namespace internal

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto dot(const VecType0& a, const VecType1& b) noexcept
    {
        return internal::compute_dot<elevated_t<VecType0, VecType1>>::compute(a, b);
    }
} // namespace rsl::math
