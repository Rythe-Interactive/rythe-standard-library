#pragma once

#include "../defines.hpp"

#include "primitives.hpp"

#include "../containers/views.hpp"
#include "../util/container_util.hpp"

#define rsl_mock_assert(expr)                                                                                          \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
	}
#define rsl_mock_assert_msg(expr, msg)                                                                                 \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
		[[maybe_unused]] const char* m = static_cast<const char*>(msg);                                                \
	}

#define rsl_mock_assert_soft(expr)                                                                                     \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
	}
#define rsl_mock_assert_msg_soft(expr, msg)                                                                            \
	if constexpr (false)                                                                                               \
	{                                                                                                                  \
		[[maybe_unused]] bool b = static_cast<bool>(expr);                                                             \
		[[maybe_unused]] const char* m = static_cast<const char*>(msg);                                                \
	}

namespace rsl
{
    namespace asserts
    {
        namespace internal
        {
            void default_assert_handler(
                    string_view expression,
                    string_view file,
                    size_type line,
                    string_view message,
                    bool soft,
                    const bool* ignore
                    );
        }

        using assert_handler_function = void (*)(
                string_view expression,
                string_view file,
                size_type line,
                string_view message,
                bool soft,
                bool* ignore
                );

        extern assert_handler_function assert_handler;
    } // namespace asserts

    namespace internal
    {
        template<typename ExprType, typename FileType, typename MsgType>
        constexpr void __rsl_assert_impl(ExprType&& expr, FileType&& file, const size_type line, MsgType&& msg, const bool soft, bool* ignore)
        {
            if(!is_constant_evaluated())
            {
                const string_view exprView = view_from_stringish(expr);
                const string_view fileView = view_from_stringish(file);
                const string_view msgView = view_from_stringish(msg);

                if (!asserts::assert_handler)
                {
                    asserts::internal::default_assert_handler(exprView, fileView, line, msgView, soft, ignore);
                }
                else
                {
                    asserts::assert_handler(exprView, fileView, line, msgView, soft, ignore);
                }
            }
        }
    }
}

#define rsl_assert_always(expr)                                                                                        \
	{                                                                                                                  \
		if (!!!(expr)) [[unlikely]]                                                                                    \
		{                                                                                                              \
			::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", false, nullptr);         \
		}                                                                                                              \
	}
#define rsl_assert_msg_always(expr, msg)                                                                               \
	{                                                                                                                  \
		if (!!!(expr)) [[unlikely]]                                                                                    \
		{                                                                                                              \
			::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, false, nullptr);        \
		}                                                                                                              \
	}

#define rsl_assert_soft_always(expr)                                                                                   \
	{                                                                                                                  \
		if (!!!(expr)) [[unlikely]]                                                                                    \
		{                                                                                                              \
			static bool ignore = false;                                                                                \
			::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, "", true, &ignore);          \
		}                                                                                                              \
	}
#define rsl_assert_msg_soft_always(expr, msg)                                                                          \
	{                                                                                                                  \
		if (!!!(expr)) [[unlikely]]                                                                                    \
		{                                                                                                              \
			static bool ignore = false;                                                                                \
			::rsl::internal::__rsl_assert_impl(RYTHE_STRINGIFY(expr), __FILE__, __LINE__, msg, true, &ignore);         \
		}                                                                                                              \
	}

#ifdef RYTHE_VALIDATE
#define rsl_assert_hard(expr) rsl_assert_always(expr)
#define rsl_assert_msg_hard(expr, msg) rsl_assert_msg_always(expr, msg)

#define rsl_assert_soft(expr) rsl_assert_soft_always(expr)
#define rsl_assert_msg_soft(expr, msg) rsl_assert_msg_soft_always(expr, msg)

#if RYTHE_VALIDATION_LEVEL >= RYTHE_HIGH_IMPACT_VALIDATION_LEVEL
#define rsl_assert_frequent(expr) rsl_assert_hard(expr)
#define rsl_assert_consistent(expr) rsl_assert_hard(expr)
#define rsl_assert_rarely(expr) rsl_assert_hard(expr)

#define rsl_assert_msg_frequent(expr, msg) rsl_assert_msg_hard(expr, msg)
#define rsl_assert_msg_consistent(expr, msg) rsl_assert_msg_hard(expr, msg)
#define rsl_assert_msg_rarely(expr, msg) rsl_assert_msg_hard(expr, msg)

#define rsl_assert_soft_frequent(expr) rsl_assert_soft(expr)
#define rsl_assert_soft_consistent(expr) rsl_assert_soft(expr)
#define rsl_assert_soft_rarely(expr) rsl_assert_soft(expr)

