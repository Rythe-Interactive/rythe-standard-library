#pragma once
#include <chrono>

#include "../defines.hpp"
#include "../util/primitives.hpp"

#include "time_point.hpp"


namespace rsl::tm
{
    // TODO(Glyn): stop relying on chrono and actually implement a clock.
	template <duration_rep Precision = time32, clock_type ClockType = std::chrono::high_resolution_clock>
	struct stopwatch
	{
	public:
		using time_type = Precision;
		using span_type = tm::span<time_type>;
		using time_point_type = tm::point<Precision, ClockType>;
		using clock_type = ClockType;

	private:
		std::chrono::time_point<clock_type> m_start{clock_type::now()};

	public:
		[[rythe_always_inline]] void start() noexcept { m_start = clock_type::now(); }
		[[rythe_always_inline]] span_type end() const noexcept { return span_type(clock_type::now() - m_start); }

		[[rythe_always_inline]] span_type restart() noexcept
		{
			auto startTime = clock_type::now();
			span_type time(startTime - m_start);
			m_start = startTime;
			return time;
		}

		[[rythe_always_inline]] time_point_type start_point() const noexcept
		{
			return time_point_type{m_start.time_since_epoch()};
		}
		[[rythe_always_inline]] static time_point_type current_point() noexcept
		{
			return time_point_type{clock_type::now()};
		}
		[[rythe_always_inline]] span_type elapsed_time() const noexcept
		{
			return span_type{clock_type::now() - m_start};
		}

		[[rythe_always_inline]] void fast_forward(span_type time) noexcept(std::is_arithmetic_v<time_type>)
		{
			m_start -= std::chrono::duration_cast<typename clock_type::duration>(time.duration);
		}
		[[rythe_always_inline]] void rewind(span_type time) noexcept(std::is_arithmetic_v<time_type>)
		{
			m_start += std::chrono::duration_cast<typename clock_type::duration>(time.duration);
		}
	};

	using timer32 = stopwatch<time32>;
	using timer64 = stopwatch<time64>;

	extern const timer32 main_clock;
} // namespace rsl::time
