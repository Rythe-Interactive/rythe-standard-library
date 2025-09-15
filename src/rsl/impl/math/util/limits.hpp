#pragma once
#include "../../util/primitives.hpp"

#include "type_util.hpp"

namespace rsl::math
{
	namespace internal
	{
		template <arithmetic_type T>
		struct bit_rep_impl;

		template <arithmetic_type T>
			requires integral_type<T>
		struct bit_rep_impl<T>
		{
			using type = T;
		};

		template <arithmetic_type T>
			requires floating_point_type<T>
		struct bit_rep_impl<T>
		{
			using type = unsigned_integer_of_size_t<sizeof(T)>;
		};
	} // namespace internal

	template <arithmetic_type T>
	using bit_rep = typename internal::bit_rep_impl<T>::type;

	template <signed_type T>
	constexpr bit_rep<T> sign_bit = static_cast<bit_rep<T>>(1) << (sizeof(T) * 8 - 1);

	namespace internal
	{
		template <floating_point_type T>
		struct mantissa_bits_impl;

		template <>
		struct mantissa_bits_impl<float32>
		{
			constexpr static size_type value = 23;
		};

		template <>
		struct mantissa_bits_impl<float64>
		{
			constexpr static size_type value = 52;
		};
	} // namespace internal

	template <floating_point_type T>
	constexpr size_type mantissa_bits = internal::mantissa_bits_impl<T>::value;

	template <floating_point_type T>
	constexpr size_type exponent_bits = (sizeof(T) * 8 - 1) - mantissa_bits<T>;

	template <typename T>
	constexpr bit_rep<T> mantissa_mask = (~sign_bit<T> >> exponent_bits<T>);

	template <typename T>
	constexpr bit_rep<T> exponent_mask = ~mantissa_mask<T> & ~sign_bit<T>;

	template <arithmetic_type T>
	struct limits;

	template <arithmetic_type T>
		requires floating_point_type<T>
	struct limits<T>
	{
		constexpr static size_type bit_count = sizeof(T) * 8;
		constexpr static T min = bit_cast<T>(static_cast<bit_rep<T>>(1) << mantissa_bits<T>);
		constexpr static T max = bit_cast<T>((exponent_mask<T> - static_cast<bit_rep<T>>(1)) | mantissa_mask<T>);
		constexpr static T lowest = bit_cast<T>(sign_bit<T> | bit_cast<bit_rep<T>>(max));
		constexpr static T infinity = bit_cast<T>(exponent_mask<T>);
		constexpr static T quiet_nan = bit_cast<T>((bit_cast<bit_rep<T>>(min) >> 1) | exponent_mask<T>);
#if defined(RYTHE_MSVC) || defined(RYTHE_CLANG_MSVC)
		constexpr static T signaling_nan =
			bit_cast<T>((bit_cast<bit_rep<T>>(min) >> mantissa_bits<T>) | exponent_mask<T>);
#else
		constexpr static T signaling_nan = bit_cast<T>((bit_cast<bit_rep<T>>(min) >> 2) | exponent_mask<T>);
#endif
	};

	template <arithmetic_type T>
		requires integral_type<T> && unsigned_type<T>
	struct limits<T>
	{
		constexpr static size_type bit_count = sizeof(T) * 8;
		constexpr static T min = 0u;
		constexpr static T max = static_cast<T>(~0u);
	};

	template <arithmetic_type T>
		requires integral_type<T> && signed_type<T>
	struct limits<T>
	{
		constexpr static size_type bit_count = sizeof(T) * 8;
		constexpr static T min = sign_bit<T>;
		constexpr static T max = ~0 & ~sign_bit<T>;
	};
} // namespace rsl::math
