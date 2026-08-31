#include "archive.hpp"
namespace rsl::fs
{
    template <string_like Domain, string_like ... Domains>
    archive::archive(Domain&& domain, Domains&&... domains)
    {
        register_domain(domain);
        (register_domain(domains), ...);
    }

    constexpr bool archive::is_valid_solution(file_solution_handle solutionHandle) const
    {
        return get_solution_index(solutionHandle) != npos;
    }

    constexpr index_type archive::get_solution_index(file_solution_handle handle) const
    {
        const auto [underlyingHandle, counter] = decompose_versioned_handle(handle);
        const index_type index = static_cast<index_type>(underlyingHandle) - 1ull;

        if (index > m_solutionReferences.size())
        {
            return npos;
        }

        if (m_solutionVersions[index].value != counter)
        {
            return npos;
        }

        return index;
    }

    constexpr file_solution_handle archive::get_solution_handle(index_type index) const
    {
        rsl_assert_out_of_range(index < m_solutionReferences.size());
        return make_versioned_handle<file_solution_handle>(
                static_cast<file_solution_handle_impl>(index + 1ull), m_solutionVersions[index].value);
    }

    inline index_type archive::find_existing_solution(const string_view path)
    {
        if (const index_type* result = m_solutionMap.find(path))
        {
            return *result;
        }

        return npos;
    }

    constexpr const manual_reference_counter& archive::get_reference_count_status(const index_type index) const
    {
        rsl_assert_out_of_range(index < m_solutionReferences.size());
        return m_solutionReferences[index];
    }

    constexpr manual_reference_counter& archive::get_reference_counter(index_type index)
    {
        rsl_assert_out_of_range(index < m_solutionReferences.size());
        return m_solutionReferences[index];
    }
}
