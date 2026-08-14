#pragma once
#include "../../rsl_core.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <typename>
        struct compute_not;

        template <vector_type vec_type>
            requires same_as<bool, typename vec_type::scalar>
        struct compute_not<vec_type>
        {
            [[nodiscard]] [[rythe_always_inline]] constexpr static vec_type compute(const vec_type& value) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    result[i] = !value[i];
                }
                return result;
            }
        };

        template <>
        struct compute_not<bool>
        {
            [[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(bool value) noexcept { return !value; }
        };
    } // namespace internal

    template <vector_type vec_type>
        requires same_as<bool, typename vec_type::scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr vec_type invert(const vec_type& value) noexcept
    {
        return internal::compute_not<vec_type>::compute(value);
    }

    [[nodiscard]] [[rythe_always_inline]] constexpr bool invert(bool value) noexcept
    {
        return internal::compute_not<bool>::compute(value);
    }

    template <vector_type vec_type>
        requires same_as<bool, typename vec_type::scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr vec_type operator!(const vec_type& value) noexcept
    {
        return internal::compute_not<vec_type>::compute(value);
    }
} // namespace rsl::math
