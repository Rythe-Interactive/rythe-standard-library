#pragma once

namespace rsl::log
{
    inline logger::logger(const string_view name, const log::severity severity, const log::severity flushSeverity)
        : m_name(dynamic_string::from_view(name)), m_severity(severity), m_flushSeverity(flushSeverity) {}

    inline void logger::set_formatter(temporary_object<formatter>&& sinkFormatter)
    {
        m_formatter = rsl::move(sinkFormatter);
    }

    template <derived_from<formatter> FormatterType, typename... Args>
    void logger::set_formatter(Args&&... args)
    {
        m_formatter = unique_object<FormatterType>::create_in_place(rsl::forward<Args>(args)...);
    }

    template <derived_from<sink>... SinkTypes>
    void logger::set_sinks(SinkTypes*... sinks)
    {
        m_sinks = dynamic_array<sink*>::from_variadic_items(sinks...);
    }

    inline void logger::set_sinks(const array_view<sink*> sinks)
    {
        m_sinks = dynamic_array<sink*>::from_view(sinks);
    }

    inline array_view<sink* const> logger::view_sinks() const noexcept { return m_sinks.view(); }

    template <typename... Args>
    constexpr void logger::log(const log::severity s, const format_string format, Args&&... args) noexcept
    {
        // Specifically no forward to force temp object lifetime to this body, and to force an address for all values.
        log_args(s, format, false, fmt::make_format_args(args...));
    }

    template <typename ... Args>
    constexpr void logger::logln(const log::severity s, const format_string format, Args&&... args) noexcept
    {
        // Specifically no forward to force temp object lifetime to this body, and to force an address for all values.
        log_args(s, format, true, fmt::make_format_args(args...));
    }

    inline void logger::filter(const severity s) noexcept
    {
        m_severity = s;
    }

    inline severity logger::filter_severity() const noexcept
    {
        return m_severity;
    }

    inline void logger::flush_at(const severity s) noexcept
    {
        m_flushSeverity = s;
    }

    inline severity logger::flush_severity() const noexcept
    {
        return m_flushSeverity;
    }
} // namespace rsl::log
