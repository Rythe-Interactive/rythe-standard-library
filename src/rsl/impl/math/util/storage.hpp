#pragma once

#include "../../defines.hpp"
#include "../../util/common.hpp"
#include "../../util/concepts.hpp"
#include "../../util/primitives.hpp"

namespace rsl::math
{
#ifndef RSL_DEFAULT_ALIGNED_MATH
	#define RSL_DEFAULT_ALIGNED_MATH false
#endif

#ifndef RSL_DEFAULT_MATH_MODE
	#if RSL_DEFAULT_ALIGNED_MATH
		#define RSL_DEFAULT_MATH_MODE aligned
	#else
		#define RSL_DEFAULT_MATH_MODE packed
	#endif
#endif

	enum struct [[rythe_closed_enum]] storage_mode : uint8
	{
#if RSL_DEFAULT_ALIGNED_MATH
		aligned,
		packed,
#else
		packed,
		aligned,
#endif
		defaultStorage = RSL_DEFAULT_MATH_MODE,
	};

	template <storage_mode M>
	struct is_aligned : false_type
	{
	};

	template <>
	struct is_aligned<storage_mode::aligned> : true_type
	{
	};

	template <storage_mode M>
	constexpr bool is_aligned_v = is_aligned<M>::value;

	template <arithmetic_type T, size_type N, bool Aligned>
	struct storage;

	template <arithmetic_type T, size_type N>
	struct storage<T, N, false>
	{
		struct type
		{
			T data[N];

			[[nodiscard]] [[rythe_always_inline]] constexpr T& operator[](size_type i) noexcept { return data[i]; }

			[[nodiscard]] [[rythe_always_inline]] constexpr const T& operator[](size_type i) const noexcept
			{
				return data[i];
			}
		};
	};

	namespace internal
	{
		consteval size_type next_power_of_two(const size_type n)
		{
			size_type powerOfTwo = 1;
			while (powerOfTwo < n)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
	} // namespace internal

	template <arithmetic_type T, size_type N>
	struct storage<T, N, true>
	{
		struct alignas(internal::next_power_of_two(N) * sizeof(T)) type
		{
			T data[internal::next_power_of_two(N)];

			[[nodiscard]] [[rythe_always_inline]] constexpr T& operator[](size_type i) noexcept { return data[i]; }

			[[nodiscard]] [[rythe_always_inline]] constexpr const T& operator[](size_type i) const noexcept
			{
				return data[i];
			}
		};
	};

	template <arithmetic_type T, size_type N, storage_mode M>
	using storage_t = typename storage<T, N, is_aligned_v<M>>::type;
} // namespace rsl::math
