#pragma once
#include "../../logging/fmt_include.hpp"

#include "../buffered_string.hpp"
#include "../constexpr_string.hpp"
#include "../string.hpp"

namespace fmt
{
    template<>
    struct formatter<rsl::string_view>
    {
        formatter<fmt::string_view> stringViewFormatter;
        constexpr const char* parse(format_parse_context& ctx) { return stringViewFormatter.parse(ctx); }

        template <typename FormatContext>
        auto format(const rsl::string_view& str, FormatContext& ctx) const
        {
            return stringViewFormatter.format(fmt::string_view(str.data(), str.size()), ctx);
        }
    };

    template <rsl::char_type CharType, rsl::allocator_type Alloc, rsl::size_type StaticCapacity>
    struct formatter<rsl::basic_dynamic_string<CharType, Alloc, StaticCapacity>>
    {
        formatter<rsl::string_view> stringViewFormatter;

        constexpr const char* parse(format_parse_context& ctx) { return stringViewFormatter.parse(ctx); }

        template <typename FormatContext>
        auto format(const rsl::basic_dynamic_string<CharType, Alloc, StaticCapacity>& str, FormatContext& ctx) const
        {
            return stringViewFormatter.format(str.view(), ctx);
        }
    };

	template <rsl::size_type MaxSize, typename CharType>
	struct formatter<rsl::buffered_string<MaxSize, CharType>>
	{
	    formatter<rsl::string_view> stringViewFormatter;

	    constexpr const char* parse(format_parse_context& ctx) { return stringViewFormatter.parse(ctx); }

		template <typename FormatContext>
		auto format(const rsl::buffered_string<MaxSize, CharType>& str, FormatContext& ctx) const
		{
            return stringViewFormatter.format(str.view(), ctx);
		}
	};

	template <rsl::size_type N>
	struct formatter<rsl::constexpr_string<N>>
	{
	    formatter<rsl::string_view> stringViewFormatter;

        constexpr const char* parse(format_parse_context& ctx) { return stringViewFormatter.parse(ctx); }

		template <typename FormatContext>
		auto format(const rsl::constexpr_string<N>& str, FormatContext& ctx) const
		{
            return stringViewFormatter.format(str.view(), ctx);
		}
	};
} // namespace fmt
