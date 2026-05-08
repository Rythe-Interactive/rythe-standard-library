#pragma once
#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_negate;

        template <linear_algebraic_construct ConstructType>
        struct compute_negate<ConstructType>
        {
            static constexpr size_type size = ConstructType::size;
            using value_type = ConstructType;

            [[nodiscard]] [[rythe_always_inline]] constexpr static value_type compute(const value_type& a) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result.data[i] = -a.data[i];
                }
                return result;
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_negate<vector<Scalar, 1u, Mode>>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar a) noexcept { return -a; }
        };
    } // namespace internal

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto negate(const VecType& a) noexcept
    {
        return internal::compute_negate<VecType>::compute(a);
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator-(const VecType& a)
    {
        return internal::compute_negate<VecType>::compute(a);
    }

    template <vector_type VecType>
    [[rythe_always_inline]] constexpr VecType& negate_assign(VecType& a) noexcept
    {
        return a = internal::compute_negate<VecType>::compute(a);
    }

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto negate(const QuatType& a) noexcept
    {
        return internal::compute_negate<QuatType>::compute(a);
    }

    template <quat_type QuatType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator-(const QuatType& a)
    {
        return internal::compute_negate<QuatType>::compute(a);
    }

    template <quat_type QuatType>
    [[rythe_always_inline]] constexpr QuatType& negate_assign(QuatType& a) noexcept
    {
        return a = internal::compute_negate<QuatType>::compute(a);
    }
} // namespace rsl::math
