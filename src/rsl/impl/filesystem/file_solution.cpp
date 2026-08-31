#include "file_solution.hpp"

#include "view.hpp"

namespace rsl::fs
{
    result<dynamic_array<view>> file_solution::ls() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->ls(m_handle);
    }
}
