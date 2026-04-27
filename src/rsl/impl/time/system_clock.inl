#pragma once

namespace rsl
{
    inline time_span system_clock::start_time() const noexcept
    {
        return m_start;
    }

    inline time_span system_clock::elapsed_time() const noexcept
    {
        return current_time() - m_start;
    }

	inline time_span system_clock::restart() noexcept
    {
        time_span startTime = current_time();
        time_span time(startTime - m_start);
        m_start = startTime;
        return time;
    }

    inline void system_clock::fast_forward(time_span time) noexcept
    {
        m_start -= time;
    }

    inline void system_clock::rewind(time_span time) noexcept
    {
        m_start += time;
    }
} // namespace rsl
