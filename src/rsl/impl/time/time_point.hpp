#pragma once
#include <chrono>

#include "../util/concepts.hpp"
#include "../util/primitives.hpp"

#include "time_span.hpp"

namespace rsl::tm
{
	template <typename Clock>
	concept clock_type = requires {
		{ Clock::now() } -> specialization_of<std::chrono::time_point>;
		requires specialization_of<typename Clock::duration, std::chrono::duration>;
	};

    // TODO(Glyn): floating point representation of time sucks,
    // TODO(Glyn): honestly all of the time library here kindah sucks except for maybe the api...
	template <duration_rep Precision = time32, clock_type ClockType = std::chrono::high_resolution_clock>
	struct point
	{ // represents a point in time
	private:
		using rep = typename ClockType::rep;
		using tp_t = typename ClockType::time_point;

		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr days_duration<T> d_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<days_duration<T>>(std::chrono::abs(duration));
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr hours_duration<T> h_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<hours_duration<T>>(std::chrono::abs(duration) - d_dur<time_large>());
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr minutes_duration<T> m_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<minutes_duration<T>>(
				std::chrono::abs(duration) - d_dur<time_large>() - h_dur<time_large>()
			);
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr seconds_duration<T> s_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<seconds_duration<T>>(
				std::chrono::abs(duration) - d_dur<time_large>() - h_dur<time_large>() - m_dur<time_large>()
			);
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr milli_seconds_duration<T> milli_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<milli_seconds_duration<T>>(
				std::chrono::abs(duration) - d_dur<time_large>() - h_dur<time_large>() - m_dur<time_large>() -
				s_dur<time_large>()
			);
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr micro_seconds_duration<T> micro_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<micro_seconds_duration<T>>(
				std::chrono::abs(duration) - d_dur<time_large>() - h_dur<time_large>() - m_dur<time_large>() -
				s_dur<time_large>() - milli_dur<time_large>()
			);
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr nano_seconds_duration<T> nano_dur() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return std::chrono::duration_cast<nano_seconds_duration<T>>(
				std::chrono::abs(duration) - d_dur<time_large>() - h_dur<time_large>() - m_dur<time_large>() -
				s_dur<time_large>() - milli_dur<time_large>() - micro_dur<time_large>()
			);
		}

	public:
		using time_type = Precision;
		using span_type = tm::span<time_type>;
		using duration_type = typename ClockType::duration;
		using clock_type = ClockType;

		duration_type duration{duration_type::zero()}; // duration since the epoch

		constexpr point() = default;

		constexpr explicit point(const duration_type& other) noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
			: duration(other)
		{
		}
		constexpr explicit point(const tp_t& other) noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
			: duration(other.time_since_epoch())
		{
		}

		template <std::convertible_to<duration_type> other_duration>
		constexpr point(const point<ClockType, other_duration>& other)
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<typename other_duration::rep>) /* strengthened */
			: duration(other.duration)
		{
		}

		[[nodiscard]] constexpr span_type time_since_epoch() const
			noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			return span_type{duration};
		}

		[[rythe_always_inline]] constexpr point& operator++() noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			++duration;
			return *this;
		}
		[[rythe_always_inline]] constexpr point operator++(int) noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			return point{duration++};
		}
		[[rythe_always_inline]] constexpr point& operator--() noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			--duration;
			return *this;
		}
		[[rythe_always_inline]] constexpr point operator--(int) noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			return point{duration--};
		}

		[[rythe_always_inline]] constexpr point& operator+=(const duration_type& _Dur)
			noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			duration += _Dur;
			return *this;
		}
		[[rythe_always_inline]] constexpr point& operator-=(const duration_type& _Dur)
			noexcept(rsl::is_arithmetic_v<rep>) /* strengthened */
		{
			duration -= _Dur;
			return *this;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr operator tp_t() const noexcept { return tp_t{duration}; }

		[[nodiscard]] [[rythe_always_inline]] static constexpr point min() noexcept
		{
			return point{duration_type::min()};
		}
		[[nodiscard]] [[rythe_always_inline]] static constexpr point max() noexcept
		{
			return point{duration_type::max()};
		}

		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T hours() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return h_dur<T>().count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T minutes() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return m_dur<T>().count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T seconds() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return s_dur<T>().count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T milliseconds() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return milli_dur<T>().count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T microseconds() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return micro_dur<T>().count();
		}
		template <duration_rep T>
		[[nodiscard]] [[rythe_always_inline]] constexpr T nanoseconds() const
			noexcept(rsl::is_arithmetic_v<rep> && rsl::is_arithmetic_v<T>)
		{
			return nano_dur<T>().count();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::hours hours() const noexcept(rsl::is_arithmetic_v<rep>)
		{
			return h_dur<rsl::hours>().count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::minutes minutes() const noexcept(rsl::is_arithmetic_v<rep>)
		{
			return m_dur<rsl::minutes>().count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::seconds seconds() const noexcept(rsl::is_arithmetic_v<rep>)
		{
			return s_dur<rsl::seconds>().count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::milli_seconds milliseconds() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return milli_dur<rsl::milli_seconds>().count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::micro_seconds microseconds() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return micro_dur<rsl::micro_seconds>().count();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr rsl::nano_seconds nanoseconds() const
			noexcept(rsl::is_arithmetic_v<rep>)
		{
			return nano_dur<rsl::nano_seconds>().count();
		}
	};

	using point32 = point<time32>;
	using point64 = point<time64>;

	extern const point32 genesis;
} // namespace rsl::tm

template <rsl::tm::duration_rep precision, rsl::tm::clock_type clock_t, rsl::tm::duration_rep precision2>
[[nodiscard]] [[rythe_always_inline]] constexpr auto
operator<=>(const rsl::tm::point<precision, clock_t>& lhs, const rsl::tm::point<precision2, clock_t>& rhs)
	noexcept(rsl::is_arithmetic_v<typename clock_t::rep>) /* strengthened */
{
	return lhs.duration <=> rhs.duration;
}

template <rsl::tm::duration_rep precision, rsl::tm::clock_type clock_t, rsl::tm::duration_rep precision2>
[[nodiscard]] [[rythe_always_inline]] constexpr rsl::tm::common_span<precision, precision2>
operator-(const rsl::tm::point<precision, clock_t>& lhs, const rsl::tm::point<precision2, clock_t>& rhs)
	noexcept(rsl::is_arithmetic_v<typename clock_t::rep>) /* strengthened */
{
	return rsl::tm::common_span<precision, precision2>{lhs.duration - rhs.duration};
}
