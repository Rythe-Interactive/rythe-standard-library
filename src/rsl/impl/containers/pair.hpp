#pragma once

#include "../rsl_core.hpp"

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

    template <typename T1>
    struct pair<T1, void>
    {
        using first_type = T1;
        using second_type = void;

        template <constructable_type<> U1 = T1, typename U2 = void>
        [[rythe_always_inline]] constexpr pair() noexcept(is_nothrow_constructible_v<U1>)
            : first()
        {}

        [[rythe_always_inline]] constexpr pair(first_type&& lhs) noexcept(is_nothrow_move_constructible_v<first_type>)
            : first(rsl::move(lhs))
        {}

        template <typename U1, typename U2>
        [[rythe_always_inline]] constexpr pair(U1&& lhs) noexcept(is_nothrow_constructible_v<first_type, U1>)
            : first(rsl::forward<U1>(lhs))
        {}

        first_type first;
    };

    template <typename T2>
    struct pair<void, T2>
    {
        using first_type = void;
        using second_type = T2;

        template <typename U1 = void, constructable_type<> U2 = T2>
        [[rythe_always_inline]] constexpr pair() noexcept(is_nothrow_constructible_v<U2>)
            : second()
        {}

        [[rythe_always_inline]] constexpr pair(second_type&& rhs) noexcept(is_nothrow_move_constructible_v<second_type>)
            : second(rsl::move(rhs))
        {}

        template <typename U2>
        [[rythe_always_inline]] constexpr pair(U2&& rhs) noexcept(is_nothrow_constructible_v<second_type, U2>)
            : second(rsl::forward<U2>(rhs))
        {}

        second_type second;
    };
} // namespace rsl

#include "pair.inl"
