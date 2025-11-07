#include "logging.hpp"

#include "formatter.hpp"
#include "sinks/console_sink.hpp"
#include "sinks/file_sink.hpp"

namespace rsl
{
    namespace internal
    {
        namespace
        {
            [[nodiscard]] log::file_sink create_file_sink(const logging_context& context) noexcept
            {
                log::file_sink sink;
                sink.set_file_name(context.logFile);
                sink.set_max_file_count(5ull);
                return sink;
            }

            [[nodiscard]] logging_context default_setup() noexcept
            {
                logging_context context;

                static log::console_sink defaultConsoleSink{};
                static log::file_sink defaultFileSink = create_file_sink(context);

                thread_local log::default_logger undecoratedLogger("default undecorated logger");
                undecoratedLogger.set_sinks(&defaultConsoleSink, &defaultFileSink);
                context.undecoratedLogger = &undecoratedLogger;

                thread_local log::default_logger logger("default logger");
                logger.set_sinks(&defaultConsoleSink, &defaultFileSink);
                logger.set_formatter<log::pattern_formatter>(
                        "[{}] [{}] [{}] [{}] : {}",
                        log::genesis_flag_formatter{},
                        log::thread_name_flag_formatter{},
                        log::logger_name_flag_formatter{},
                        log::severity_flag_formatter{},
                        log::message_flag_formatter{}
                        );

                context.logger = &logger;

                undecoratedLogger.log(log::severity::info, "== Initializing Logging =="_sv);
                return context;
            }
        }

        logging_context& get_default_logging_context() noexcept
        {
            thread_local logging_context context = default_setup();
            return context;
        }
    }

    get_logging_context_func get_logging_context = &internal::get_default_logging_context;
}
