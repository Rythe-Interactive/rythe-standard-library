#pragma once
#include "../defines.hpp"
#include "../containers/string.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    enum struct [[rythe_closed_enum]] filesystem_error : errc
    {
        no_error = 0,
        domain_not_found,
        directory_not_found,
        file_not_found,
        no_solution_found,
        multiple_solutions_found,
        invalid_solution,
        invalid_filesystem,
    };

    template <>
    constexpr string_view default_error_message<filesystem_error>(const filesystem_error err)
    {
        switch (err)
        {
            case filesystem_error::no_error: return "No error.";
            case filesystem_error::domain_not_found: return "Domain not found.";
            case filesystem_error::directory_not_found: return "Directory not found.";
            case filesystem_error::file_not_found: return "File not found.";
            case filesystem_error::no_solution_found: return "No solution found.";
            case filesystem_error::multiple_solutions_found: return "Multiple solutions found.";
            case filesystem_error::invalid_solution: return "Invalid solution.";
            case filesystem_error::invalid_filesystem: return "Invalid filesystem.";
            default: return "Unknown filesystem_error.";
        }
    }
}

template <>
struct fmt::formatter<rsl::filesystem_error>
{
    static constexpr const char* parse(format_parse_context& ctx)
    {
        return formatter<string_view>{}.parse(ctx);
    }

    template <typename FormatContext>
    static constexpr auto format(const rsl::filesystem_error& err, FormatContext& ctx)
    {
        return format_to(ctx.out(), "{}", rsl::default_error_message(err));
    }
};
