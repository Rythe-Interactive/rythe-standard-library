#pragma once
#include "../containers/array.hpp"
#include "../memory/index_allocator.hpp"
#include "../memory/reference_counter.hpp"
#include "../platform/file.hpp"
#include "../util/container_util.hpp"
#include "../util/error_handling.hpp"
#include "../util/versioned_handle.hpp"

#include "traits.hpp"

namespace rsl::fs
{
    using domain_string = hybrid_string<16>;

    RYTHE_DECLARE_VERSIONED_OPAQUE_HANDLE_BIT_COUNT(file_solution_handle, 24ull, 8ull)

    class view;

    class archive
    {
        friend class file_solution;

    public:
        VIRTUAL_RULE_OF_5(archive);

        template <string_like Domain, string_like... Domains>
        explicit archive(Domain&& domain, Domains&&... domains);

        archive& register_domain(string_view domain) noexcept;
        [[nodiscard]] array_view<const domain_string> get_domains() const noexcept;
        [[nodiscard]] filesystem_traits filesystem_info() const;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;
        [[nodiscard]] virtual bool is_readonly() const = 0;
        [[nodiscard]] virtual bool is_valid() const = 0;

        [[nodiscard]] virtual result<file_solution_handle> create_solution(string_view path) = 0;
        virtual void release_solution(file_solution_handle solutionHandle) = 0;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_valid_solution(file_solution_handle solutionHandle) const;

        [[nodiscard]] virtual bool is_file(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool is_directory(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool is_empty(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool is_valid_path(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool can_be_written(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool can_be_read(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool can_be_created(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual bool exists(file_solution_handle solutionHandle) const = 0;

        [[nodiscard]] virtual result<dynamic_array<view>> ls(file_solution_handle solutionHandle) const = 0;

        virtual void set_access_hint(file_solution_handle solutionHandle, file_access_flags flags) = 0;
        [[nodiscard]] virtual result<void> create(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual result<void> delete_entry(file_solution_handle solutionHandle, file_delete_flags flags) const = 0;
        [[nodiscard]] virtual result<byte_view> read(file_solution_handle solutionHandle) const = 0;
        [[nodiscard]] virtual result<void> write(file_solution_handle solutionHandle, byte_view data) = 0;
        [[nodiscard]] virtual result<void> append(file_solution_handle solutionHandle, byte_view data) = 0;
        [[nodiscard]] virtual result<void> flush(file_solution_handle solutionHandle) const = 0;

    protected:
        [[nodiscard]] static file_solution_handle get_solution_handle(file_solution solution) noexcept;

        pair<index_type, bool> create_solution_reference(string_view path);
        void destroy_solution_reference(string_view path);

        [[nodiscard]] [[rythe_always_inline]] constexpr index_type get_solution_index(file_solution_handle handle) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr file_solution_handle get_solution_handle(index_type index) const;

        [[nodiscard]] [[rythe_always_inline]] index_type find_existing_solution(string_view path);
        [[nodiscard]] [[rythe_always_inline]] constexpr const manual_reference_counter& get_reference_count_status(index_type index) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr manual_reference_counter& get_reference_counter(index_type index);

        dynamic_array<domain_string> m_domains;
        index_allocator m_solutionIndexAllocator;
        dynamic_map<dynamic_string, index_type> m_solutionMap;
        dynamic_array<manual_reference_counter> m_solutionReferences;
        dynamic_array<counter_of_handle<file_solution_handle>> m_solutionVersions;
    };
}

#include "archive.inl"
