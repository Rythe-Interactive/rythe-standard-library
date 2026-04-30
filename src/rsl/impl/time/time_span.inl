#pragma once
#include "time_span.hpp"

namespace rsl
{
    template <arithmetic_type T>
    constexpr T time_span::hours() const noexcept
    {
        if constexpr (is_floating_point_v<T>)
        {
            return static_cast<T>(static_cast<float_max>(value) / static_cast<float_max>(3600_g));
        }
        else
        {
            return static_cast<T>(value / 3600_g);
        }
    }

    template <arithmetic_type T>
    constexpr T time_span::minutes() const noexcept
    {
        if constexpr (is_floating_point_v<T>)
        {
            return static_cast<T>(static_cast<float_max>(value) / static_cast<float_max>(60_g));
        }
        else
        {
            return static_cast<T>(value / 60_g);
        }
    }

    template <arithmetic_type T>
    constexpr T time_span::seconds() const noexcept
    {
        if constexpr (is_floating_point_v<T>)
        {
            return static_cast<T>(static_cast<float_max>(value) / static_cast<float_max>(1_g));
        }
        else
        {
            return static_cast<T>(value / 1_g);
        }
    }

    template <arithmetic_type T>
    constexpr T time_span::milliseconds() const noexcept
    {
        if constexpr (is_floating_point_v<T>)
        {
            return static_cast<T>(static_cast<float_max>(value) / static_cast<float_max>(1_m));
        }
        else
        {
            return static_cast<T>(value / 1_m);
        }
    }

    template <arithmetic_type T>
    constexpr T time_span::microseconds() const noexcept
    {
        if constexpr (is_floating_point_v<T>)
        {
            return static_cast<T>(static_cast<float_max>(value) / static_cast<float_max>(1_k));
        }
        else
        {
            return static_cast<T>(value / 1_k);
        }
    }

    template <arithmetic_type T>
    constexpr T time_span::nanoseconds() const noexcept
    {
        return static_cast<T>(value);
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_nanoseconds(const T count) noexcept
    {
        return { static_cast<int64>(count) };
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_microseconds(const T count) noexcept
    {
        return { static_cast<int64>(count * 1_k) };
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_milliseconds(const T count) noexcept
    {
        return { static_cast<int64>(count * 1_m) };
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_seconds(const T count) noexcept
    {
        return { static_cast<int64>(count * 1_g) };
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_minutes(const T count) noexcept
    {
        return { static_cast<int64>(count * 60_g) };
    }

    template <arithmetic_type T>
    inline constexpr time_span time_span::from_hours(const T count) noexcept
    {
        return { static_cast<int64>(count * 3600_g) };
    }

    constexpr float32 time_span::hours() const noexcept
    {
        return hours<float32>();
    }

    constexpr float32 time_span::minutes() const noexcept
    {
        return minutes<float32>();
    }

    constexpr float32 time_span::seconds() const noexcept
    {
        return seconds<float32>();
    }

    constexpr float32 time_span::milliseconds() const noexcept
    {
        return milliseconds<float32>();
    }

    constexpr float32 time_span::microseconds() const noexcept
    {
        return microseconds<float32>();
    }

    constexpr float32 time_span::nanoseconds() const noexcept
    {
        return nanoseconds<float32>();
    }

    constexpr time_span::operator float32() const noexcept
    {
        return seconds();
    }

    constexpr time_span time_span::operator-() const noexcept
    {
        return { -value };
    }

    constexpr time_span& time_span::operator+=(const time_span& rhs) noexcept
    {
        value += rhs.value;
        return *this;
    }

    constexpr time_span& time_span::operator-=(const time_span& rhs) noexcept
    {
        value -= rhs.value;
        return *this;
    }

    constexpr time_span& time_span::operator*=(const time_span& rhs) noexcept
    {
        value *= rhs.value;
        return *this;
    }

    constexpr time_span& time_span::operator/=(const time_span& rhs) noexcept
    {
        value /= rhs.value;
        return *this;
    }

    constexpr time_span& time_span::operator%=(const time_span& rhs) noexcept
    {
        value %= rhs.value;
        return *this;
    }

    constexpr time_span time_span::operator+(const time_span& rhs) const noexcept
    {
        return { value + rhs.value };
    }

    constexpr time_span time_span::operator-(const time_span& rhs) const noexcept
    {
        return { value - rhs.value };
    }

    constexpr time_span time_span::operator*(const time_span& rhs) const noexcept
    {
        return { value * rhs.value };
    }

    constexpr time_span time_span::operator/(const time_span& rhs) const noexcept
    {
        return { value / rhs.value };
    }

    constexpr time_span time_span::operator%(const time_span& rhs) const noexcept
    {
        return { value % rhs.value };
    }

    constexpr auto time_span::operator<=>(const time_span& rhs) const noexcept
    {
        return value <=> rhs.value;
    }

    inline namespace literals
    {
        consteval time_span operator""_ns(const size_type count) noexcept
        {
            return time_span::from_nanoseconds(count);
        }

        consteval time_span operator""_us(const size_type count) noexcept
        {
            return time_span::from_microseconds(count);
        }

        consteval time_span operator""_ms(const size_type count) noexcept
        {
            return time_span::from_milliseconds(count);
        }

        consteval time_span operator""_s(const size_type count) noexcept
        {
            return time_span::from_seconds(count);
        }

        consteval time_span operator""_min(const size_type count) noexcept
        {
            return time_span::from_minutes(count);
        }

        consteval time_span operator""_hr(const size_type count) noexcept
        {
            return time_span::from_hours(count);
        }

        consteval time_span operator""_fps(const size_type count) noexcept
        {
            return { static_cast<int64>(1_g / count) };
        }
    }
} // namespace rsl
