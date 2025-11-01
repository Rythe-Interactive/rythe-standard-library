#pragma once

namespace rsl::fs
{
    constexpr registry_iterator::registry_iterator(archive_registry** ptr) noexcept : m_ptr(ptr) {}

    constexpr bool registry_iterator::operator==(const registry_iterator& other) const noexcept
    {
        return m_ptr == other.m_ptr;
    }

    constexpr bool registry_iterator::operator!=(const registry_iterator& other) const noexcept
    {
        return m_ptr != other.m_ptr;
    }

    constexpr registry_iterator::operator bool() const noexcept
    {
        return m_ptr;
    }

    constexpr registry_iterator& registry_iterator::operator++() noexcept
    {
        ++m_ptr;
        return *this;
    }

    constexpr registry_iterator registry_iterator::operator++(int) noexcept
    {
        const registry_iterator temp = *this;
        ++(*this);
        return temp;
    }

    constexpr registry_iterator& registry_iterator::operator--() noexcept
    {
        --m_ptr;
        return *this;
    }

    constexpr registry_iterator registry_iterator::operator--(int) noexcept
    {
        const registry_iterator temp = *this;
        --(*this);
        return temp;
    }

    constexpr archive_registry& registry_iterator::operator*() const noexcept
    {
        return **m_ptr;
    }

    constexpr archive_registry* registry_iterator::operator->() const noexcept
    {
        return *m_ptr;
    }

    inline void filesystem::add_registry(archive_registry* registry)
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

    inline iterator_view<registry_iterator> filesystem::registries() noexcept
    {
        return iterator_view(registry_iterator(m_archiveRegistries.begin()), registry_iterator(m_archiveRegistries.end()));
    }
}
