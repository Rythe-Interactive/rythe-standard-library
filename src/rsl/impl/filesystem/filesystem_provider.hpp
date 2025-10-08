#pragma once
#include "../containers/array.hpp"
#include "../memory/index_allocator.hpp"
#include "../util/container_util.hpp"
#include "../util/error_handling.hpp"

#include "traits.hpp"

namespace rsl::filesystem
{
    class view;

    using domain_string = hybrid_string<16>;

    // TODO: archive would be a more fitting name.
    class filesystem_provider
    {
        friend class file_solution;

    public:
        VIRTUAL_RULE_OF_5(filesystem_provider);

        template <string_like Domain, string_like... Domains>
        explicit filesystem_provider(Domain&& domain, Domains&&... domains);

        filesystem_provider& register_domain(string_view domain) noexcept;
        [[nodiscard]] array_view<const domain_string> get_domains() const noexcept;
        [[nodiscard]] filesystem_traits filesystem_info() const;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;
        [[nodiscard]] virtual bool is_readonly() const = 0;
        [[nodiscard]] virtual bool is_valid() const = 0;

        [[nodiscard]] virtual result<file_solution*> create_solution(string_view path) = 0;
        virtual void release_solution(file_solution* solution) = 0;

    protected:
        static void set_solution_provider(file_solution* solution, filesystem_provider* provider);

        [[nodiscard]] virtual result<void> open_file_for_read(const file_solution* solution) const = 0;
        [[nodiscard]] virtual result<void> open_file_for_write(file_solution* solution) = 0;
        [[nodiscard]] virtual result<void> close_file(const file_solution* solution) const = 0;
        [[nodiscard]] virtual result<void> flush_file(const file_solution* solution) const = 0;

        pair<index_type, bool> create_solution_reference(string_view path);
        void destroy_solution_reference(string_view path);
        index_type find_existing_solution(string_view path);
        const manual_reference_counter& get_reference_count_status(index_type index) const;

        dynamic_array<domain_string> m_domains;
        index_allocator m_solutionIndexAllocator;
        dynamic_map<dynamic_string, index_type> m_solutionMap;
        dynamic_array<manual_reference_counter> m_solutionReferences;
    };
}

#include "filesystem_provider.inl"
