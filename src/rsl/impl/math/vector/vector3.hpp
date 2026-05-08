#pragma once
#include "../../defines.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 3, Mode> : internal::vector_additions<Scalar, 3, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type size = 3;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<Scalar, size, Mode>;

        union
        {
            storage_type data;

#include "swizzle/swizzle3.inl"
        };

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

        template <math::storage_mode OtherMode>
        [[rythe_always_inline]] explicit constexpr vector(
            const vector<scalar, 2, OtherMode>& v, scalar s = static_cast<scalar>(0)
        ) noexcept;

        template <math::storage_mode OtherMode>
        [[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept;
        [[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

        template <typename VecType>
            requires not_same_as<Scalar, typename VecType::scalar> || (VecType::size != 3)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
    };

    template <storage_mode Mode>
    struct vector<bool, 3, Mode> : internal::vector_additions<bool, 3, Mode>
    {
        using scalar = bool;
        static constexpr size_type size = 3;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<bool, size, Mode>;

        union
        {
            storage_type data;

#include "swizzle/swizzle3.inl"
        };

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

        template <math::storage_mode OtherMode>
        [[rythe_always_inline]] explicit constexpr vector(
            const vector<scalar, 2, OtherMode>& v, scalar s = static_cast<scalar>(0)
        ) noexcept;

        template <math::storage_mode OtherMode>
        [[rythe_always_inline]] constexpr vector(scalar s, const vector<scalar, 2, OtherMode>& v) noexcept;
        [[rythe_always_inline]] constexpr vector(scalar _x, scalar _y, scalar _z = static_cast<scalar>(0)) noexcept;

        template <typename VecType>
            requires not_same_as<bool, typename VecType::scalar> || (VecType::size != 3)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept
        {
            data[0] = (mask & 1) != 0;
            data[1] = (mask & 2) != 0;
            data[2] = (mask & 4) != 0;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
        {
            return static_cast<bitfield8>(
                static_cast<uint32>(data[0]) | static_cast<uint32>(data[1]) << 1u | static_cast<uint32>(data[2]) << 2u
            );
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept
        {
            return data[0] && data[1] && data[2];
        }

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
    };

    using float3 = vector<float32, 3>;
    using double3 = vector<float64, 3>;
    using int3 = vector<int32, 3>;
    using uint3 = vector<uint32, 3>;
    using bool3 = vector<bool, 3>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
