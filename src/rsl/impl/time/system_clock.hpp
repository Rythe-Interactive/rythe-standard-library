#pragma once
#include "../rsl_core.hpp"

#include "time_span.hpp"


namespace rsl
{
    struct system_clock
    {
    public:
        system_clock(system_clock&&) = default;

        // Current absolute time.
        [[nodiscard]] static time_span current_time() noexcept;

        // Absolute time of the start of the process.
        [[nodiscard]] [[rythe_always_inline]] time_span start_time() const noexcept;

        // Relative time since start of the process.
        [[nodiscard]] [[rythe_always_inline]] time_span elapsed_time() const noexcept;

        [[rythe_always_inline]] time_span restart() noexcept;
        [[rythe_always_inline]] void fast_forward(time_span time) noexcept;
        [[rythe_always_inline]] void rewind(time_span time) noexcept;

        ~system_clock();

    private:
        system_clock() = default;
        friend system_clock initialize_main_clock();
        time_span m_start;
    };

    extern system_clock main_clock;
    extern const time_span genesis;
} // namespace rsl

#include "system_clock.inl"
