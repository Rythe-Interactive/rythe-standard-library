#pragma once
#include "timer.hpp"

namespace rsl
{
    inline void timer::start() noexcept
    {
        m_start = main_clock.elapsed_time();
    }

    inline time_span timer::end() const noexcept
    {
        return elapsed_time();
    }

    inline time_span timer::restart() noexcept
    {
        time_span elapsedTime = end();
        start();
        return elapsedTime;
    }

    inline time_span timer::start_time() const noexcept
    {
        return m_start;
    }

    inline time_span timer::elapsed_time() const noexcept
    {
        return (main_clock.elapsed_time() - m_start);
    }

    inline void timer::fast_forward(time_span time) noexcept
    {
        m_start -= time;
    }

    inline void timer::rewind(time_span time) noexcept
    {
        m_start += time;
    }
}
