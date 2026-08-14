#pragma once
#include "../../rsl_core.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_cross;

        template <vector_type VecType>
            requires(VecType::size == 3 || VecType::size == 7)
        struct compute_cross<VecType>
        {
            static constexpr size_type size = VecType::size;
            using value_type = VecType;

            [[rythe_always_inline]] constexpr static size_type index_at(const size_type index) noexcept
            {
                return index % size;
            }

            [[rythe_always_inline]] constexpr static value_type
            compute(const value_type& a, const value_type& b) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = a[index_at(i + 1)] * b[index_at(i + 2)] - b[index_at(i + 1)] * a[index_at(i + 2)];
                }
                return result;
            }
        };
    } // namespace internal

    template <vector_type VecType0, vector_type VecType1>
        requires(
            remove_cvr_t<VecType0>::size == remove_cvr_t<VecType1>::size &&
            (remove_cvr_t<VecType0>::size == 3 || remove_cvr_t<VecType0>::size == 7)
        )
    [[nodiscard]] [[rythe_always_inline]] constexpr auto cross(const VecType0& a, const VecType1& b) noexcept
    {
        return internal::compute_cross<elevated_t<remove_cvr_t<VecType0>, remove_cvr_t<VecType1>>>::compute(a, b);
    }
} // namespace rsl::math
