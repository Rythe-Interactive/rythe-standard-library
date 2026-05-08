#pragma once
#include "../arithmetic/subtraction.hpp"
#include "length.hpp"

namespace rsl::math
{
    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto dist(const VecType0& a, const VecType1& b) noexcept
    {
        return length(a - b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto dist2(const VecType0& a, const VecType1& b) noexcept
    {
        return length2(a - b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto distance(const VecType0& a, const VecType1& b) noexcept
    {
        return length(a - b);
    }

    template <vector_type VecType0, vector_type VecType1>
    [[nodiscard]] [[rythe_always_inline]] constexpr auto distance2(const VecType0& a, const VecType1& b) noexcept
    {
        return length2(a - b);
    }
} // namespace rsl::math
