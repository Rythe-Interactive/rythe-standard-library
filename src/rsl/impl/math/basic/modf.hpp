#pragma once
#include <cmath>

#include "../../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_modf;

        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        struct compute_modf<vector<Scalar, Size, Mode>>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[rythe_always_inline]] static value_type compute(const value_type& val, value_type& integer) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = ::std::modf(val[i], &integer[i]);
                }
                return result;
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_modf<vector<Scalar, 1ull, Mode>>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size, Mode>;

            template <typename Ret>
            [[rythe_always_inline]] static value_type compute(Scalar val, Ret& integer) noexcept
            {
                if constexpr (is_vector_v<Ret>)
                {
                    return ::std::modf(val, &integer[0]);
                }
                else
                {
                    return ::std::modf(val, &integer);
                }
            }
        };
    } // namespace internal

    template <typename T, typename Ret>
    [[rythe_always_inline]] static auto modf(const T& val, Ret& integer)
    {
        if constexpr (is_linear_algebraic_construct_v<T>)
        {
            static_assert(
                is_floating_point_v<typename T::scalar>,
                "Value must be floating point in order to use modf. (Did you mean fmod?)"
            );
            return internal::compute_modf<T>::compute(val, integer);
        }
        else
        {
            static_assert(
                is_floating_point_v<T>, "Value must be floating point in order to use modf. (Did you mean fmod?)"
            );
            return ::std::modf(val, &integer);
        }
    }
} // namespace rsl::math
