#pragma once
#include "../platform/platform.hpp"

#include "logger.hpp"
#include "sink.hpp"

namespace rsl
{
    struct logging_context
    {
        dynamic_string logFile = "logs/rythe.log"_ds;
        log::logger* logger = nullptr;
        log::logger* undecoratedLogger = nullptr;
    };
    RYTHE_DECLARE_SINGLETON(logging_context)

    namespace log
    {
        template <typename... Args>
        [[rythe_always_inline]] void println(severity s, format_string format, Args&&... args) noexcept;
        template <typename... Args>
        [[rythe_always_inline]] void undecoratedln(severity s, format_string format, Args&&... args) noexcept;

        [[rythe_always_inline]] inline void filter(severity level);
        [[rythe_always_inline]] inline void flush_at(severity level);

        template <typename... Args>
        [[rythe_always_inline]] void trace(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void debug(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void info(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void warn(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void error(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void fatal(format_string format, Args&&... args);

        template <typename... Args>
        [[rythe_always_inline]] void undecorated_trace(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void undecorated_debug(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void undecorated_info(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void undecorated_warn(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void undecorated_error(format_string format, Args&&... args);
        template <typename... Args>
        [[rythe_always_inline]] void undecorated_fatal(format_string format, Args&&... args);
    } // namespace log
} // namespace rsl

#include "logging.inl"
