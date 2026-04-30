#pragma once
#include <immintrin.h>

#include "../../defines.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
	template <arithmetic_type Scalar, storage_mode Mode>
	struct vector<Scalar, 4, Mode> : internal::vector_additions<Scalar, 4, Mode>
	{
		using scalar = Scalar;
		static constexpr size_type size = 4;
		static constexpr storage_mode mode = Mode;
		using storage_type = storage_t<Scalar, size, Mode>;

		union
		{
			storage_type data;
			__m128 intrin[(sizeof(scalar) * size) / sizeof(__m128)];

#include "swizzle/swizzle4.inl"
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 3, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(
			scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s0 = static_cast<scalar>(0),
			scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode Mode0, math::storage_mode Mode1>
		[[rythe_always_inline]] constexpr vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
			noexcept;

		[[rythe_always_inline]] constexpr vector(
			scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)
		) noexcept;

		template <typename VecType>
			requires not_same_as<Scalar, typename VecType::scalar> || (VecType::size != 4)
		[[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	template <storage_mode Mode>
	struct vector<bool, 4, Mode> : internal::vector_additions<bool, 4, Mode>
	{
		using scalar = bool;
		static constexpr size_type size = 4;
		static constexpr storage_mode mode = Mode;
		using storage_type = storage_t<bool, size, Mode>;

		union
		{
			storage_type data;

#include "swizzle/swizzle4.inl"
		};

		[[rythe_always_inline]] constexpr vector() noexcept;
		[[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
		[[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 3, OtherMode>& v, scalar s = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 3, OtherMode>& v) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(
			scalar s0, const vector<scalar, 2, OtherMode>& v, scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] constexpr vector(scalar s0, scalar s1, const vector<scalar, 2, OtherMode>& v) noexcept;

		template <math::storage_mode OtherMode>
		[[rythe_always_inline]] explicit constexpr vector(
			const vector<scalar, 2, OtherMode>& v, scalar s0 = static_cast<scalar>(0),
			scalar s1 = static_cast<scalar>(0)
		) noexcept;

		template <math::storage_mode Mode0, math::storage_mode Mode1>
		[[rythe_always_inline]] constexpr vector(const vector<scalar, 2, Mode0>& v0, const vector<scalar, 2, Mode1>& v1)
			noexcept;

		[[rythe_always_inline]] constexpr vector(
			scalar _x, scalar _y, scalar _z = static_cast<scalar>(0), scalar _w = static_cast<scalar>(0)
		) noexcept;

		template <typename VecType>
			requires not_same_as<bool, typename VecType::scalar> || (VecType::size != 4)
		[[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

		[[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
		{
			data[0] = (mask & 1) != 0;
			data[1] = (mask & 2) != 0;
			data[2] = (mask & 4) != 0;
			data[3] = (mask & 8) != 0;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
		{
			return static_cast<bitfield8>(
				static_cast<uint32>(data[0]) | static_cast<uint32>(data[1]) << 1u | static_cast<uint32>(data[2]) << 2u |
				static_cast<uint32>(data[3]) << 3u
			);
		}

		[[rythe_always_inline]] constexpr operator bool() const noexcept
		{
			return data[0] && data[1] && data[2] && data[3];
		}

		[[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
	};

	using float4 = vector<float32, 4>;
	using double4 = vector<float64, 4>;
	using int4 = vector<int32, 4>;
	using uint4 = vector<uint32, 4>;
	using bool4 = vector<bool, 4>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
