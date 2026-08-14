#pragma once
#include "../../rsl_core.hpp"
#include "../quaternion/quaternion.hpp"
#include "../util/type_util.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename T>
        struct compute_division;

        template <typename Scalar>
        struct compute_division<quaternion<Scalar>>
        {
            static constexpr size_type size = 4u;
            using value_type = quaternion<Scalar>;

            [[nodiscard]] constexpr static value_type compute(const value_type& a, const value_type& b) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = a[i] / b[i];
                }
                return result;
            }

            [[nodiscard]] constexpr static value_type compute(const value_type& a, Scalar b) noexcept
            {
                value_type result;
                Scalar inv = static_cast<Scalar>(1) / b;
                for (size_type i = 0; i < size; i++)
                {
                    result[i] = a[i] * inv;
                }
                return result;
            }
        };
    } // namespace internal
} // namespace rsl::math
