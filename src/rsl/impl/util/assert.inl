#include "../containers/views.hpp"
#include "../util/container_util.hpp"

#define FMT_HEADER_ONLY
#include <fmt/chrono.h> // NOLINT
#include <fmt/format.h>

namespace rsl
{
    namespace asserts
    {
        namespace internal
        {
            [[rythe_never_inline]] void default_assert_handler(
                    string_view expression, string_view file, size_type line, string_view message, bool soft, const bool* ignore);
            [[rythe_never_inline]] void
                    raw_assert_handler(string_view expression, string_view file, size_type line, string_view message);

            void forward_to_assert_handler(
                    string_view expression,
                    string_view file,
                    size_type line,
                    string_view message,
                    fmt::format_args args,
                    bool soft,
                    bool* ignore);
        } // namespace internal

        using assert_handler_function =
                void (*)(string_view expression, string_view file, size_type line, string_view message, bool soft, bool* ignore);

        extern assert_handler_function assert_handler;
    } // namespace asserts

    namespace internal
    {
        template <typename ExprType, typename FileType, typename MsgType, typename... Args>
        constexpr void __rsl_assert_impl(
                ExprType&& expr, FileType&& file, const size_type line, MsgType&& msg, const bool soft, bool* ignore, Args&&... args)
        {
            if (!is_constant_evaluated())
            {
                const string_view exprView = view_from_stringish(expr);
                const string_view fileView = view_from_stringish(file);
                asserts::internal::forward_to_assert_handler(
                        exprView, fileView, line, msg, fmt::vargs<Args...>{ { args... } }, soft, ignore);
            }
        }

        template <typename ExprType, typename FileType, typename MsgType, typename... Args>
        constexpr void __rsl_assert_raw_impl(ExprType&& expr, FileType&& file, const size_type line, MsgType&& msg, Args&&...)
        {
            if (!is_constant_evaluated())
            {
                const string_view exprView = view_from_stringish(expr);
                const string_view fileView = view_from_stringish(file);
                asserts::internal::raw_assert_handler(exprView, fileView, line, msg);
            }
        }
    } // namespace internal
} // namespace rsl

#define rsl_mock_assert(expr)                                                                                                         \
    if constexpr (false)                                                                                                              \
    {                                                                                                                                 \
        [[maybe_unused]] bool b = static_cast<bool>(expr);                                                                            \
    }
#define rsl_mock_assert_msg(expr, msg, ...)                                                                                           \
    if constexpr (false)                                                                                                              \
    {                                                                                                                                 \
        [[maybe_unused]] bool b = static_cast<bool>(expr);                                                                            \
        rsl::surpress_unused(msg, __VA_ARGS__);                                                                                       \
    }

#define rsl_mock_assert_soft(expr, ...)                                                                                               \
    if constexpr (false)                                                                                                              \
    {                                                                                                                                 \
        [[maybe_unused]] bool b = static_cast<bool>(expr);                                                                            \
    }
#define rsl_mock_assert_msg_soft(expr, msg, ...)                                                                                      \
    if constexpr (false)                                                                                                              \
    {                                                                                                                                 \
        [[maybe_unused]] bool b = static_cast<bool>(expr);                                                                            \
        rsl::surpress_unused(msg, __VA_ARGS__);                                                                                       \
    }

#define rsl_assert_raw(expr)                                                                                                          \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            ::rsl::internal::__rsl_assert_raw_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "");                                    \
        }                                                                                                                             \
    }

#define rsl_assert_msg_raw(expr, msg, ...)                                                                                            \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            ::rsl::internal::__rsl_assert_raw_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, __VA_ARGS__);                      \
        }                                                                                                                             \
    }

#define rsl_assert_always(expr)                                                                                                       \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            ::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", false, nullptr);                        \
        }                                                                                                                             \
    }
#define rsl_assert_msg_always(expr, msg, ...)                                                                                         \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            ::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, false, nullptr, __VA_ARGS__);          \
        }                                                                                                                             \
    }

#define rsl_assert_soft_always(expr)                                                                                                  \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            static bool ignore = false;                                                                                               \
            ::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", true, &ignore);                         \
        }                                                                                                                             \
    }
#define rsl_assert_msg_soft_always(expr, msg, ...)                                                                                    \
    {                                                                                                                                 \
        if (!!!(expr)) [[unlikely]]                                                                                                   \
        {                                                                                                                             \
            static bool ignore = false;                                                                                               \
            ::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, true, &ignore, __VA_ARGS__);           \
        }                                                                                                                             \
    }

