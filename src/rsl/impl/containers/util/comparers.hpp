#pragma once
#include "../../defines.hpp"

namespace rsl
{
	template <typename T>
	struct equal
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
		{
			return lhs == rhs;
		}
	};

    template <typename T>
    struct less
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
        {
            return lhs < rhs;
        }
    };

    template <typename T>
    struct lequal
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
        {
            return lhs <= rhs;
        }
    };

    template <typename T>
    struct greater
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
        {
            return lhs > rhs;
        }
    };

    template <typename T>
    struct gequal
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator()(const T& lhs, const T& rhs) const noexcept
        {
            return lhs >= rhs;
        }
    };
} // namespace rsl
