#pragma once
#include "../../containers/string.hpp"
#include "../../logging/fmt_include.hpp"

#include "../../util/assert.hpp"

#include "../color.hpp"
#include "../matrix/matrix.hpp"
#include "../quaternion/quaternion.hpp"
#include "../vector/vector.hpp"

namespace rsl::log::internal
{
	constexpr const char* parse_float(const fmt::format_parse_context& ctx, char& presentation)
	{
		auto it = ctx.begin();
		const auto end = ctx.end();

		if (!it)
		{
			return nullptr;
		}

		if (it != end && (*it == 'f' || *it == 'e'))
		{
			presentation = *it++;
		}

		rsl_assert_msg_rarely((it == end || *it == '}'), "invalid format");

		return it;
	}
} // namespace rsl::log::internal

namespace fmt
{
	template <>
	struct formatter<rsl::math::float2>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::float2& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				format_string<float, float>(presentation == 'f' ? "({:.1f}, {:.1f})" : "({:.1e}, {:.1e})"), p.x,
				p.y
			);
		}
	};

	template <>
	struct formatter<rsl::math::int2>
	{
		constexpr const char* parse(format_parse_context& ctx) { return formatter<int>{}.parse(ctx); }

		template <typename FormatContext>
		auto format(const rsl::math::int2& p, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {})", p.x, p.y);
		}
	};

	template <>
	struct formatter<rsl::math::float3>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::float3& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float>(
					presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e})"
				),
				p.x, p.y, p.z
			);
		}
	};

	template <>
	struct formatter<rsl::math::int3>
	{
		constexpr const char* parse(format_parse_context& ctx) { return formatter<int>{}.parse(ctx); }

		template <typename FormatContext>
		constexpr auto format(const rsl::math::int3& p, FormatContext& ctx)
		{
			return format_to(ctx.out(), "({}, {}, {})", p.x, p.y, p.z);
		}
	};

	template <>
	struct formatter<rsl::math::float4>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::float4& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float, float>(
					presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e}, {:.1e})"
				),
				p.x, p.y, p.z, p.w
			);
		}
	};

	template <>
	struct formatter<rsl::math::float3x3> : fmt::formatter<std::string>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::float3x3& p, FormatContext& ctx) const -> decltype(ctx.out())
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float, float, float, float, float, float, float>(
					presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f})\n"
										  "({:.1f}, {:.1f}, {:.1f})\n"
										  "({:.1f}, {:.1f}, {:.1f})"
										: "({:.1e}, {:.1e}, {:.1e})\n"
										  "({:.1e}, {:.1e}, {:.1e})\n"
										  "({:.1e}, {:.1e}, {:.1e})"
				),
				p[0][0], p[0][1], p[0][2], p[1][0], p[1][1], p[1][2], p[2][0], p[2][1], p[2][2]
			);
		}
	};

	template <>
	struct formatter<rsl::math::float4x4> : fmt::formatter<std::string>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::float4x4& p, FormatContext& ctx) const -> decltype(ctx.out())
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>(
					presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
										  "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
										  "({:.1f}, {:.1f}, {:.1f}, {:.1f})\n"
										  "({:.1f}, {:.1f}, {:.1f}, {:.1f})"
										: "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
										  "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
										  "({:.1e}, {:.1e}, {:.1e}, {:.1e})\n"
										  "({:.1e}, {:.1e}, {:.1e}, {:.1e})"
				),
				p[0][0], p[0][1], p[0][2], p[0][3], p[1][0], p[1][1], p[1][2], p[1][3], p[2][0], p[2][1], p[2][2],
				p[2][3], p[3][0], p[3][1], p[3][2], p[3][3]
			);
		}
	};

	template <>
	struct formatter<rsl::math::color>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::color& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float, float>(
					presentation == 'f' ? "({:.1f}, {:.1f}, {:.1f}, {:.1f})" : "({:.1e}, {:.1e}, {:.1e}, {:.1e})"
				),
				p.r, p.g, p.b, p.a
			);
		}
	};

	template <>
	struct formatter<rsl::math::quat>
	{
		char presentation = 'f';

		constexpr const char* parse(format_parse_context& ctx)
		{
			return rsl::log::internal::parse_float(ctx, presentation);
		}

		template <typename FormatContext>
		constexpr auto format(const rsl::math::quat& p, FormatContext& ctx)
		{
			return format_to(
				ctx.out(),
				format_string<float, float, float, float>(
					presentation == 'f' ? "(({:.1f}, {:.1f}, {:.1f}), w: {:.1f})"
										: "(({:.1e}, {:.1e}, {:.1e}), w: {:.1e})"
				),
				p.i, p.j, p.k, p.w
			);
		}
	};
} // namespace fmt
