#pragma once

namespace rsl::fs
{
    constexpr void filesystem::add_registry(pointer<archive_registry> registry)
    {
        m_archiveRegistries.push_back(registry);
    }

    inline bool filesystem::has_domain(const string_view domain) const noexcept
    {
        for (auto& archive : m_archiveRegistries)
        {
            if (archive->has_domain(domain))
            {
                return true;
            }
        }

        return false;
    }

    constexpr array_view<pointer<archive_registry>> filesystem::registries() noexcept
    {
        return m_archiveRegistries;
    }
} // namespace rsl::fs
