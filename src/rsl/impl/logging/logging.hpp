#pragma once
#include "../platform/platform.hpp"

#include "logger.hpp"
#include "sink.hpp"

namespace rsl
{
    struct logging_context
    {
        dynamic_string logFile = "logs/rythe.log"_ds;
        pointer<log::logger> logger = { nullptr };
        pointer<log::logger> undecoratedLogger = { nullptr };
        size_type indent;
        char indentChar = ' ';
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

        [[rythe_always_inline]] inline void set_indent_char(char indentChar);
        [[rythe_always_inline]] inline void indent(size_type amount = 1ull);
        [[rythe_always_inline]] inline void unindent(size_type amount = 1ull);

        struct indent_scope
        {
            [[rythe_always_inline]] indent_scope(size_type amount = 1ull)
                : m_amount(amount)
            {
                indent(m_amount);
            }
            [[rythe_always_inline]] ~indent_scope() { unindent(m_amount); }

        private:
            size_type m_amount;
        };

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
