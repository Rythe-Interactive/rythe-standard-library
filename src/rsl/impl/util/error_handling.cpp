#include "error_handling.hpp"
#include "../../logging"

namespace rsl
{
    void error_handler::handle_assert(
            const string_view expression,
            const string_view file,
            const size_type line,
            const string_view message,
            const bool soft,
            bool* ignore
            )
    {
        asserts::internal::default_assert_handler(expression, file, line, message, soft, ignore);
    }

	void error_handler::handle_error(const error_type& error, const bool assertError)
	{
		switch (error.severity)
		{
			case error_severity::warning:
			{
				if (assertError)
				{
					rsl_assert_msg_soft_consistent(false, error.message);
				}
				else
				{
					log::warn("{}: {}", error.code, error.message);
				}
				break;
			}
			case error_severity::error:
			{
				if (assertError)
				{
					rsl_assert_msg_hard(false, error.message);
				}
				else
				{
					log::error("{}: {}", error.code, error.message);
				}
				break;
			}
			case error_severity::fatal:
			{
				if (assertError)
				{
					rsl_assert_msg_hard(false, error.message);
				}
				else
				{
					log::fatal("{}: {}", error.code, error.message);
				}
				break;
			}
		}
	}

	namespace internal
	{
		errid& get_errid(result_base& r) noexcept
		{
			return r.m_errid;
		}

        namespace
        {
		    error_handler* get_default_error_handler() noexcept
		    {
		        static error_handler defaultErrorHandler;
		        return &defaultErrorHandler;
		    }
        }

        error_context& get_default_error_context() noexcept
        {
            thread_local error_context context;
            return context;
        }
	} // namespace internal

    get_error_context_func get_error_context = &internal::get_default_error_context;

	void enable_assert_on_error(const bool enabled) noexcept
	{
		get_error_context().assertOnError = enabled;
	}

	void disable_assert_on_error() noexcept
	{
		enable_assert_on_error(false);
	}

	bool assert_on_error_enabled() noexcept
	{
		return get_error_context().assertOnError;
	}

	scoped_assert_on_error::scoped_assert_on_error(const bool enabled)
		: wasEnabled(assert_on_error_enabled())
	{
		enable_assert_on_error(enabled);
	}

	scoped_assert_on_error::~scoped_assert_on_error()
	{
		enable_assert_on_error(wasEnabled);
	}

	namespace
	{
		void error_handler_based_assert_handler(
			const string_view expression, const string_view file, const size_type line, const string_view message, const bool soft, bool* ignore
		)
		{
			get_error_handler()->handle_assert(expression, file, line, message, soft, ignore);
		}
	} // namespace

	void set_error_handler(error_handler* errorHandler) noexcept
	{
		asserts::assert_handler = &error_handler_based_assert_handler;
		get_error_context().errorHandlerOverride = errorHandler;
	}

	error_handler* get_error_handler() noexcept
	{
		return get_error_context().errorHandlerOverride ? get_error_context().errorHandlerOverride
												   : internal::get_default_error_handler();
	}

	scoped_error_handler::scoped_error_handler(error_handler* errorHandler)
		: previousErrorHandler(get_error_handler())
	{
		set_error_handler(errorHandler);
	}

	scoped_error_handler::~scoped_error_handler()
	{
		set_error_handler(previousErrorHandler);
	}
} // namespace rsl
