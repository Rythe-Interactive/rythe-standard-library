#include "thread.hpp"

#include "../platform/platform.hpp"

namespace rsl
{
    thread::operator bool() const
    {
        return platform::is_thread_active(*this);
    }

    thread_id thread::get_id() const
    {
        return platform::get_thread_id(*this);
    }

    uint32 thread::join() const
    {
        return platform::destroy_thread(*this);
    }
}
