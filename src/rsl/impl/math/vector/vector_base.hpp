#pragma once
#include "predefined.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type Size, storage_mode Mode = storage_mode::defaultStorage>
    struct vector
    {
        using scalar = Scalar;
        static constexpr size_type size = Size;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<Scalar, Size, Mode>;

        storage_type data;

        static const vector one;
        static const vector zero;

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

        template <typename VecType>
            requires not_same_as<Scalar, typename VecType::scalar> || (VecType::size != Size)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;
    };

    template <size_type Size, storage_mode Mode>
    struct vector<bool, Size, Mode>
    {
        using scalar = bool;
        static constexpr size_type size = Size;
        static constexpr storage_mode mode = Mode;
        using storage_type = storage_t<bool, Size, Mode>;

        storage_type data;

        static const vector one;
        static const vector zero;

        [[rythe_always_inline]] constexpr vector() noexcept;
        [[rythe_always_inline]] constexpr vector(const vector&) noexcept = default;
        [[rythe_always_inline]] explicit constexpr vector(scalar s) noexcept;

        template <typename VecType>
            requires not_same_as<bool, typename VecType::scalar> || (VecType::size != Size)
        [[rythe_always_inline]] constexpr vector(const VecType& other) noexcept;

        [[rythe_always_inline]] constexpr vector& operator=(const vector&) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr scalar& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const scalar& operator[](size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept
        {
            bool result = true;
            for (size_type i = 0; i < size; ++i)
            {
                result &= data[i];
            }
            return result;
        }
    };
} // namespace rsl::math
