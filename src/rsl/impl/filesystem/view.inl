#pragma once
#include "path_util.hpp"

namespace rsl::fs
{
    constexpr view::view(const string_view path) noexcept : m_path(dynamic_string::from_view(path)) {}

    constexpr view::view(dynamic_string&& path) noexcept : m_path(rsl::move(path)) {}

    inline view::operator bool() const noexcept
    {
        return is_valid();
    }

    inline bool view::is_valid(const bool deepCheck) const
    {
        if (m_path.empty())
        {
            return false;
        }

        if (deepCheck)
        {
            return prefetch_solution().reduce_and_discard();
        }

        return true;
    }

    inline dynamic_string view::domain() const
    {
        return fs::domain(m_path);
    }

    inline string_view view::path() const noexcept
    {
        return m_path;
    }

    inline string_view view::extension(const bool fullExtension) const noexcept
    {
        return fs::extension(m_path, fullExtension);
    }

    inline string_view view::filename() const noexcept
    {
        return fs::filename(m_path);
    }

    inline string_view view::stem() const noexcept
    {
        return fs::stem(m_path);
    }

    inline view view::parent() const
    {
        return view(fs::parent(m_path));
    }

    inline view view::subdir(const string_view identifier) const
    {
        return view(fs::subdir(m_path, identifier));
    }

    inline view view::operator/(const string_view identifier) const
    {
        return subdir(identifier);
    }

    constexpr view_list::view_list(const dynamic_array<fs::view>& src) noexcept : dynamic_array<fs::view>(src) {}
    constexpr view_list::view_list(dynamic_array<fs::view>&& src) noexcept : dynamic_array<fs::view>(rsl::move(src)) {}
}
