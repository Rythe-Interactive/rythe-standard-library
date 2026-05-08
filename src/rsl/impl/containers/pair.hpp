#pragma once

#include "../util/concepts.hpp"

namespace rsl
{
    template <typename T1, typename T2>
    struct pair
    {
        using first_type = T1;
        using second_type = T2;

        template <constructable_type<> U1 = T1, constructable_type<> U2 = T2>
        [[rythe_always_inline]] constexpr pair()
            noexcept(is_nothrow_constructible_v<U1> && is_nothrow_constructible_v<U2>);

        [[rythe_always_inline]] constexpr pair(first_type&& lhs, second_type&& rhs)
            noexcept(is_nothrow_move_constructible_v<first_type> && is_nothrow_move_constructible_v<second_type>);

        template <typename U1, typename U2>
        [[rythe_always_inline]] constexpr pair(U1&& lhs, U2&& rhs)
            noexcept(is_nothrow_constructible_v<first_type, U1> && is_nothrow_constructible_v<second_type, U2>);

        first_type first;
        second_type second;
    };
} // namespace rsl

#include "pair.inl"
