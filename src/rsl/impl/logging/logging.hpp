#pragma once

#include <sstream>
#include <thread>

#include "../util/primitives.hpp"
#include "../containers/map/dynamic_map.hpp"

#include "sink.hpp"
#include "logger.hpp"

namespace rsl
{
	using logger_ptr = std::shared_ptr<spdlog::logger>;

	struct logging_context
	{
		rsl::cstring logFile = "logs/rythe.log";
		logger_ptr logger;
		logger_ptr fileLogger;
		logger_ptr consoleLogger = spdlog::stdout_color_mt("console-logger");
		logger_ptr undecoratedLogger = spdlog::stdout_color_mt("undecorated-logger");

		static logging_context& get()
		{
			static logging_context m_instance;
			return m_instance;
		}
	};


	[[rythe_always_inline]] static void
	initLogger([[maybe_unused]] logger_ptr& logger, [[maybe_unused]] std::string_view loggerName = "")
	{
		// auto f = std::make_unique<spdlog::pattern_formatter>();
		//
		// f->add_flag<log::thread_name_formatter_flag>('f');
		// f->add_flag<log::genesis_formatter_flag>('*');
		//
		// f->set_pattern("T+ %* [%^%=7l%$] [%=13!f] : %v");
		//
		// logger->set_formatter(std::move(f));
	}

	[[maybe_unused]] [[rythe_always_inline]] static void setDefaultLogger(const logger_ptr& newLogger)
	{
		logging_context::get().logger = newLogger;
	}

	namespace log
	{
		/** @brief prints a log line, using the specified `severity`
		 *  @param s The severity you wan't to report this log with
		 *  @param format The format string you want to print
		 *  @param a The arguments to the format string
		 *  @note This uses fmt lib style syntax check
		 *         https://fmt.dev/latest/syntax.html
		 */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void println([[maybe_unused]] severity s, [[maybe_unused]] const FormatString& format, [[maybe_unused]] Args&&... a)
		{
			//logging_context::get().logger->log(internal::rythe_to_spdlog(s), format, std::forward<Args>(a)...);
		}

		/** @brief same as println but uses the undecorated logger */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedln([[maybe_unused]] severity s, [[maybe_unused]] const FormatString& format, [[maybe_unused]] Args&&... a)
		{
			// I need to get back to this
			//logging_context::get().undecoratedLogger->log(internal::rythe_to_spdlog(s), format, std::forward<Args>(a)...);
		}

		/** @brief prints a log line, using the specified `severity`
		 *  @param level selects the severity level you are interested in
		 */
		[[rythe_always_inline]] static void filter(const severity level)
		{
			auto& inst = logging_context::get();
			inst.logger->set_level(internal::rythe_to_spdlog(level));
			inst.undecoratedLogger->set_level(internal::rythe_to_spdlog(level));
		}

		/** @brief same as println but with severity = trace */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedTrace(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::trace, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = debug */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedDebug(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::debug, format, std::forward<Args>(a)...);
		}

		/** @brief same as undecoratedln but with severity = info */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedInfo(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::info, format, std::forward<Args>(a)...);
		}

		/** @brief same as undecoratedln but with severity = warn */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedWarn(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::warn, format, std::forward<Args>(a)...);
		}

		/** @brief same as undecoratedln but with severity = error */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedError(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::error, format, std::forward<Args>(a)...);
		}

		/** @brief same as undecoratedln but with severity = fatal */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void undecoratedFatal(const FormatString& format, Args&&... a)
		{
			undecoratedln(severity::fatal, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = trace */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void trace(const FormatString& format, Args&&... a)
		{
			println(severity::trace, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = debug */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void debug(const FormatString& format, Args&&... a)
		{
			println(severity::debug, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = info */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void info(const FormatString& format, Args&&... a)
		{
			println(severity::info, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = warn */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void warn(const FormatString& format, Args&&... a)
		{
			println(severity::warn, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = error */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void error(const FormatString& format, Args&&... a)
		{
			println(severity::error, format, std::forward<Args>(a)...);
		}

		/** @brief same as println but with severity = fatal */
		template <class... Args, class FormatString>
		[[rythe_always_inline]] void fatal(const FormatString& format, Args&&... a)
		{
			println(severity::fatal, format, std::forward<Args>(a)...);
		}

		namespace internal
		{
			inline void setup()
			{
				auto& inst = logging_context::get();

				auto f = std::make_unique<spdlog::pattern_formatter>();
				f->set_pattern("%^%v%$");
				inst.undecoratedLogger->set_formatter(std::move(f));

				inst.fileLogger = spdlog::rotating_logger_mt(inst.logFile, inst.logFile, 1'048'576, 5);
				initLogger(inst.consoleLogger);
				initLogger(inst.fileLogger);

				#if defined(RYTHE_KEEP_CONSOLE) || defined(RYTHE_DEBUG)
				inst.logger = inst.consoleLogger;
				#else
				inst.logger = inst.fileLogger;
				#endif

				#if defined(RYTHE_LOG_TRACE)
				filter(severity::trace);
				#elif defined(RYTHE_LOG_DEBUG)
				filter(severity::debug);
				#elif defined(RYTHE_LOG_INFO)
				filter(severity::info);
				#elif defined(RYTHE_LOG_WARN)
				filter(severity::warn);
				#elif defined(RYTHE_LOG_ERROR)
				filter(severity::error);
				#elif defined(RYTHE_LOG_FATAL)
				filter(severity::fatal);
				#elif defined(RYTHE_DEBUG)
				filter(severity::debug);
				#else
				filter(severity::info);
				#endif

				undecoratedInfo("== Initializing Logger");
			}
		} // namespace internal
	} // namespace log
} // namespace rsl
#undef logger
