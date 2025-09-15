#pragma once

namespace rsl
{
    template <error_code ErrorType>
    constexpr string_view default_error_message(ErrorType)
    {
        rsl_assert_unimplemented();
        return {};
    }

    constexpr error_type::operator errc() const noexcept
    {
        return code;
    }

    constexpr bool error_type::operator==(const error_type& other) const noexcept
    {
        return code == other.code;
    }

    inline result_base::result_base(error_signal) noexcept : m_errid(get_error_context().currentError) {}

    inline bool result_base::is_okay() const noexcept
    {
        return get_error() == success;
    }

    inline const error_type& result_base::get_error() const noexcept
    {
        return m_errid == invalid_err_id ? success : get_error_context().errors[m_errid];
    }

    inline error_view result_base::get_errors() const noexcept
    {
        if (m_errid == invalid_err_id)
        {
            return error_view{};
        }

        error_context& context = get_error_context();

        const errid blockStart = context.errors[m_errid].errorBlockStart;
        return error_view::from_buffer(&context.errors[blockStart], static_cast<size_type>((m_errid - blockStart) + 1));
    }

    inline errid result_base::id() const noexcept
    {
        return m_errid;
    }

    inline void result_base::resolve() noexcept
    {
        error_context& context = get_error_context();

        rsl_assert_msg_hard(!is_okay(), "Tried to resolve a valid result without error.");
        rsl_assert_msg_hard(
                m_errid == context.currentError,
                "Errors not resolved in order. Earlier result remains unresolved."
                );

        const auto& error = context.errors[context.currentError];
        context.errors.reduce((context.currentError - error.errorBlockStart) + 1);
        context.currentError = static_cast<errid>(context.errors.size() - 1);
        m_errid = invalid_err_id;
    }

    inline error_signal result_base::propagate() noexcept
    {
        return error_signal{};
    }

    inline bool result_base::reduce_and_discard() noexcept
    {
        if (is_okay())
        {
            return true;
        }

        resolve();
        return false;
    }

    inline errc result_base::report_errors() noexcept
    {
        error_handler* errorHandler = get_error_handler();

        for (auto& error : get_errors())
        {
            errorHandler->handle_error(error, assert_on_error_enabled());
        }

        errc errorCode = no_error_code;
        if (m_errid != invalid_err_id)
        {
            errorCode = get_error().code;
            resolve();
        }

        return errorCode;
    }

    inline result_base::~result_base()
    {
        #if RYTHE_VALIDATION_LEVEL != RYTHE_VALIDATION_LEVEL_NONE
        report_errors();
        #endif
    }

    template <typename T>
    result<T>::result(error_signal) noexcept
        : result_base(error_signal{}),
          m_dummy(0) {}

    template <typename T>
    template <typename... Args>
        requires (rsl::constructible_from<T, Args...>)
    constexpr result<T>::result(Args&&... args) noexcept(rsl::is_nothrow_constructible_v<T, Args...>)
        : result_base(),
          m_value(rsl::forward<Args>(args)...) {}

    template <typename T>
    bool result<T>::carries_value() const noexcept
    {
        for (const auto& error : get_errors())
        {
            if (error.severity != error_severity::warning)
            {
                return false;
            }
        }
        return true;
    }

    template <typename T>
    typename result<T>::result_type& result<T>::value() noexcept
    {
        rsl_assert_msg_hard(report_errors() == no_error_code, "Tried to get value of result with unresolved error.");
        return m_value;
    }

    template <typename T>
    typename result<T>::result_type* result<T>::operator->() noexcept
    {
        return &value();
    }

    template <typename T>
    typename result<T>::result_type& result<T>::operator*() noexcept
    {
        return value();
    }

