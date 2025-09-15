#pragma once
#include "../defines.hpp"
#include "../containers/string.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    enum struct [[rythe_closed_enum]] memory_error : errc
    {
        no_error = 0,
        out_of_memory,
        allocation_failure,
    };

    template <>
    constexpr string_view default_error_message<memory_error>(const memory_error err)
    {
        switch (err)
        {
            case memory_error::no_error: return "No error.";
            case memory_error::out_of_memory: return "Out of memory.";
            case memory_error::allocation_failure: return "Allocation failure.";
            default: return "Unknown memory_error.";
        }
    }
}

template <>
struct fmt::formatter<rsl::memory_error>
{
    static constexpr const char* parse(format_parse_context& ctx)
    {
        return formatter<string_view>{}.parse(ctx);
    }

    template <typename FormatContext>
    static constexpr auto format(const rsl::memory_error& err, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", rsl::default_error_message(err));
    }
};
