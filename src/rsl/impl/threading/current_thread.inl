#pragma once

#include "current_thread.hpp"

namespace rsl::current_thread
{
	template <time::duration_rep Precision>
	void sleep_for(time::span<Precision> duration)
	{
		platform::sleep_current_thread(duration.template milliseconds<uint32>());
	}

	template <time::duration_rep Precision, time::clock_type ClockType>
	void sleep_until(time::point<Precision, ClockType> timepoint)
	{
		sleep_for(timepoint - time::stopwatch<Precision, ClockType>::current_point());
	}
}
