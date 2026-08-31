#include "local_disk_archive.hpp"
#pragma once

namespace rsl::fs
{
    template <string_like Domain, string_like ... Domains>
    local_disk_archive::local_disk_archive(const string_view rootPath, Domain&& domain, Domains&&... domains)
        : archive(rsl::forward<Domain>(domain), rsl::forward<Domains>(domains)...),
          m_rootPath(localize(rootPath))
    {
        if (!platform::is_path_valid(m_rootPath) && !platform::is_directory(rootPath))
        {
            m_rootPath.clear();
            m_rootPath.shrink_to_fit();
            return;
        }

        if (m_rootPath.back() != separator_char{})
        {
            m_rootPath.append(separator_char{});
        }
    }

    constexpr pointer<local_disk_file_solution_data> local_disk_archive::get_solution_data(const file_solution_handle handle) noexcept
    {
        const index_type solutionIndex = get_solution_index(handle);
        if (solutionIndex == npos) [[unlikely]]
        {
            return { nullptr };
        }

        return { &m_solutions[solutionIndex] };
    }

    constexpr pointer<const local_disk_file_solution_data>
        local_disk_archive::get_solution_data(const file_solution_handle handle) const noexcept
    {
        const index_type solutionIndex = get_solution_index(handle);
        if (solutionIndex == npos) [[unlikely]]
        {
            return { nullptr };
        }

        return { &m_solutions[solutionIndex] };
    }
}
