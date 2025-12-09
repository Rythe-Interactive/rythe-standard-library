#pragma once

#include "../containers/string.hpp"
#include "../containers/views.hpp"

namespace rsl::log
{
    #if !defined(RSL_DEFAULT_LOG_SEVERITY)
    #if defined(RYTHE_DEBUG)
    #define RSL_DEFAULT_LOG_SEVERITY debug
    #else
    #define RSL_DEFAULT_LOG_SEVERITY info
    #endif
    #endif

    #if !defined(RSL_DEFAULT_LOG_FLUSH_SEVERITY)
    #if defined(RYTHE_DEBUG)
    #define RSL_DEFAULT_LOG_FLUSH_SEVERITY debug
    #else
    #define RSL_DEFAULT_LOG_FLUSH_SEVERITY off
    #endif
    #endif

    enum struct [[rythe_closed_enum]] severity : uint8
    {
        trace = 0,
        // lowest severity
        debug,
        info,
        warn,
        error,
        fatal,
        // highest severity
        off,
        default_severity       = RSL_DEFAULT_LOG_SEVERITY,
        default_flush_severity = RSL_DEFAULT_LOG_FLUSH_SEVERITY,
    };

    [[nodiscard]] [[rythe_always_inline]] constexpr string_view severity_name(const severity severity) noexcept
    {
        switch (severity)
        {
            case rsl::log::severity::trace: return "trace";
            case rsl::log::severity::debug: return "debug";
            case rsl::log::severity::info: return "info";
            case rsl::log::severity::warn: return "warning";
            case rsl::log::severity::error: return "error";
            case rsl::log::severity::fatal: return "fatal";
            case rsl::log::severity::off: return "off";
        }

        rsl_assert_unreachable();
        return "";
    }

    [[nodiscard]] [[rythe_always_inline]] constexpr string_view severity_short_name(const severity severity) noexcept
    {
        switch (severity)
        {
            case rsl::log::severity::trace: return "t";
            case rsl::log::severity::debug: return "d";
            case rsl::log::severity::info: return "i";
            case rsl::log::severity::warn: return "w";
            case rsl::log::severity::error: return "e";
            case rsl::log::severity::fatal: return "f";
            case rsl::log::severity::off: return "off";
        }

        rsl_assert_unreachable();
        return "";
    }
}

template <>
struct fmt::formatter<rsl::log::severity>
{
    static constexpr const char* parse(format_parse_context& ctx)
    {
        return formatter<fmt::string_view>{}.parse(ctx);
    }

    template <typename FormatContext>
    static constexpr auto format(const rsl::log::severity& s, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", rsl::log::severity_name(s));
    }
};
