namespace rsl::fs
{
    constexpr view::view(const string_view path, bool standardizePath) noexcept
        : m_path(dynamic_string::from_view(path))
    {
        if (standardizePath)
        {
            standardize();
        }
    }

    constexpr view::view(dynamic_string&& path, bool standardizePath) noexcept
        : m_path(rsl::move(path))
    {
        if (standardizePath)
        {
            standardize();
        }
    }

    constexpr void view::standardize() noexcept
    {
        rsl::fs::standardize(in_place_signal, m_path);
    }

    inline view::operator bool() const noexcept
    {
        return is_valid();
    }

    inline bool view::is_valid(const bool deepCheck) const
    {
        if (m_path.is_empty())
        {
            return false;
        }

        if (deepCheck)
        {
            return prefetch_solution().reduce_and_discard();
        }

        return true;
    }

    inline bool view::exists() const
    {
        // TODO(Glyn): kindah wastefull
        return file_info().exists;
    }

    inline bool view::is_file() const
    {
        // TODO(Glyn): kindah wastefull
        return file_info().isFile;
    }

    inline bool view::is_directory() const
    {
        // TODO(Glyn): kindah wastefull
        return file_info().isDirectory;
    }

    inline bool view::is_empty() const
    {
        // TODO(Glyn): kindah wastefull
        return file_info().isEmpty;
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

    template <invocable<void(view&)> Func>
    void view::iterate_recursive(Func&& func, bool reportErrors) const
    {
        result<view_list> entries = ls();
        if (entries.has_errors())
        {
            if (reportErrors)
            {
                entries.report_errors_and_resolve();
            }
            else
            {
                entries.resolve();
            }
        }

        if (!entries.carries_value())
        {
            return;
        }

        for (view& entry : *entries)
        {
            if (entry.is_directory())
            {
                entry.iterate_recursive(rsl::forward<Func>(func), reportErrors);
            }

            func(entry);
        }
    }

    constexpr view_list::view_list(const dynamic_array<fs::view>& src) noexcept
        : dynamic_array<fs::view>(src)
    {}
    constexpr view_list::view_list(dynamic_array<fs::view>&& src) noexcept
        : dynamic_array<fs::view>(rsl::move(src))
    {}
} // namespace rsl::fs
