#pragma once

#include "../defines.hpp"
#include "../containers/array.hpp"
#include "../containers/string.hpp"
#include "../math/util/limits.hpp"
#include "../util/assert.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
    #if !defined(RSL_ERR_MESSAGE_MAX_LENGTH)
    #define RSL_ERR_MESSAGE_MAX_LENGTH 512
    #endif

    #if !defined(RSL_WARN_MAX_COUNT)
    #define RSL_WARN_MAX_COUNT 16
    #endif

    #if !defined(RSL_ERR_UNDERLYING)
    #define RSL_ERR_UNDERLYING int64
    #endif

    #if !defined(RSL_ERR_ID_UNDERLYING)
    #define RSL_ERR_ID_UNDERLYING uint8
    #endif

    #if defined(RSL_ERR_MAX_COUNT)
    RYTHE_COMPILER_ERROR ("RSL_ERR_MAX_COUNT cannot be user defined, use RSL_ERR_ID_UNDERLYING instead")
    #endif

    using errid = RSL_ERR_ID_UNDERLYING;

    #define RSL_ERR_MAX_COUNT rsl::math::limits<errid>::max

    constexpr errid invalid_err_id = RSL_ERR_MAX_COUNT;

    using errc = RSL_ERR_UNDERLYING;
    constexpr errc no_error_code = 0;

    template <typename ErrC>
    concept error_code = is_enum_v<ErrC> && is_same_v<std::underlying_type_t<ErrC>, errc>;

    template <error_code ErrorType>
    constexpr string_view default_error_message(ErrorType);

    enum struct [[rythe_closed_enum]] error_severity : uint8
    {
        warning,
        error,
        fatal
    };

    using error_message = static_string<RSL_ERR_MESSAGE_MAX_LENGTH>;

    struct error_type
    {
        errc code;
        error_message message;
        error_severity severity;
        errid errorBlockStart;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator errc() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const error_type& other) const noexcept;
    };

    constexpr static error_type success = error_type{
                .code = no_error_code,
                .message = {},
                .severity = static_cast<error_severity>(-1),
                .errorBlockStart = 0
            };

    using error_list = static_array<error_type, RSL_ERR_MAX_COUNT>;
    using error_view = typename error_list::view_type;

    struct error_handler
    {
        virtual ~error_handler() = default;

        virtual void handle_assert(
                string_view expression,
                string_view file,
                size_type line,
                string_view message,
                bool soft,
                bool* ignore
                );

        virtual void handle_error(const error_type& error, bool assertError);
    };

    struct error_context
    {
        error_list errors;
        errid currentError = invalid_err_id;
        error_handler* errorHandlerOverride = nullptr;
        bool assertOnError = true;
    };

    DECLARE_SINGLETON(error_context)

    void enable_assert_on_error(bool enabled = true) noexcept;
    void disable_assert_on_error() noexcept;
    bool assert_on_error_enabled() noexcept;

    struct scoped_assert_on_error final
    {
        bool wasEnabled;
        scoped_assert_on_error(bool enabled = true);
        ~scoped_assert_on_error();
    };

    void set_error_handler(error_handler* errorHandler) noexcept;
    error_handler* get_error_handler() noexcept;

    struct scoped_error_handler final
    {
        error_handler* previousErrorHandler;
        scoped_error_handler(error_handler* errorHandler);
        ~scoped_error_handler();
    };

    struct error_signal {};

    struct result_base;

    namespace internal
    {
        errid& get_errid(result_base&) noexcept;
    }

    struct result_base
    {
    protected:
        errid m_errid = invalid_err_id;

        constexpr result_base() noexcept = default;
        result_base(error_signal) noexcept;
        friend errid& internal::get_errid(result_base&) noexcept;

    public:
        [[nodiscard]] [[rythe_always_inline]] bool is_okay() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] const error_type& get_error() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] error_view get_errors() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] errid id() const noexcept;

        [[rythe_always_inline]] void resolve() noexcept;
        [[nodiscard]] [[rythe_always_inline]] static error_signal propagate() noexcept;
        [[nodiscard]] [[rythe_always_inline]] bool reduce_and_discard() noexcept;

        [[rythe_always_inline]] errc report_errors() noexcept;

        virtual ~result_base();
    };

    template <typename...>
    struct result;

    template <typename T>
    struct [[nodiscard]] result<T> final : public result_base
    {
    public:
        using result_type = remove_cvr_t<T>;

        [[rythe_always_inline]] result(error_signal) noexcept;

        template <typename... Args>
            requires(rsl::constructible_from<T, Args...>)
        [[rythe_always_inline]] constexpr result(Args&&... args) noexcept(rsl::is_nothrow_constructible_v<T, Args...>);

        [[nodiscard]] [[rythe_always_inline]] bool carries_value() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] result_type& value() noexcept;
        [[nodiscard]] [[rythe_always_inline]] result_type* operator->() noexcept;
        [[nodiscard]] [[rythe_always_inline]] result_type& operator*() noexcept;

    private:
        union
        {
            T m_value;
            byte m_dummy;
        };
    };

    template <>
    struct [[nodiscard]] result<void> final : public result_base
    {
        [[rythe_always_inline]] result(error_signal) noexcept : result_base(error_signal{}) {}
        [[rythe_always_inline]] constexpr result() noexcept : result_base() {}
    };

    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_error(
            ErrorType errorType,
            string_view message,
            error_severity severity = error_severity::error
            ) noexcept;

    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_error(
            ErrorType errorType,
            error_severity severity = error_severity::error
            ) noexcept;

    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_warning(ErrorType errorType, string_view message) noexcept;
    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_warning(ErrorType errorType) noexcept;

    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_fatal(ErrorType errorType, string_view message) noexcept;
    template <error_code ErrorType>
    [[nodiscard]] [[rythe_always_inline]] constexpr error_signal make_fatal(ErrorType errorType) noexcept;

    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_error(
            result<T>& result,
            ErrorType errorType,
            string_view message,
            error_severity severity = error_severity::error
            ) noexcept;

    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_error(
            result<T>& result,
            ErrorType errorType,
            error_severity severity = error_severity::error
            ) noexcept;

    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_warning(result<T>& result, ErrorType errorType, string_view message) noexcept;
    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_warning(result<T>& result, ErrorType errorType) noexcept;

    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_fatal(result<T>& result, ErrorType errorType, string_view message) noexcept;
    template <typename T, error_code ErrorType>
    [[rythe_always_inline]] constexpr void append_fatal(result<T>& result, ErrorType errorType) noexcept;

    template <typename T, typename... Args>
    [[nodiscard]] [[rythe_always_inline]] constexpr result<T> make_result(
            Args&&... args
            )
        noexcept(rsl::is_nothrow_constructible_v<result<T>, Args...>);

    inline static result<void> okay = result<void>();

    struct unspecified_error
    {
        enum struct unspecified_error_code : errc
        {
            unspecified = -1,
        };

        template <typename T>
        operator result<T>();
    };

    inline static unspecified_error error = unspecified_error{};
} // namespace rsl

#include "error_handling.inl"
