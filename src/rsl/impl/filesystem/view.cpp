#include "view.hpp"

namespace rsl::fs
{
    result<void> view::create() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->create();
    }

    result<void> view::delete_entry(file_delete_flags flags) const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->delete_entry(flags);
    }

    file_traits view::file_info() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            solution.resolve();
            return invalid_file_traits;
        }

        return solution.value()->file_info();
    }

    filesystem_traits view::filesystem_info() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            solution.resolve();
            return invalid_filesystem_traits;
        }

        return solution.value()->filesystem_info();
    }

    result<view_list> view::ls() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        result<dynamic_array<view>> listResult = solution.value()->ls();
        if (listResult.carries_value())
        {
            return make_partial_result<view_list>(listResult.propagate(), listResult.value());
        }

        return listResult.propagate();
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
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->read();
    }

    result<void> view::write(const byte_view data) // NOLINT
    {
        result<file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->write(data);
    }

    result<void> view::append(const byte_view data) // NOLINT
    {
        result<file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->append(data);
    }

    result<void> view::flush() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            return solution.propagate();
        }
        return solution.value()->flush();
    }

    pointer<file_solution> view::get_solution()
    {
        result<file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            solution.resolve();
            return { nullptr };
        }
        return { *solution };
    }

    pointer<const file_solution> view::get_solution() const
    {
        result<const file_solution*> solution = find_solution();
        if (solution.has_errors())
        {
            solution.resolve();
            return { nullptr };
        }
        return { *solution };
    }

    void view::set_path(dynamic_string&& path)
    {
        m_path = rsl::move(path);
        release_solution();
    }

    result<const file_solution*> view::find_solution() const
    {
        result<void> result = prefetch_solution();
        if (result.has_errors())
        {
            return result.propagate();
        }
        return &m_solution;
    }

    result<file_solution*> view::find_solution()
    {
        result<void> result = prefetch_solution();
        if (result.has_errors())
        {
            return result.propagate();
        }
        return &m_solution;
    }
} // namespace rsl::fs
