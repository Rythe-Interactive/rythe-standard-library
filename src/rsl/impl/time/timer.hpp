#pragma once
#include "system_clock.hpp"

namespace rsl
{
    struct timer
    {
    public:
        [[rythe_always_inline]] void start() noexcept;
        [[nodiscard]] [[rythe_always_inline]] time_span end() const noexcept;
        [[rythe_always_inline]] time_span restart() noexcept;

        [[nodiscard]] [[rythe_always_inline]] time_span start_time() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] time_span elapsed_time() const noexcept;

        [[rythe_always_inline]] void fast_forward(time_span time) noexcept;
        [[rythe_always_inline]] void rewind(time_span time) noexcept;

    private:
        time_span m_start;
    };
} // namespace rsl

#include "timer.inl"
