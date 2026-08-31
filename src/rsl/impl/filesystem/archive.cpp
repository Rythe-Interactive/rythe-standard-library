#include "archive.hpp"

#include "file_solution.hpp"

namespace rsl::fs
{
    archive& archive::register_domain(const string_view domain) noexcept
    {
        m_domains.push_back(domain_string::from_view(domain));
        return *this;
    }

    array_view<const domain_string> archive::get_domains() const noexcept
    {
        return m_domains;
    }

    filesystem_traits archive::filesystem_info() const
    {
        return filesystem_traits{
            .isReadonly = is_readonly(),
            .isValid = is_valid(),
        };
    }

    file_solution_handle archive::get_solution_handle(file_solution solution) noexcept
    {
        return solution.m_handle;
    }

    pair<index_type, bool> archive::create_solution_reference(const string_view path)
    {
        auto [index, newValue] = m_solutionMap.try_emplace(path);
        if (newValue)
        {
            index = m_solutionIndexAllocator.allocate_index();
            if (index >= m_solutionReferences.size())
            {
                m_solutionReferences.resize(index + 1ull);
                m_solutionVersions.resize(index + 1ull, counter_of_handle<file_solution_handle>{0});
            }
            m_solutionVersions[index].value++;
        }

        m_solutionReferences[index].borrow();

        return { index, newValue };
    }

    void archive::destroy_solution_reference(const string_view path)
    {
        const index_type index = m_solutionMap.at(path);
        m_solutionReferences[index].release();

        if (m_solutionReferences[index].is_free())
        {
            m_solutionMap.erase(path);
            m_solutionIndexAllocator.free_index(index);
        }
    }
}
