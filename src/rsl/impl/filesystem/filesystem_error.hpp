#pragma once
#include "../defines.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    enum struct [[rythe_closed_enum]] filesystem_error : errc
    {
        no_error = no_error_code,
        domain_not_found,
        directory_not_found,
        file_not_found,
        no_solution_found,
        multiple_solutions_found,
        invalid_solution,
        invalid_filesystem,
        invalid_operation,
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
            case filesystem_error::invalid_operation: return "Invalid operation.";
            default: return "Unknown filesystem_error.";
        }
    }
}
