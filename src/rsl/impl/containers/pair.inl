#pragma once
#include "pair.hpp"

namespace rsl
{
	template <typename T1, typename T2>
	template <constructable_type<> U1, constructable_type<> U2>
	inline constexpr pair<T1, T2>::pair() noexcept(is_nothrow_constructible_v<U1> && is_nothrow_constructible_v<U2>)
		: first(),
		  second()
	{
	}

	template <typename T1, typename T2>
	inline constexpr pair<T1, T2>::pair(first_type&& lhs, second_type&& rhs)
		noexcept(is_nothrow_move_constructible_v<first_type> && is_nothrow_move_constructible_v<second_type>)
		: first(move(lhs)),
		  second(move(rhs))
	{
	}

	template <typename T1, typename T2>
	template <typename U1, typename U2>
	inline constexpr pair<T1, T2>::pair(U1&& lhs, U2&& rhs)
		noexcept(is_nothrow_constructible_v<first_type, U1> && is_nothrow_constructible_v<second_type, U2>)
		: first(forward<U1>(lhs)),
		  second(forward<U2>(rhs))
	{
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator==(pair<A, B> const& x, pair<A, B> const& y) noexcept(
		noexcept(declval<A const&>() == declval<A const&>()) && noexcept(declval<B const&>() == declval<B const&>())
	)
	{
		return (x.first == y.first) && (x.second == y.second);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator!=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x == y))
	{
		return !(x == y);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator<(pair<A, B> const& x, pair<A, B> const& y) noexcept(
		noexcept(declval<A const&>() < declval<A const&>()) && noexcept(declval<B const&>() < declval<B const&>())
	)
	{
		return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator>(pair<A, B> const& x, pair<A, B> const& y) noexcept(noexcept(y < x))
	{
		return y < x;
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator<=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x > y))
	{
		return !(x > y);
	}

	template <typename A, typename B>
	[[rythe_always_inline]] constexpr bool operator>=(pair<A, B> const& x, pair<A, B> const& y)
		noexcept(noexcept(x < y))
	{
		return !(x < y);
	}

	template <size_type I, typename Tuple>
	struct tuple_element;

	template <size_type I, typename T1, typename T2>
	struct tuple_element<I, pair<T1, T2>>
	{
		using type = element_at_t<I, T1, T2>;
	};

	template <size_type I, typename T1, typename T2>
	const element_at_t<I, T1, T2>& get(const pair<T1, T2>& val)
	{
		if constexpr (I == 0)
		{
			return val.first;
		}
		else
		{
			return val.second;
		}
	}

	template <typename T, typename T1, typename T2>
	const T& get(const pair<T1, T2>& val)
	{
		return get<index_of_element_v<T, T1, T2>>(val);
	}

	template <size_type I, typename T1, typename T2>
	element_at_t<I, T1, T2>& get(pair<T1, T2>& val)
	{
		return const_cast<element_at_t<I, T1, T2>&>(get<I>(rsl::as_const(val)));
	}

	template <typename T, typename T1, typename T2>
	T& get(pair<T1, T2>& val)
	{
		return const_cast<element_at_t<index_of_element_v<T, T1, T2>, T1, T2>&>(get<T>(rsl::as_const(val)));
	}

	template <typename Tuple>
	struct tuple_size;

	template <typename T1, typename T2>
	struct tuple_size<pair<T1, T2>> : public integral_constant<size_type, 2>
	{
	};
} // namespace std
