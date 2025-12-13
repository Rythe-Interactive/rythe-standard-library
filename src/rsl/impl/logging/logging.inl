#pragma once
namespace rsl::log
{
    template <typename... Args>
    void println(const severity s, const format_string format, Args&&... args) noexcept
    {
        get_logging_context().logger->logln(s, format, rsl::forward<Args>(args)...);
    }

    template <typename... Args>
    void undecoratedln(const severity s, const format_string format, Args&&... args) noexcept
    {
        get_logging_context().undecoratedLogger->logln(s, format, rsl::forward<Args>(args)...);
    }

    inline void filter(const severity level)
    {
        logging_context& context = get_logging_context();
        context.logger->filter(level);
        context.undecoratedLogger->filter(level);
    }

    template <typename... Args>
    void trace(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_TRACE_LOG)
        println(severity::trace, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void debug(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_DEBUG_LOG)
        println(severity::debug, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void info(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_INFO_LOG)
        println(severity::info, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void warn(const format_string format, Args&&... args)
    {
        println(severity::warn, format, rsl::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(const format_string format, Args&&... args)
    {
        println(severity::error, format, rsl::forward<Args>(args)...);
        #if !defined(RSL_NO_LOGGING_DEBUGBREAK)
        if (platform::is_debugger_attached())
        {
            rythe_debugbreak_instruction();
        }
        #endif
    }

    template <typename... Args>
    void fatal(const format_string format, Args&&... args)
    {
        println(severity::fatal, format, rsl::forward<Args>(args)...);
        #if !defined(RSL_NO_LOGGING_DEBUGBREAK)
        if (platform::is_debugger_attached())
        {
            rythe_debugbreak_instruction();
        }
        #endif
    }

    template <typename... Args>
    void undecorated_trace(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_TRACE_LOG)
        undecoratedln(severity::trace, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void undecorated_debug(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_DEBUG_LOG)
        undecoratedln(severity::debug, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void undecorated_info(const format_string format, Args&&... args)
    {
        #if !defined(RSL_STRIP_INFO_LOG)
        undecoratedln(severity::info, format, rsl::forward<Args>(args)...);
        #endif
    }

    template <typename... Args>
    void undecorated_warn(const format_string format, Args&&... args)
    {
        undecoratedln(severity::warn, format, rsl::forward<Args>(args)...);
    }

    template <typename... Args>
    void undecorated_error(const format_string format, Args&&... args)
    {
        undecoratedln(severity::error, format, rsl::forward<Args>(args)...);
        #if !defined(RSL_NO_LOGGING_DEBUGBREAK)
        if (platform::is_debugger_attached())
        {
            rythe_debugbreak_instruction();
        }
        #endif
    }

    template <typename... Args>
    void undecorated_fatal(const format_string format, Args&&... args)
    {
        undecoratedln(severity::fatal, format, rsl::forward<Args>(args)...);
        #if !defined(RSL_NO_LOGGING_DEBUGBREAK)
        if (platform::is_debugger_attached())
        {
            rythe_debugbreak_instruction();
        }
        #endif
    }
}
