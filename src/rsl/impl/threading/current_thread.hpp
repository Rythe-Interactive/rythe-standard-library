#pragma once

#include "../time/time_span.hpp"

#include "thread_id.hpp"

namespace rsl::current_thread
{
    void yield();
    thread_id get_id();

    void sleep_for(time_span duration);

    void sleep_until(time_span timepoint);

    string_view get_name();
    void set_name(string_view name);
}
