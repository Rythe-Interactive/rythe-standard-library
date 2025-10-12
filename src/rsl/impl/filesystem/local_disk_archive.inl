#pragma once

namespace rsl::filesystem
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
}
