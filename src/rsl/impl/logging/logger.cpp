#include "logger.hpp"

#include "time/stopwatch.hpp"
#include "time/time_point.hpp"

#include "severity.hpp"

#include "logging.hpp"
#include "containers/string.hpp"
#include "threading/current_thread.hpp"

#include "message.hpp"

namespace rsl::log
{
	void logger::log_args(const log::severity s, const format_string format, const bool appendNewLine, const fmt::format_args args) noexcept
	{
		const log::message logMessage
		{
			.loggerName = m_name,
			.threadId = current_thread::get_id(),
			.timestamp = time::main_clock.current_point(),
			.sourceLocation = format.srcLoc,
			.severity = s,
			.msg = format.str,
			.formatArgs = args,
		    .appendNewLine = appendNewLine,
		};

		log(logMessage);
	}

	void logger::flush()
	{
		for (auto* sink : m_sinks)
		{
			sink->flush();
		}
	}

	void synchronous_logger::log(const log::message& message)
	{
		const bool logEnabled = message.severity >= m_severity && message.severity != log::severity::off;
		if (!logEnabled)
		{
			return;
		}

	    if(!m_formatter.is_armed())
	    {
	        set_formatter<undecorated_formatter>();
	    }

	    formatter& formatter = *m_formatter;
		for (auto* sink : m_sinks)
		{
			sink->log(formatter, message);
		}

		if (message.severity >= m_flushSeverity)
		{
			flush();
		}
	}
} // namespace rsl::log
