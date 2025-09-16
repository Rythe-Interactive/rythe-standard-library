#include "view.hpp"

#include "file_solution.hpp"
#include "filesystem_registry.hpp"

namespace rsl::filesystem
{
    result<void> view::prefetch_solution(const bool ignoreMultipleSolutions) const
    {
        if (m_solution == nullptr)
        {
            auto result = get_filesystem_registry().find_solution(m_path, ignoreMultipleSolutions);
            if (result.is_okay())
            {
                m_solution = result.value();
            }
            else
            {
                return result.propagate();
            }
        }

        return okay;
    }

    file_traits view::file_info() const
    {
        return find_solution()->file_info();
    }

    filesystem_traits view::filesystem_info() const
    {
        return find_solution()->filesystem_info();
    }

    result<dynamic_array<view>> view::ls() const
    {
        return find_solution()->ls();
    }

    result<byte_view> view::read() const
    {
        return find_solution()->read();
    }

    result<void> view::write(const byte_view data) // NOLINT
    {
        return find_solution()->write(data);
    }

    result<void> view::append(const byte_view data) // NOLINT
    {
        return find_solution()->append(data);
    }

    result<void> view::flush() const
    {
        return find_solution()->flush();
    }

    void view::set_path(dynamic_string&& path)
    {
        m_path = rsl::move(path);
        if (m_solution)
        {
            m_solution->release();
        }
        m_solution = nullptr;
    }

    const file_solution* view::find_solution() const
    {
        prefetch_solution().report_errors();
        return m_solution;
    }

    file_solution* view::find_solution()
    {
        prefetch_solution().report_errors();
        return m_solution;
    }
}
