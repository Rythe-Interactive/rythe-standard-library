#pragma once

#include "../time/stopwatch.hpp"

#include "thread_id.hpp"
#include "../platform/platform.hpp"

namespace rsl::current_thread
{
    void yield();
    thread_id get_id();

	template <tm::duration_rep Precision = time32>
	void sleep_for(tm::span<Precision> duration);

	template <tm::duration_rep Precision = time32, tm::clock_type ClockType = tm::timer32::clock_type>
	void sleep_until(tm::point<Precision, ClockType> timepoint);

	string_view get_name();
	void set_name(string_view name);
}

#include "current_thread.inl"