    namespace internal
    {
        template <error_code ErrorType>
        [[rythe_always_inline]] constexpr void append_error(
                const errid errorBlockStart,
                const ErrorType errorType,
                const string_view message,
                const error_severity severity
                ) noexcept
        {
            error_context& context = get_error_context();
            rsl_assert_msg_hard(
                    context.errors.size() != RSL_ERR_MAX_COUNT,
                    "Max error count overflow, consider changing RSL_ERR_ID_UNDERLYING to a larger type."
                    );

            context.currentError = static_cast<errid>(context.errors.size());
            context.errors.emplace_back(
                    error_type{
                        .code = static_cast<errc>(errorType),
                        .message = error_message::from_view(message),
                        .severity = severity,
                        .errorBlockStart = errorBlockStart,
                    }
                    );
        }
    } // namespace internal

    template <error_code ErrorType>
    constexpr error_signal make_error(const ErrorType errorType, const string_view message, const error_severity severity) noexcept
    {
        internal::append_error(static_cast<errid>(get_error_context().errors.size()), errorType, message, severity);
        return error_signal{};
    }

    template <error_code ErrorType>
    constexpr error_signal make_error(const ErrorType errorType, const error_severity severity) noexcept
    {
        internal::append_error(
                static_cast<errid>(get_error_context().errors.size()),
                errorType,
                default_error_message(errorType),
                severity
                );
        return error_signal{};
    }

    template <error_code ErrorType>
    constexpr error_signal make_warning(const ErrorType errorType, const string_view message) noexcept
    {
        return make_error(errorType, message, error_severity::warning);
    }

    template <error_code ErrorType>
    constexpr error_signal make_warning(const ErrorType errorType) noexcept
    {
        return make_error(errorType, error_severity::warning);
    }

    template <error_code ErrorType>
    constexpr error_signal make_fatal(const ErrorType errorType, const string_view message) noexcept
    {
        return make_error(errorType, message, error_severity::fatal);
    }

    template <error_code ErrorType>
    constexpr error_signal make_fatal(const ErrorType errorType) noexcept
    {
        return make_error(errorType, error_severity::fatal);
    }

    template <typename T, error_code ErrorType>
    constexpr void append_error(
            result<T>& result,
            const ErrorType errorType,
            const string_view message,
            const error_severity severity
            ) noexcept
    {
        error_context& context = get_error_context();
        errid blockStart = 0;
        if (context.currentError != invalid_err_id)
        {
            blockStart = context.errors[context.currentError].errorBlockStart;
        }
        else if (result.id() != invalid_err_id)
        {
            blockStart = result.id();
        }

        internal::append_error(blockStart, errorType, message, severity);
        internal::get_errid(result) = context.currentError;
    }

    template <typename T, error_code ErrorType>
    constexpr void append_error(
            result<T>& result,
            const ErrorType errorType,
            const error_severity severity
            ) noexcept
    {
        append_error(result, errorType, default_error_message(errorType), severity);
    }

    template <typename T, error_code ErrorType>
    constexpr void append_warning(result<T>& result, const ErrorType errorType, const string_view message) noexcept
    {
        append_error(result, errorType, message, error_severity::warning);
    }

    template <typename T, error_code ErrorType>
    constexpr void append_warning(result<T>& result, const ErrorType errorType) noexcept
    {
        append_error(result, errorType, error_severity::warning);
    }

    template <typename T, error_code ErrorType>
    constexpr void append_fatal(result<T>& result, const ErrorType errorType, const string_view message) noexcept
    {
        append_error(result, errorType, message, error_severity::fatal);
    }

    template <typename T, error_code ErrorType>
    constexpr void append_fatal(result<T>& result, const ErrorType errorType) noexcept
    {
        append_error(result, errorType, error_severity::fatal);
    }

    template <typename T, typename ... Args>
    constexpr result<T> make_result(Args&&... args) noexcept(rsl::is_nothrow_constructible_v<result<T>, Args...>)
    {
        return result<T>(rsl::forward<Args>(args)...);
    }

    template <typename T>
    unspecified_error::operator result<T>()
    {
        return make_error(unspecified_error_code::unspecified, "Unspecified error"_sv);
    }
}
