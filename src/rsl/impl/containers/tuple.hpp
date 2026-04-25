#pragma once
#include "../util/concepts.hpp"

namespace rsl
{
    template<typename...>
    struct tuple;

	template <size_type I, typename Tuple>
    struct tuple_element;

    template <size_type I, typename... Types>
    struct tuple_element<I, tuple<Types...>>
    {
        using type = element_at_t<I, Types...>;
    };

    template <size_type I, typename Tuple>
    using tuple_element_t = typename tuple_element<I, Tuple>::type;

    template <size_type I, typename... Types>
    [[rythe_always_inline]] constexpr const element_at_t<I, Types...>& get(const tuple<Types...>& val) noexcept;

    template <size_type I, typename... Types>
    [[rythe_always_inline]] constexpr element_at_t<I, Types...>& get(tuple<Types...>& val) noexcept;

    template <size_type I, typename... Types>
    [[rythe_always_inline]] constexpr tuple_element_t<I, tuple<Types...>>&& get(tuple<Types...>&& val) noexcept;

    template <typename T, typename... Types>
    [[rythe_always_inline]] constexpr const T& get(const tuple<Types...>& val) noexcept;

    template <typename T, typename... Types>
    [[rythe_always_inline]] constexpr T& get(tuple<Types...>& val) noexcept;

    template <typename T, typename... Types>
    [[rythe_always_inline]] constexpr T&& get(tuple<Types...>&& val) noexcept;

    template <typename Tuple>
    struct tuple_size;

    template <typename... Types>
    struct tuple_size<tuple<Types...>> : public integral_constant<size_type, sizeof...(Types)>
    {};

    template <typename Tuple>
    constexpr size_type tuple_size_v = tuple_size<Tuple>::value;

    template<>
    struct tuple<> {};

    template<typename T, typename... Rest>
    struct tuple<T, Rest...> : private tuple<Rest...>
    {
        [[rythe_always_inline]] constexpr
            explicit(conjunction_v<is_implicitly_default_constructible<T>, is_implicitly_default_constructible<Rest>...>)
                tuple()
            noexcept(conjunction_v<is_nothrow_constructible<T>, is_nothrow_constructible<Rest>...>)
            requires(conjunction_v<is_constructible<T>, is_constructible<Rest>...>)
        = default;

        template <typename T2, typename... Rest2>
        [[rythe_always_inline]] constexpr
            explicit(conjunction_v<is_convertible<const T2&, T>, is_convertible<const Rest2&, Rest>...>)
                tuple(const T2& arg, const Rest2&... rest)
            noexcept(conjunction_v<is_nothrow_constructible<T, const T2&>, is_nothrow_constructible<Rest, const Rest2&>...>)
            requires(sizeof...(Rest) == sizeof...(Rest2) && conjunction_v<is_constructible<T, const T2&>, is_constructible<Rest, const Rest2&>...>)
            : tuple<Rest...>(rest...),
              firstValue(arg)
        {}

        template <typename T2, typename... Rest2>
        [[rythe_always_inline]] constexpr
            explicit(conjunction_v<is_convertible<T2, T>, is_convertible<Rest2, Rest>...>)
                tuple(T2&& arg, Rest2&&... rest)
            noexcept(conjunction_v<is_nothrow_constructible<T, T2>, is_nothrow_constructible<Rest, Rest2>...>)
            requires (sizeof...(Rest) == sizeof...(Rest2) && conjunction_v<is_constructible<T, T2>, is_constructible<Rest, Rest2>...>)
            : tuple<Rest...>(rsl::forward<Rest2>(rest)...),
              firstValue(rsl::forward<T2>(arg))
        {}

        tuple(const tuple&) = default;
        tuple(tuple&&) = default;

        template <size_type I, typename... Types>
        friend constexpr const element_at_t<I, Types...>& get(const tuple<Types...>&) noexcept;

        template <size_type I, typename... Types>
        friend constexpr element_at_t<I, Types...>& get(tuple<Types...>&) noexcept;

        template <size_type I, typename... Types>
        friend constexpr element_at_t<I, Types...>&& get(tuple<Types...>&&) noexcept;

        template <typename T2, typename... Types>
        friend constexpr const T2& get(const tuple<Types...>&) noexcept;

        template <typename T2, typename... Types>
        friend constexpr T2& get(tuple<Types...>&) noexcept;

        template <typename T2, typename... Types>
        friend constexpr T2&& get(tuple<Types...>&&) noexcept;

        T firstValue;
    };
}

#include "tuple.inl"
