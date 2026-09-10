#pragma once
#include "../memory/pointer.hpp"

#include "archive_registry.hpp"
#include "filesystem_error.hpp"

namespace rsl::fs
{
    class file_solution;
    class archive_registry;

    class filesystem
    {
    public:
        [[rythe_always_inline]] constexpr void add_registry(pointer<archive_registry> registry);

        [[nodiscard]] result<file_solution> find_solution(string_view path, bool ignoreMultipleSolutions = false);
        [[nodiscard]] [[rythe_always_inline]] bool has_domain(string_view domain) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr array_view<pointer<archive_registry>> registries() noexcept;

    private:
        dynamic_array<pointer<archive_registry>> m_archiveRegistries;
    };

    RYTHE_DECLARE_SINGLETON(filesystem)
}

#include "filesystem.inl"
