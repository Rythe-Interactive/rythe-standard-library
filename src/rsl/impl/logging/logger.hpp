#pragma once

#include "../containers/string.hpp"
#include "../memory/unique_object.hpp"
#include "../util/source_location.hpp"

#include "formatter.hpp"
#include "severity.hpp"

namespace rsl::log
{
	class sink;
	struct message;

	struct format_string
	{
		[[rythe_always_inline]] constexpr format_string(
			const string_view s,
			const source_location loc = source_location::current())
			noexcept : str(s), srcLoc(loc) {}

		string_view str;
		source_location srcLoc;
	};

	class logger
	{
	public:
        MOVE_FUNCS_CONSTEXPR_NOEXCEPT(logger)

		explicit logger(string_view name, log::severity severity = log::severity::default_severity,
		                      log::severity flushSeverity = log::severity::default_flush_severity);

		virtual ~logger() = default;

		template <typename... Args>
		[[rythe_always_inline]] constexpr void log(log::severity s, format_string format, Args&&... args) noexcept;

		void log_args(log::severity s, format_string format, fmt::format_args args) noexcept;

	    void flush();

	    [[rythe_always_inline]] void set_formatter(temporary_object<formatter>&& sinkFormatter);
	    template <derived_from<formatter> FormatterType, typename... Args>
        [[rythe_always_inline]] void set_formatter(Args&&... args);

	    template <derived_from<sink>... SinkTypes>
		[[rythe_always_inline]] void set_sinks(SinkTypes*... sinks);
		[[rythe_always_inline]] void set_sinks(array_view<sink*> sinks);
		[[nodiscard]] [[rythe_always_inline]] array_view<sink* const> view_sinks() const noexcept;

		[[rythe_always_inline]] void filter(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;

		[[rythe_always_inline]] void flush_at(severity s) noexcept;
		[[nodiscard]] [[rythe_always_inline]] severity flush_severity() const noexcept;

	protected:
		virtual void log(const log::message& message) = 0;

		dynamic_string m_name;
		log::severity m_severity;
		log::severity m_flushSeverity;
		dynamic_array<sink*> m_sinks;
	    unique_object<formatter> m_formatter;
	};

	class synchronous_logger final : public logger
	{
	public:
		using logger::logger;
	    using logger::log;

	protected:
		void log(const log::message& message) override;
	};

    #if !defined(RSL_DEFAULT_LOGGER_OVERRIDE)
    using default_logger = synchronous_logger;
    #else
    using default_logger = RSL_DEFAULT_LOGGER_OVERRIDE;
    #endif
} // namespace rsl::log

#include "logger.inl"
