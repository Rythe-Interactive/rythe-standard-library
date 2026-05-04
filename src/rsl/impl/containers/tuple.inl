#pragma once

namespace rsl
{
    namespace internal
    {
        template<size_type I, typename Tuple>
        struct _rest_tuple;

        template<typename T, typename... Rest>
        struct _rest_tuple<0, tuple<T, Rest...>>
        {
            using type = tuple<T, Rest...>;
        };

        template <size_type I, typename T, typename... Rest>
        struct _rest_tuple<I, tuple<T, Rest...>> : _rest_tuple<I-1, tuple<Rest...>>
        {
        };

        template <size_type I, typename Tuple>
        using _rest_tuple_t = _rest_tuple<I, Tuple>::type;
    }

    template <size_type I, typename... Types>
    constexpr const element_at_t<I, Types...>& get(const tuple<Types...>& val) noexcept
    {
        return static_cast<const internal::_rest_tuple_t<I, tuple<Types...>>&>(val).firstValue;
    }

    template <size_type I, typename... Types>
    constexpr element_at_t<I, Types...>& get(tuple<Types...>& val) noexcept
    {
        return static_cast<internal::_rest_tuple_t<I, tuple<Types...>>&>(val).firstValue;
    }

    template <size_type I, typename... Types>
    constexpr element_at_t<I, Types...>&& get(tuple<Types...>&& val) noexcept
    {
        return rsl::move(static_cast<internal::_rest_tuple_t<I, tuple<Types...>>&>(val).firstValue);
    }

    template <typename T, typename... Types>
    constexpr const T& get(const tuple<Types...>& val) noexcept
    {
        return static_cast<const internal::_rest_tuple_t<index_of_element_v<T, Types...>, tuple<Types...>>&>(val).firstValue;
    }

    template <typename T, typename... Types>
    constexpr T& get(tuple<Types...>& val) noexcept
    {
        return static_cast<internal::_rest_tuple_t<index_of_element_v<T, Types...>, tuple<Types...>>&>(val).firstValue;
    }

    template <typename T, typename... Types>
    constexpr T&& get(tuple<Types...>&& val) noexcept
    {
        return rsl::move(static_cast<internal::_rest_tuple_t<index_of_element_v<T, Types...>, tuple<Types...>>&>(val).firstValue);
    }
}

namespace std
{
    template <size_t I, typename... Types>
    struct tuple_element<I, rsl::tuple<Types...>> : rsl::tuple_element<I, rsl::tuple<Types...>>
    {
    };

    template <typename... Types>
    struct tuple_size<rsl::tuple<Types...>> : rsl::tuple_size<rsl::tuple<Types...>>
    {};
}
