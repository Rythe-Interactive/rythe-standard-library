#pragma once
#include "../containers/views.hpp"
#include "../memory/unique_object.hpp"
#include "../util/error_handling.hpp"

#include "archive.hpp"

namespace rsl::fs
{
    class file_solution;

    class domain_iterator
    {
    public:
        using difference_type = diff_type;
        using value_type = const domain_string;
        using ref_type = value_type&;
        using ptr_type = value_type*;

        RULE_OF_5_CONSTEXPR_NOEXCEPT(domain_iterator);
        [[rythe_always_inline]] constexpr domain_iterator(const unique_object<archive>* provider) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const domain_iterator& other) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const domain_iterator& other) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept;
        [[rythe_always_inline]] constexpr domain_iterator& operator++() noexcept;
        [[rythe_always_inline]] constexpr domain_iterator operator++(int) noexcept;
        [[rythe_always_inline]] constexpr domain_iterator& operator--() noexcept;
        [[rythe_always_inline]] constexpr domain_iterator operator--(int) noexcept;
        constexpr const domain_string& operator*() const noexcept;
        constexpr const domain_string* operator->() const noexcept;

    private:
        size_type m_index = 0ull;
        const unique_object<archive>* m_provider = nullptr;
    };

    class archive_registry
    {
    public:
        archive_registry() = default;
        virtual ~archive_registry() = default;

        template <derived_from<archive> ProviderType, typename... Args>
        void register_provider(Args&&... args);
        void register_provider(temporary_object<archive>&& provider);

        result<file_solution*> find_solution(string_view path, bool ignoreMultipleSolutions = false);

        bool has_domain(string_view domain) const noexcept;
        array_view<const unique_object<archive>> providers() const noexcept;
        iterator_view<domain_iterator> domains() const noexcept;

    protected:
        dynamic_map<string_view, dynamic_array<archive*>> m_domainMap;
        dynamic_array<unique_object<archive>> m_providers;
    };

    DECLARE_SINGLETON(archive_registry)
}

#include "archive_registry.inl"
