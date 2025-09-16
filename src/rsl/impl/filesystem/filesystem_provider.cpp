#include "filesystem_provider.hpp"

#include "file_solution.hpp"

namespace rsl::filesystem
{
    filesystem_provider& filesystem_provider::register_domain(const string_view domain) noexcept
    {
        m_domains.push_back(domain_string::from_view(domain));
        return *this;
    }

    array_view<const domain_string> filesystem_provider::get_domains() const noexcept
    {
        return m_domains;
    }

    filesystem_traits filesystem_provider::filesystem_info() const
    {
        return filesystem_traits{
            .isReadonly = is_readonly(),
            .isValid = is_valid(),
        };
    }

    void filesystem_provider::set_solution_provider(file_solution* solution, filesystem_provider* provider)
    {
        solution->m_provider = provider;
    }

    pair<index_type, bool> filesystem_provider::create_solution_reference(const string_view path)
    {
        auto [index, newValue] = m_solutionMap.try_emplace(dynamic_string::from_view(path));
        if (newValue)
        {
            index = m_solutionIndexAllocator.allocate_index();
            if (index >= m_solutionReferences.size())
            {
                m_solutionReferences.resize(index + 1ull);
            }
        }

        m_solutionReferences[index].borrow();

        return { index, newValue };
    }

    void filesystem_provider::destroy_solution_reference(const string_view path)
    {
        const index_type index = m_solutionMap.at(path);
        m_solutionReferences[index].release();

        if (m_solutionReferences[index].is_free())
        {
            m_solutionMap.erase(path);
            m_solutionIndexAllocator.free_index(index);
        }
    }

    index_type filesystem_provider::find_existing_solution(const string_view path)
    {
        if (const index_type* result = m_solutionMap.find(path))
        {
            return *result;
        }

        return npos;
    }

    const manual_reference_counter& filesystem_provider::get_reference_count_status(const index_type index) const
    {
        return m_solutionReferences[index];
    }
}