#define rsl_assert_msg_soft_frequent(expr, msg) rsl_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_consistent(expr, msg) rsl_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_rarely(expr, msg) rsl_assert_msg_soft(expr, msg)
#elif RYTHE_VALIDATION_LEVEL == RYTHE_MEDIUM_IMPACT_VALIDATION_LEVEL
#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
#define rsl_assert_consistent(expr) rsl_assert_hard(expr)
#define rsl_assert_rarely(expr) rsl_assert_hard(expr)

#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_msg_consistent(expr, msg) rsl_assert_msg_hard(expr, msg)
#define rsl_assert_msg_rarely(expr, msg) rsl_assert_msg_hard(expr, msg)

#define rsl_assert_soft_frequent(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_soft_consistent(expr) rsl_assert_soft(expr)
#define rsl_assert_soft_rarely(expr) rsl_assert_soft(expr)

#define rsl_assert_msg_soft_frequent(expr, msg) rsl_mock_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_consistent(expr, msg) rsl_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_rarely(expr, msg) rsl_assert_msg_soft(expr, msg)
#elif RYTHE_VALIDATION_LEVEL == RYTHE_LOW_IMPACT_VALIDATION_LEVEL
#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
#define rsl_assert_consistent(expr) rsl_mock_assert(expr)
#define rsl_assert_rarely(expr) rsl_assert_hard(expr)

#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_msg_consistent(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_msg_rarely(expr, msg) rsl_assert_msg_hard(expr, msg)

#define rsl_assert_soft_frequent(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_soft_consistent(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_soft_rarely(expr) rsl_assert_soft(expr)

#define rsl_assert_msg_soft_frequent(expr, msg) rsl_mock_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_consistent(expr, msg) rsl_mock_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_rarely(expr, msg) rsl_assert_msg_soft(expr, msg)
#endif
#else
#define rsl_assert_frequent(expr) rsl_mock_assert(expr)
#define rsl_assert_consistent(expr) rsl_mock_assert(expr)
#define rsl_assert_rarely(expr) rsl_mock_assert(expr)

#define rsl_assert_msg_frequent(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_msg_consistent(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_msg_rarely(expr, msg) rsl_mock_assert_msg(expr, msg)

#define rsl_assert_soft_frequent(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_soft_consistent(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_soft_rarely(expr) rsl_mock_assert_soft(expr)

#define rsl_assert_msg_soft_frequent(expr, msg) rsl_mock_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_consistent(expr, msg) rsl_mock_assert_msg_soft(expr, msg)
#define rsl_assert_msg_soft_rarely(expr, msg) rsl_mock_assert_msg_soft(expr, msg)

#define rsl_assert_hard(expr) rsl_mock_assert(expr)
#define rsl_assert_msg_hard(expr, msg) rsl_mock_assert_msg(expr, msg)
#define rsl_assert_soft(expr) rsl_mock_assert_soft(expr)
#define rsl_assert_msg_soft(expr, msg) rsl_mock_assert_msg_soft(expr, msg)

#endif // RYTHE_VALIDATE

#define rsl_ensure(expr) rsl_assert_always(expr)

#define rsl_assert_unimplemented() rsl_assert_msg_always(false, "function not implemented")
#define rsl_assert_unreachable() rsl_assert_msg_always(false, "reached unreachable code")
#define rsl_assert_ptr_out_of_range(ptr, begin, end) rsl_assert_msg_frequent(((ptr) > (begin)) && ((ptr) < (end)), "pointer out of range")
#define rsl_assert_ptr_out_of_range_msg(ptr, begin, end, msg) rsl_assert_msg_frequent(((ptr) > (begin)) && ((ptr) < (end)), msg)
#define rsl_assert_out_of_range(expr) rsl_assert_msg_frequent(expr, "out of range")
#define rsl_assert_out_of_range_msg(expr, msg) rsl_assert_msg_frequent(expr, msg)
#define rsl_assert_invalid_access(expr) rsl_assert_msg_frequent(expr, "invalid access")
#define rsl_assert_invalid_parameters(expr) rsl_assert_msg_rarely(expr, "invalid parameters")
#define rsl_assert_invalid_operation(expr) rsl_assert_msg_rarely(expr, "invalid operation")
#define rsl_assert_invalid_operation_frequent(expr) rsl_assert_msg_frequent(expr, "invalid operation")
#define rsl_assert_invalid_object(expr) rsl_assert_msg_rarely(expr, "invalid object")
#define rsl_assert_duplicate_object(expr) rsl_assert_msg_consistent(expr, "duplicate object")
#define rsl_assert_borrow_release_mismatch(expr) rsl_assert_msg_consistent(expr, "borrow release mismatch")
#define rsl_assert_invalid_cast(expr) rsl_assert_msg_rarely(expr, "invalid cast")
#define rsl_assert_invalid_pattern(expr) rsl_assert_msg_rarely(expr, "invalid pattern")
#define rsl_assert_alignment(ptr, alignment)                                                                           \
	rsl_assert_msg_rarely((reinterpret_cast<size_type>(ptr) & ((alignment) - 1)) == 0, "wrong alignment")
