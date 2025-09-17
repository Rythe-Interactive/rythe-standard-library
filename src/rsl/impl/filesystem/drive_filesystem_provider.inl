#pragma once

namespace rsl::filesystem
{
    template <string_like Domain, string_like ... Domains>
    drive_filesystem_provider::drive_filesystem_provider(const string_view rootPath, Domain&& domain, Domains&&... domains)
        : filesystem_provider(rsl::forward<Domain>(domain), rsl::forward<Domains>(domains)...),
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
