#pragma once
#include "../basic/abs.hpp"
#include "type_util.hpp"

namespace rsl::math
{
    template <typename TypeA, typename TypeB>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool close_enough(TypeA lhs, TypeB rhs)
    {
        constexpr bool lhsIsInt = is_integer_v<TypeA>;
        constexpr bool rhsIsInt = is_integer_v<TypeB>;

        if constexpr (lhsIsInt && rhsIsInt)
        {
            return lhs == rhs;
        }
        else if constexpr (lhsIsInt || rhsIsInt)
        {
            using fp_t = select_floating_type_t<TypeA, TypeB>;
            return abs(static_cast<fp_t>(lhs) - static_cast<fp_t>(rhs)) <= epsilon_v<fp_t>;
        }
        else
        {
            using lowest_t = lowest_precision_t<TypeA, TypeB>;
            return abs(static_cast<lowest_t>(lhs) - static_cast<lowest_t>(rhs)) <= epsilon_v<lowest_t>;
        }
    }
} // namespace rsl::math
