#pragma once
#include "../../rsl_core.hpp"
#include "predefined.hpp"
#include "vector_additions.hpp"

RYTHE_MSVC_SUPPRESS_WARNING_WITH_PUSH(4201) // anonymous struct

namespace rsl::math
{
    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 1, Mode> : internal::vector_additions<Scalar, 1, Mode>
    {
        using scalar = Scalar;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<Scalar, size, Mode>;

        union
        {
            storage_type data;

#include "swizzle/swizzle1.inl"
        };

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] constexpr vector(scalar s) noexcept;

        template <typename VecType>
            requires not_same_as<Scalar, typename VecType::scalar> || (VecType::size != 1)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator scalar() const noexcept { return data[0]; }

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
    };

    template <storage_mode Mode>
    struct vector<bool, 1, Mode> : internal::vector_additions<bool, 1, Mode>
    {
        using scalar = bool;
        static constexpr size_type size = 1;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<bool, size, Mode>;

        union
        {
            storage_type data;

#include "swizzle/swizzle1.inl"
        };

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] constexpr vector(scalar s) noexcept;

        template <typename VecType>
            requires not_same_as<bool, typename VecType::scalar> || (VecType::size != 1)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr void set_mask(bitfield8 mask) noexcept { data[0] = (mask & 1) != 0; }
        [[nodiscard]] [[rythe_always_inline]] constexpr bitfield8 mask() const noexcept
        {
            return static_cast<bitfield8>(data[0]);
        }

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept { return data[0]; }

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
    };

    using float1 = vector<float32, 1>;
    using double1 = vector<float64, 1>;
    using int1 = vector<int32, 1>;
    using uint1 = vector<uint32, 1>;
    using bool1 = vector<bool, 1>;
} // namespace rsl::math

RYTHE_MSVC_SUPPRESS_WARNING_POP
