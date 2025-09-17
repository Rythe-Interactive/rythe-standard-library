#pragma once
#include "../defines.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    enum struct [[rythe_closed_enum]] platform_error : errc
    {
        no_error = no_error_code,
        file_not_found,
        no_permission,
        invalid_argument,
        out_of_memory,
        eof_reached,
        already_exists,
        disk_full,
        not_supported,
        broken_pipe,
        temporary_still_running,
        directory_not_empty,
        sharing_violation,
        canceled,
        temporary_time_out,
        buffer_too_small,
        too_many_links,
        network_down,
        connection_refused,
        address_in_use,
        address_not_available,
        no_connection,
        network_unreachable,
        connection_closed,
        path_too_long,
        encoding_error,
        generic_error,
    };

    template <>
    constexpr string_view default_error_message<platform_error>(const platform_error err)
    {
        switch (err)
        {
            case platform_error::no_error: return "No error."_sv;
            case platform_error::file_not_found: return "File not found."_sv;
            case platform_error::no_permission: return "No permission."_sv;
            case platform_error::invalid_argument: return "Invalid argument."_sv;
            case platform_error::out_of_memory: return "Out of memory."_sv;
            case platform_error::eof_reached: return "Eof reached."_sv;
            case platform_error::already_exists: return "Already exists."_sv;
            case platform_error::disk_full: return "Disk full."_sv;
            case platform_error::not_supported: return "Not supported."_sv;
            case platform_error::broken_pipe: return "Broken pipe."_sv;
            case platform_error::temporary_still_running: return "Temporary still running."_sv;
            case platform_error::directory_not_empty: return "Directory not empty."_sv;
            case platform_error::sharing_violation: return "Sharing violation."_sv;
            case platform_error::canceled: return "Canceled."_sv;
            case platform_error::temporary_time_out: return "Temporary time out."_sv;
            case platform_error::buffer_too_small: return "Buffer too small."_sv;
            case platform_error::too_many_links: return "Too many links."_sv;
            case platform_error::network_down: return "Network down."_sv;
            case platform_error::connection_refused: return "Connection refused."_sv;
            case platform_error::address_in_use: return "Address in use."_sv;
            case platform_error::address_not_available: return "Address not available."_sv;
            case platform_error::no_connection: return "No connection."_sv;
            case platform_error::network_unreachable: return "Network unreachable."_sv;
            case platform_error::connection_closed: return "Connection closed."_sv;
            case platform_error::path_too_long: return "Path too long."_sv;
            case platform_error::encoding_error: return "Encoding error."_sv;
            case platform_error::generic_error: return "Generic platform error."_sv;
            default: return "Unknown platform_error.";
        }
    }
}
