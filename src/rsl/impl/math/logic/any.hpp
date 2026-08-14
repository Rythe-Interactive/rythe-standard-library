#pragma once
#include "../../rsl_core.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <vector_type vec_type>
            requires same_as<bool, typename vec_type::scalar>
        struct compute_any
        {
            [[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(const vec_type& value) noexcept
            {
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    if (value[i])
                    {
                        return true;
                    }
                }
                return false;
            }
        };
    } // namespace internal

    template <vector_type vec_type>
        requires same_as<bool, typename vec_type::scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool any(const vec_type& value) noexcept
    {
        return internal::compute_any<vec_type>::compute(value);
    }
} // namespace rsl::math
