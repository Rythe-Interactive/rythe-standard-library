#pragma once
#include "../../rsl_core.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        [[nodiscard]] [[rythe_always_inline]] constexpr auto abs_impl(T val)
        {
            using value_type = remove_cvr_t<T>;
            if constexpr (is_floating_point_v<value_type>)
            {
                return bit_cast<value_type>(bit_cast<bit_rep<value_type>>(val) & ~sign_bit<value_type>);
            }
            else
            {
                return val < 0 ? -val : val;
            }
        }

        template <typename T>
        struct compute_abs;

        template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
        struct compute_abs<vector<Scalar, Size, Mode>>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr value_type compute(const value_type& val) noexcept
            {
                if constexpr (!is_signed_v<Scalar>)
                {
                    return val;
                }
                else
                {
                    value_type result;
                    for (size_type i = 0; i < size; i++)
                    {
                        result[i] = internal::abs_impl(val[i]);
                    }
                    return result;
                }
            }
        };

        template <arithmetic_type Scalar, storage_mode Mode>
        struct compute_abs<vector<Scalar, 1ull, Mode>>
        {
            static constexpr size_type size = 1ull;
            using value_type = vector<Scalar, size, Mode>;

            [[nodiscard]] [[rythe_always_inline]] constexpr Scalar compute(Scalar val) noexcept
            {
                if constexpr (!is_signed_v<Scalar>)
                {
                    return val;
                }
                else
                {
                    return internal::abs_impl(val);
                }
            }
        };
    } // namespace internal

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto abs(T val)
    {
        using value_type = remove_cvr_t<T>;
        if constexpr (is_linear_algebraic_construct_v<value_type>)
        {
            return internal::compute_abs<value_type>::compute(val);
        }
        else if constexpr (!is_signed_v<value_type>)
        {
            return val;
        }
        else
        {
            return internal::abs_impl(val);
        }
    }
} // namespace rsl::math
