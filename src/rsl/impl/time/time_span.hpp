#pragma once
#include <chrono>

#include "../util/concepts.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
    namespace tm
    {
        template <typename Rep>
        concept duration_rep = !is_specialization_v<Rep, std::chrono::duration>;

        template <duration_rep Precision = nano_seconds>
        using nano_seconds_duration = std::chrono::duration<Precision, std::nano>;

        template <duration_rep Precision = micro_seconds>
        using micro_seconds_duration = std::chrono::duration<Precision, std::micro>;

        template <duration_rep Precision = milli_seconds>
        using milli_seconds_duration = std::chrono::duration<Precision, std::milli>;

        template <duration_rep Precision = seconds>
        using seconds_duration = std::chrono::duration<Precision, std::ratio<1>>;

        template <duration_rep Precision = minutes>
        using minutes_duration = std::chrono::duration<Precision, std::ratio<60>>;

        template <duration_rep Precision = hours>
        using hours_duration = std::chrono::duration<Precision, std::ratio_multiply<std::ratio<60>, std::chrono::minutes::period>>;

        template <duration_rep Precision = days>
        using days_duration = std::chrono::duration<Precision, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

        template <duration_rep Precision = time32>
        struct span
        {
            using time_type = Precision;
            using duration_type = std::chrono::duration<time_type>;

            duration_type duration{ duration_type::zero() };

            constexpr span() noexcept = default;

            constexpr span(time_type other) noexcept
                : duration(other)
            {}

            template <duration_rep OtherTime, ratio_type Ratio>
            constexpr explicit span(const std::chrono::duration<OtherTime, Ratio>& other) noexcept
                : duration(std::chrono::duration_cast<duration_type>(other))
            {}

            template <duration_rep OtherTime>
                requires(!std::same_as<OtherTime, time_type>)
            constexpr explicit span(const span<OtherTime>& other) noexcept
                : duration(other.duration)
            {}

            template <duration_rep OtherTime, ratio_type Ratio>
            constexpr explicit span(std::chrono::duration<OtherTime, Ratio>&& other) noexcept
                : duration(std::chrono::duration_cast<duration_type>(other))
            {}

            template <duration_rep OtherTime>
                requires(!std::same_as<OtherTime, time_type>)
            constexpr explicit span(span<OtherTime>&& other) noexcept
                : duration(std::move(other.duration))
            {}

            template <duration_rep OtherTime>
                requires(!std::same_as<OtherTime, time_type>)
            constexpr explicit span(OtherTime other) noexcept
                : duration(other)
            {}

            template <duration_rep OtherTime>
            constexpr span<OtherTime> cast() const noexcept
            {
                return { *this };
            }

            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T hours() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<hours_duration<T>>(duration).count();
            }
            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T minutes() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<minutes_duration<T>>(duration).count();
            }
            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T seconds() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<seconds_duration<T>>(duration).count();
            }
            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T milliseconds() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<milli_seconds_duration<T>>(duration).count();
            }
            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T microseconds() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<micro_seconds_duration<T>>(duration).count();
            }
            template <duration_rep T>
            [[nodiscard]] [[rythe_always_inline]] constexpr T nanoseconds() const
                    noexcept(rsl::is_arithmetic_v<time_type> && rsl::is_arithmetic_v<T>)
            {
                return std::chrono::duration_cast<nano_seconds_duration<T>>(duration).count();
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr time_type hours() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return std::chrono::duration_cast<hours_duration<time_type>>(duration).count();
            }
            [[nodiscard]] [[rythe_always_inline]] constexpr time_type minutes() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return std::chrono::duration_cast<minutes_duration<time_type>>(duration).count();
            }
            [[nodiscard]] [[rythe_always_inline]] constexpr time_type seconds() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return duration.count();
            }
            [[nodiscard]] [[rythe_always_inline]] constexpr time_type milliseconds() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return std::chrono::duration_cast<milli_seconds_duration<time_type>>(duration).count();
            }
            [[nodiscard]] [[rythe_always_inline]] constexpr time_type microseconds() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return std::chrono::duration_cast<micro_seconds_duration<time_type>>(duration).count();
            }
            [[nodiscard]] [[rythe_always_inline]] constexpr time_type nanoseconds() const noexcept(rsl::is_arithmetic_v<time_type>)
            {
                return std::chrono::duration_cast<nano_seconds_duration<time_type>>(duration).count();
            }

            [[nodiscard]] [[rythe_always_inline]] constexpr operator duration_type&() noexcept { return duration; }
            [[nodiscard]] [[rythe_always_inline]] constexpr operator const duration_type&() const noexcept { return duration; }
            [[nodiscard]] [[rythe_always_inline]] constexpr operator time_type() const noexcept { return duration.count(); }

            [[rythe_always_inline]] constexpr span& operator++() noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                ++duration;
                return *this;
            }
            [[rythe_always_inline]] constexpr span operator++(int) noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                return span(duration++);
            }
            [[rythe_always_inline]] constexpr span& operator--() noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                --duration;
                return *this;
            }
            [[rythe_always_inline]] constexpr span operator--(int) noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                return span(duration--);
            }

            template <duration_rep OtherTime>
            [[rythe_always_inline]] constexpr span& operator+=(const span<OtherTime>& rhs)
                    noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                duration += std::chrono::duration_cast<duration_type>(rhs.duration);
                return *this;
            }

            template <duration_rep OtherTime>
            [[rythe_always_inline]] constexpr span& operator-=(const span<OtherTime>& rhs)
                    noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                duration -= std::chrono::duration_cast<duration_type>(rhs.duration);
                return *this;
            }

            template <duration_rep OtherTime>
            [[rythe_always_inline]] constexpr span& operator*=(const span<OtherTime>& rhs)
                    noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                duration *= std::chrono::duration_cast<duration_type>(rhs.duration);
                return *this;
            }

            template <duration_rep OtherTime>
            [[rythe_always_inline]] constexpr span& operator/=(const span<OtherTime>& rhs)
                    noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                duration /= std::chrono::duration_cast<duration_type>(rhs.duration);
                return *this;
            }

            template <duration_rep OtherTime>
            [[rythe_always_inline]] constexpr span& operator%=(const span<OtherTime>& rhs)
                    noexcept(rsl::is_arithmetic_v<time_type>) /* strengthened */
            {
                duration %= std::chrono::duration_cast<duration_type>(rhs.duration);
                return *this;
            }

            [[nodiscard]] [[rythe_always_inline]] static constexpr span zero() noexcept { return span(0); }
            [[nodiscard]] [[rythe_always_inline]] static constexpr span min() noexcept
            {
                return span(std::chrono::duration_values<time_type>::min());
            }
            [[nodiscard]] [[rythe_always_inline]] static constexpr span max() noexcept
            {
                return span(std::chrono::duration_values<time_type>::max());
            }
        };

        template <duration_rep PrecisionLHS, duration_rep PrecisionRHS>
        using common_span = tm::span<std::common_type_t<PrecisionLHS, PrecisionRHS>>;

        using span32 = span<time32>;
        using span64 = span<time64>;
    } // namespace tm

    [[nodiscard]] [[rythe_always_inline]] consteval tm::span32 operator""_ns(const size_type count) noexcept
    {
        return tm::span32(tm::nano_seconds_duration<size_type>(count));
    }

    [[nodiscard]] [[rythe_always_inline]] consteval tm::span32 operator""_us(const size_type count) noexcept
    {
        return tm::span32(tm::micro_seconds_duration<size_type>(count));
    }

    [[nodiscard]] [[rythe_always_inline]] consteval tm::span32 operator""_ms(const size_type count) noexcept
    {
        return tm::span32(tm::milli_seconds_duration<size_type>(count));
    }

    [[nodiscard]] [[rythe_always_inline]] consteval tm::span32 operator""_s(const size_type count) noexcept
    {
        return tm::span32(tm::seconds_duration<size_type>(count));
    }

} // namespace rsl

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator+(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return rsl::tm::common_span<PrecisionLHS, PrecisionRHS>(lhs.duration + rhs.duration);
}

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator-(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return rsl::tm::common_span<PrecisionLHS, PrecisionRHS>(lhs.duration - rhs.duration);
}

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator*(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return rsl::tm::common_span<PrecisionLHS, PrecisionRHS>(lhs.duration * rhs.duration);
}

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator/(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return rsl::tm::common_span<PrecisionLHS, PrecisionRHS>(lhs.duration / rhs.duration);
}

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator%(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return rsl::tm::common_span<PrecisionLHS, PrecisionRHS>(lhs.duration % rhs.duration);
}

template <rsl::tm::duration_rep PrecisionLHS, rsl::tm::duration_rep PrecisionRHS>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator<=>(const rsl::tm::span<PrecisionLHS>& lhs, const rsl::tm::span<PrecisionRHS>& rhs)
	noexcept(rsl::is_arithmetic_v<PrecisionLHS> && rsl::is_arithmetic_v<PrecisionRHS>) /* strengthened */
{
	return lhs.duration.count() <=> rhs.duration.count();
}
