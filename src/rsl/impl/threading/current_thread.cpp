#include "current_thread.hpp"

#include "../platform/platform.hpp"
#include "../time/system_clock.hpp"

namespace rsl::current_thread
{
    void yield()
    {
        platform::yield_current_thread();
    }

    thread_id get_id()
    {
        return platform::get_current_thread_id();
    }

    void sleep_for(time_span duration)
    {
        platform::sleep_current_thread(duration.milliseconds<uint32>());
    }

    void sleep_until(time_span timepoint)
    {
        sleep_for(timepoint - main_clock.current_time());
    }

    string_view get_name()
    {
        return platform::get_thread_name(get_id());
    }

    void set_name(string_view name)
    {
        platform::set_thread_name(get_id(), name);
    }
}
