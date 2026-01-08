#pragma once

#include "current_thread.hpp"

namespace rsl::current_thread
{
	template <tm::duration_rep Precision>
	void sleep_for(tm::span<Precision> duration)
	{
		platform::sleep_current_thread(duration.template milliseconds<uint32>());
	}

	template <tm::duration_rep Precision, tm::clock_type ClockType>
	void sleep_until(tm::point<Precision, ClockType> timepoint)
	{
		sleep_for(timepoint - tm::stopwatch<Precision, ClockType>::current_point());
	}
}
