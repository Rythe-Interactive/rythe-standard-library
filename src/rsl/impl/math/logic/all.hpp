#pragma once
#include "../../rsl_core.hpp"
#include "../vector/vector.hpp"

namespace rsl::math
{
    namespace internal
    {
        template <vector_type vec_type>
            requires same_as<bool, typename vec_type::scalar>
        struct compute_all
        {
            [[nodiscard]] [[rythe_always_inline]] constexpr static bool compute(vec_type value) noexcept
            {
                for (size_type i = 0; i < vec_type::size; i++)
                {
                    if (!value[i])
                    {
                        return false;
                    }
                }
                return true;
            }
        };
    } // namespace internal

    template <vector_type vec_type>
        requires same_as<bool, typename vec_type::scalar>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool all(const vec_type& value) noexcept
    {
        return internal::compute_all<vec_type>::compute(value);
    }
} // namespace rsl::math
