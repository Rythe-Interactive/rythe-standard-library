#pragma once
#include "archive_registry.hpp"
#include "filesystem_error.hpp"

namespace rsl::fs
{
    class file_solution;
    class archive_registry;

    class registry_iterator
    {
    public:
        using difference_type = diff_type;
        using value_type = archive_registry;
        using ref_type = value_type&;
        using ptr_type = value_type*;

        RULE_OF_5_CONSTEXPR_NOEXCEPT(registry_iterator);
        [[rythe_always_inline]] constexpr registry_iterator(archive_registry** ptr) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const registry_iterator& other) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const registry_iterator& other) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept;
        [[rythe_always_inline]] constexpr registry_iterator& operator++() noexcept;
        [[rythe_always_inline]] constexpr registry_iterator operator++(int) noexcept;
        [[rythe_always_inline]] constexpr registry_iterator& operator--() noexcept;
        [[rythe_always_inline]] constexpr registry_iterator operator--(int) noexcept;
        constexpr archive_registry& operator*() const noexcept;
        constexpr archive_registry* operator->() const noexcept;

    private:
        archive_registry** m_ptr;
    };

    class filesystem
    {
    public:
        void add_registry(archive_registry* registry);

        result<file_solution*> find_solution(string_view path, bool ignoreMultipleSolutions = false);
        bool has_domain(string_view domain) const noexcept;
        iterator_view<registry_iterator> registries() noexcept;

    private:
        dynamic_array<archive_registry*> m_archiveRegistries;
    };

    DECLARE_SINGLETON(filesystem)
}

#include "filesystem.inl"
