#pragma once

namespace rsl::filesystem
{
    constexpr domain_iterator::domain_iterator(const unique_object<archive>* provider) noexcept : m_provider(provider) {}

    constexpr bool domain_iterator::operator==(const domain_iterator& other) const noexcept
    {
        return m_provider == other.m_provider;
    }

    constexpr bool domain_iterator::operator!=(const domain_iterator& other) const noexcept
    {
        return m_provider != other.m_provider;
    }

    constexpr domain_iterator::operator bool() const noexcept
    {
        return m_provider != nullptr;
    }

    constexpr domain_iterator& domain_iterator::operator++() noexcept
    {
        ++m_index;
        const auto domains = m_provider->get()->get_domains();
        if (m_index >= domains.size())
        {
            ++m_provider;
            m_index = 0;
        }
        return *this;
    }

    constexpr domain_iterator domain_iterator::operator++(int) noexcept
    {
        const domain_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    constexpr domain_iterator& domain_iterator::operator--() noexcept
    {
        if (m_index == 0ull)
        {
            --m_provider;
            m_index = m_provider->get()->get_domains().size() - 1ull;
        }
        else
        {
            --m_index;
        }
        return *this;
    }

    constexpr domain_iterator domain_iterator::operator--(int) noexcept
    {
        const domain_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    constexpr const domain_string& domain_iterator::operator*() const noexcept
    {
        return m_provider->get()->get_domains()[m_index];
    }

    constexpr const domain_string* domain_iterator::operator->() const noexcept
    {
        return &operator*();
    }

    template <derived_from<archive> ProviderType, typename... Args>
    void archive_registry::register_provider(Args&&... args)
    {
        register_provider(temporary_object<ProviderType>::create_in_place(rsl::forward<Args>(args)...));
    }
}
