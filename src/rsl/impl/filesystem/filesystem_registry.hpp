#pragma once
#include "../containers/views.hpp"
#include "../memory/unique_object.hpp"
#include "../util/error_handling.hpp"

#include "filesystem_provider.hpp"

namespace rsl::filesystem
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
        [[rythe_always_inline]] constexpr domain_iterator(const unique_object<filesystem_provider>* provider) noexcept;

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
        const unique_object<filesystem_provider>* m_provider = nullptr;
    };

    // TODO: either name it a archive_registry, or filesystem.
    // TODO: potentially have both and split responsibilities of owning archives and using archives
    class filesystem_registry
    {
    public:
        filesystem_registry() = default;
        virtual ~filesystem_registry() = default;

        template <derived_from<filesystem_provider> ProviderType, typename... Args>
        void register_provider(Args&&... args);
        void register_provider(temporary_object<filesystem_provider>&& provider);

        result<file_solution*> find_solution(string_view path, bool ignoreMultipleSolutions = false);

        bool has_domain(string_view domain) const noexcept;
        array_view<const unique_object<filesystem_provider>> providers() const noexcept;
        iterator_view<domain_iterator> domains() const noexcept;

    protected:
        dynamic_map<string_view, dynamic_array<filesystem_provider*>> m_domainMap;
        dynamic_array<unique_object<filesystem_provider>> m_providers;
    };

    DECLARE_SINGLETON(filesystem_registry)
}

#include "filesystem_registry.inl"
