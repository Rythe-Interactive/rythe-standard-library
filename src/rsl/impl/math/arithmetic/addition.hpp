#pragma once
#include "../../rsl_core.hpp"
#include "../util/type_util.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        struct compute_addition
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static value_type
            compute(const value_type& a, const value_type& b) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = a[i] + b[i];
                }
                return result;
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr static value_type
            compute(const value_type& a, Scalar b) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = a[i] + b;
                }
                return result;
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_addition<Scalar, 1u, Mode>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static Scalar compute(Scalar a, Scalar b) noexcept
            {
                return a + b;
            }
        };

        template <typename T>
        struct compute_quat_addition;

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_quat_addition<quaternion<Scalar, Mode>>
        {
            using value_type = quaternion<Scalar>;

            [[nodiscard]] [[rythe_always_inline]] constexpr static auto
            compute(const value_type& a, const value_type& b) noexcept
            {
                value_type result;
                result.i = a.i + b.i;
                result.j = a.j + b.j;
                result.k = a.k + b.k;
                result.w = a.w + b.w;
                return result;
            }
        };
    } // namespace internal

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto add(const VecType0& a, const VecType1& b) noexcept
    {
        return internal::compute_addition<typename VecType0::scalar, VecType0::size, VecType0::mode>::compute(a, b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator+(const VecType0& a, const VecType1& b) noexcept
    {
        return internal::compute_addition<typename VecType0::scalar, VecType0::size, VecType0::mode>::compute(a, b);
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto add(const VecType& a, typename VecType::scalar b) noexcept
    {
        return internal::compute_addition<typename VecType::scalar, VecType::size, VecType::mode>::compute(a, b);
    }

    template <vector_type VecType>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto
    operator+(const VecType& a, typename VecType::scalar b) noexcept
    {
        return internal::compute_addition<typename VecType::scalar, VecType::size, VecType::mode>::compute(a, b);
    }

    template <quat_type QuatType0, quat_type QuatType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto add(const QuatType0& a, const QuatType1& b) noexcept
    {
        return internal::compute_quat_addition<QuatType0>::compute(a, b);
    }
    template <quat_type QuatType0, quat_type QuatType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto operator+(const QuatType0& a, const QuatType1& b) noexcept
    {
        return internal::compute_quat_addition<QuatType0>::compute(a, b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[rythe_always_inline]] constexpr VecType0& add_assign(VecType0& a, const VecType1& b) noexcept
    {
        return a = internal::compute_addition<typename VecType0::scalar, VecType0::size, VecType0::mode>::compute(a, b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[rythe_always_inline]] constexpr VecType0& operator+=(VecType0& a, const VecType1& b) noexcept
    {
        return a = internal::compute_addition<typename VecType0::scalar, VecType0::size, VecType0::mode>::compute(a, b);
    }

    template <vector_type VecType>
    [[rythe_always_inline]] constexpr VecType& add_assign(VecType& a, typename VecType::scalar b) noexcept
    {
        return a = internal::compute_addition<typename VecType::scalar, VecType::size, VecType::mode>::compute(a, b);
    }

    template <vector_type VecType>
    [[rythe_always_inline]] constexpr VecType& operator+=(VecType& a, typename VecType::scalar b) noexcept
    {
        return a = internal::compute_addition<typename VecType::scalar, VecType::size, VecType::mode>::compute(a, b);
    }
} // namespace rsl::math