#ifdef RYTHE_VALIDATE
    #define rsl_assert_hard(expr) rsl_assert_always(expr)
    #define rsl_assert_msg_hard(expr, msg, ...) rsl_assert_msg_always(expr, msg, __VA_ARGS__)

    #define rsl_assert_soft(expr) rsl_assert_soft_always(expr)
    #define rsl_assert_msg_soft(expr, msg, ...) rsl_assert_msg_soft_always(expr, msg, __VA_ARGS__)

    #if RYTHE_VALIDATION_LEVEL >= RYTHE_HIGH_IMPACT_VALIDATION_LEVEL
        #define rsl_assert_high_impact(expr) rsl_assert_hard(expr)
        #define rsl_assert_medium_impact(expr) rsl_assert_hard(expr)
        #define rsl_assert_low_impact(expr) rsl_assert_hard(expr)

        #define rsl_assert_msg_high_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_medium_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_low_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)

        #define rsl_assert_soft_high_impact(expr) rsl_assert_soft(expr)
        #define rsl_assert_soft_medium_impact(expr) rsl_assert_soft(expr)
        #define rsl_assert_soft_low_impact(expr) rsl_assert_soft(expr)

        #define rsl_assert_msg_soft_high_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_medium_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_low_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
    #elif RYTHE_VALIDATION_LEVEL == RYTHE_MEDIUM_IMPACT_VALIDATION_LEVEL
        #define rsl_assert_high_impact(expr) rsl_mock_assert(expr)
        #define rsl_assert_medium_impact(expr) rsl_assert_hard(expr)
        #define rsl_assert_low_impact(expr) rsl_assert_hard(expr)

        #define rsl_assert_msg_high_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_medium_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_low_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)

        #define rsl_assert_soft_high_impact(expr) rsl_mock_assert_soft(expr)
        #define rsl_assert_soft_medium_impact(expr) rsl_assert_soft(expr)
        #define rsl_assert_soft_low_impact(expr) rsl_assert_soft(expr)

        #define rsl_assert_msg_soft_high_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_medium_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_low_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
    #elif RYTHE_VALIDATION_LEVEL == RYTHE_LOW_IMPACT_VALIDATION_LEVEL
        #define rsl_assert_high_impact(expr) rsl_mock_assert(expr)
        #define rsl_assert_medium_impact(expr) rsl_mock_assert(expr)
        #define rsl_assert_low_impact(expr) rsl_assert_hard(expr)

        #define rsl_assert_msg_high_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_medium_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_low_impact(expr, msg, ...) rsl_assert_msg_hard(expr, msg, __VA_ARGS__)

        #define rsl_assert_soft_high_impact(expr) rsl_mock_assert_soft(expr)
        #define rsl_assert_soft_medium_impact(expr) rsl_mock_assert_soft(expr)
        #define rsl_assert_soft_low_impact(expr) rsl_assert_soft(expr)

        #define rsl_assert_msg_soft_high_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_medium_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)
        #define rsl_assert_msg_soft_low_impact(expr, msg, ...) rsl_assert_msg_soft(expr, msg, __VA_ARGS__)
    #endif
#else
    #define rsl_assert_high_impact(expr) rsl_mock_assert(expr)
    #define rsl_assert_medium_impact(expr) rsl_mock_assert(expr)
    #define rsl_assert_low_impact(expr) rsl_mock_assert(expr)

    #define rsl_assert_msg_high_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
    #define rsl_assert_msg_medium_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
    #define rsl_assert_msg_low_impact(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)

    #define rsl_assert_soft_high_impact(expr) rsl_mock_assert_soft(expr)
    #define rsl_assert_soft_medium_impact(expr) rsl_mock_assert_soft(expr)
    #define rsl_assert_soft_low_impact(expr) rsl_mock_assert_soft(expr)

    #define rsl_assert_msg_soft_high_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)
    #define rsl_assert_msg_soft_medium_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)
    #define rsl_assert_msg_soft_low_impact(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)

    #define rsl_assert_hard(expr) rsl_mock_assert(expr)
    #define rsl_assert_msg_hard(expr, msg, ...) rsl_mock_assert_msg(expr, msg, __VA_ARGS__)
    #define rsl_assert_soft(expr) rsl_mock_assert_soft(expr)
    #define rsl_assert_msg_soft(expr, msg, ...) rsl_mock_assert_msg_soft(expr, msg, __VA_ARGS__)

#endif // RYTHE_VALIDATE

#define rsl_ensure(expr) rsl_assert_always(expr)

#define rsl_assert_unimplemented() rsl_assert_msg_always(false, "function not implemented")
#define rsl_assert_unreachable() rsl_assert_msg_always(false, "reached unreachable code")
#define rsl_assert_ptr_out_of_range(ptr, begin, end)                                                                                  \
    rsl_assert_msg_high_impact(((ptr) > (begin)) && ((ptr) < (end)), "pointer out of range")
#define rsl_assert_ptr_out_of_range_msg(ptr, begin, end, msg, ...)                                                                    \
    rsl_assert_msg_high_impact(((ptr) > (begin)) && ((ptr) < (end)), msg, __VA_ARGS__)
#define rsl_assert_out_of_range(expr) rsl_assert_msg_high_impact(expr, "out of range")
#define rsl_assert_out_of_range_msg(expr, msg, ...) rsl_assert_msg_high_impact(expr, msg, __VA_ARGS__)
#define rsl_assert_invalid_access(expr) rsl_assert_msg_high_impact(expr, "invalid access")
#define rsl_assert_invalid_parameters(expr) rsl_assert_msg_low_impact(expr, "invalid parameters")
#define rsl_assert_invalid_operation(expr) rsl_assert_msg_low_impact(expr, "invalid operation")
#define rsl_assert_invalid_operation_frequent(expr) rsl_assert_msg_high_impact(expr, "invalid operation")
#define rsl_assert_invalid_object(expr) rsl_assert_msg_low_impact(expr, "invalid object")
#define rsl_assert_duplicate_object(expr) rsl_assert_msg_medium_impact(expr, "duplicate object")
#define rsl_assert_borrow_release_mismatch(expr) rsl_assert_msg_medium_impact(expr, "borrow release mismatch")
#define rsl_assert_invalid_cast(expr) rsl_assert_msg_low_impact(expr, "invalid cast")
#define rsl_assert_invalid_pattern(expr) rsl_assert_msg_low_impact(expr, "invalid pattern")
#define rsl_assert_alignment(ptr, alignment)                                                                                          \
    rsl_assert_msg_low_impact((reinterpret_cast<size_type>(ptr) & ((alignment) - 1)) == 0, "wrong alignment")
