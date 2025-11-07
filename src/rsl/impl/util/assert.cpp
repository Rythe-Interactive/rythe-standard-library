#include "assert.hpp"

#include "../../logging"

namespace rsl::asserts
{
	namespace internal
	{
		void default_assert_handler(
			string_view expression, string_view file, size_type line, string_view message, const bool soft, const bool* ignore
		)
		{
			if (ignore && *ignore)
			{
				return;
			}

			log::undecoratedln(
				soft ? log::severity::warn : log::severity::fatal,
				"Assertion failed:\t{}\nExpected:\t\t{}\nSource:\t\t\t{}, line {}"_sv, message, expression, file, line
			);

			rythe_debugbreak_instruction();

			if (!soft)
			{
				std::abort();
			}
		}
	} // namespace internal

	assert_handler_function assert_handler = nullptr;
} // namespace rsl::asserts
