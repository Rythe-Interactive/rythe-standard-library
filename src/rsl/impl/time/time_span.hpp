#pragma once
#include "../rsl_core.hpp"

namespace rsl
{
    struct time_span
    {
        int64 value;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] constexpr T days() const noexcept;

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

        [[nodiscard]] [[rythe_always_inline]] constexpr float32 days() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 hours() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 minutes() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 seconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 milliseconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 microseconds() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr float32 nanoseconds() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator float32() const noexcept;

        [[rythe_always_inline]] constexpr time_span operator-() const noexcept;

        [[rythe_always_inline]] constexpr time_span& operator+=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator-=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator*=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator/=(const time_span& rhs) noexcept;
        [[rythe_always_inline]] constexpr time_span& operator%=(const time_span& rhs) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator+(const time_span& rhs) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator-(const time_span& rhs) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator*(const time_span& rhs) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator/(const time_span& rhs) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr time_span operator%(const time_span& rhs) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr auto operator<=>(const time_span& rhs) const noexcept;

        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_nanoseconds(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_microseconds(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_milliseconds(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_seconds(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_minutes(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_hours(T count) noexcept;
        template <arithmetic_type T>
        [[nodiscard]] [[rythe_always_inline]] static constexpr time_span from_days(T count) noexcept;

        static const time_span zero;
        static const time_span infinity;
    };

    inline namespace literals
    {
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_ns(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_us(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_ms(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_s(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_min(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_hr(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_days(size_type count) noexcept;
        [[nodiscard]] [[rythe_always_inline]] consteval time_span operator""_fps(size_type count) noexcept;
    }
} // namespace rsl

#include "time_span.inl"
