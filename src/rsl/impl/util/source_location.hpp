#pragma once

#include "primitives.hpp"

namespace rsl
{
	struct source_location
	{
		[[nodiscard]] static consteval source_location current(
			const uint32 line = static_cast<uint32>(__builtin_LINE()),
			const uint32 column = static_cast<uint32>(__builtin_COLUMN()),
			const cstring file = static_cast<cstring>(__builtin_FILE()),
			const cstring function = static_cast<cstring>(__builtin_FUNCTION())
		) noexcept
		{
			source_location result;
			result.m_line = line;
			result.m_column = column;
			result.m_file = file;
			result.m_function = function;
			return result;
		}

		[[nodiscard]] constexpr source_location() noexcept = default;

		[[nodiscard]] constexpr uint32 line() const noexcept
		{
			return m_line;
		}

		[[nodiscard]] constexpr uint32 column() const noexcept
		{
			return m_column;
		}

		[[nodiscard]] constexpr cstring file_name() const noexcept
		{
			return m_file;
		}

		[[nodiscard]] constexpr cstring function_name() const noexcept
		{
			return m_function;
		}

	private:
		uint32 m_line{};
		uint32 m_column{};
		cstring m_file = "";
		cstring m_function = "";
	};
}
