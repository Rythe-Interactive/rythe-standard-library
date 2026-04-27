#pragma once
#include "../util/concepts.hpp"
#include "../util/common.hpp"
#include "../math/util/limits.hpp"

namespace rsl
{
    struct time_span
    {
        int64 value;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T hours() const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T minutes() const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T seconds() const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T milliseconds() const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T microseconds() const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T nanoseconds() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr float32 hours() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 minutes() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 seconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 milliseconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 microseconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 nanoseconds() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator float32() const noexcept;

        [[rythe_always_inline]] constexpr time_span operator-() noexcept;

        [[rythe_always_inline]] constexpr time_span& operator+=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator-=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator*=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator/=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator%=(const time_span& rhs) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator+(const time_span& rhs) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator-(const time_span& rhs) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator*(const time_span& rhs) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator/(const time_span& rhs) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator%(const time_span& rhs) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr auto operator<=>(const time_span& rhs) noexcept;

        static const time_span zero;
        static const time_span infinity;
    };

    inline namespace literals
    {
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_ns(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_us(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_ms(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_s(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_min(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_hr(const size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_fps(const size_type count) noexcept;
    }
} // namespace rsl

#include "time_span.inl"
