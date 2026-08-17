#include "view.hpp"

#include "file_solution.hpp"
#include "filesystem.hpp"

namespace rsl::fs
{
    result<void> view::prefetch_solution(const bool ignoreMultipleSolutions) const
    {
        if (m_solution == nullptr)
        {
            auto result = get_filesystem().find_solution(m_path, ignoreMultipleSolutions);
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

    bool view::exists() const
    {
        return file_info().exists;
    }

    file_traits view::file_info() const
    {
        const file_solution* solution = find_solution(false);
        if (!solution)
        {
            return invalid_file_traits;
        }

        return solution->file_info();
    }

    filesystem_traits view::filesystem_info() const
    {
        const file_solution* solution = find_solution(false);
        if (!solution)
        {
            return invalid_filesystem_traits;
        }

        return solution->filesystem_info();
    }

    result<view_list> view::ls() const
    {
        result<dynamic_array<view>> solutionResult = find_solution(true)->ls();
        if (solutionResult.carries_value())
        {
            return make_partial_result<view_list>(solutionResult.propagate(), solutionResult.value());
        }

        return solutionResult.propagate();
    }

    view& view::replace_extension(const string_view extension, const bool fullExtension)
    {
        // TODO(Glyn): move to path_util.hpp
        string_view oldExtension = fs::extension(m_path, fullExtension);
        m_path.resize(m_path.size() - oldExtension.size());
        m_path.append(extension);
        release_solution();
        return *this;
    }

    result<byte_view> view::read() const
    {
        return find_solution(true)->read();
    }

    result<void> view::write(const byte_view data) // NOLINT
    {
        return find_solution(true)->write(data);
    }

    result<void> view::append(const byte_view data) // NOLINT
    {
        return find_solution(true)->append(data);
    }

    result<void> view::flush() const
    {
        return find_solution(true)->flush();
    }

    void view::set_path(dynamic_string&& path)
    {
        m_path = rsl::move(path);
        release_solution();
    }

    void view::release_solution() const
    {
        if (m_solution)
        {
            m_solution->release();
        }
        m_solution = nullptr;
    }

    const file_solution* view::find_solution(bool reportErrors) const
    {
        result<void> result = prefetch_solution();
        if (reportErrors)
        {
            result.report_errors_and_resolve();
        }
        else if (result.has_errors())
        {
            result.resolve();
        }
        return m_solution;
    }

    file_solution* view::find_solution(bool reportErrors)
    {
        result<void> result = prefetch_solution();
        if (reportErrors)
        {
            result.report_errors_and_resolve();
        }
        else if (result.has_errors())
        {
            result.resolve();
        }
        return m_solution;
    }
}
