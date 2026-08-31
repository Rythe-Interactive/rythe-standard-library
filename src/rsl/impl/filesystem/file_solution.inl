namespace rsl::fs
{
    inline pointer<const archive> file_solution::get_provider() const noexcept
    {
        return m_provider;
    }

    inline pointer<archive> file_solution::get_provider() noexcept
    {
        return m_provider;
    }

    inline void file_solution::release()
    {
        if (!m_provider)
        {
            return;
        }

        m_provider->release_solution(m_handle);
        m_provider = { nullptr };
        m_handle = invalid_file_solution_handle;
    }

    inline bool file_solution::is_valid() const noexcept
    {
        return m_provider && m_handle != invalid_file_solution_handle;
    }

    inline file_solution::operator bool() const noexcept
    {
        return is_valid();
    }

    inline file_traits file_solution::file_info() const
    {
        if (!m_provider) [[unlikely]]
        {
            return invalid_file_traits;
        }

        const auto [isReadonly, isValid] = filesystem_info();

        if (!isValid) [[unlikely]]
        {
            return invalid_file_traits;
        }

        file_traits traits = invalid_file_traits;
        traits.isValidPath = is_valid_path();

        if (!traits.isValidPath)
        {
            return traits;
        }

        traits.isDirectory = is_directory();
        traits.isFile = !traits.isDirectory && is_file();
        traits.isEmpty = is_empty();
        traits.canBeRead = can_be_read();
        traits.exists = exists();

        if (isReadonly)
        {
            return traits;
        }

        traits.canBeWritten = can_be_written();
        traits.canBeCreated = !traits.exists && can_be_created();

        return traits;
    }

    inline bool file_solution::is_file() const
    {
        return m_provider && m_provider->is_file(m_handle);
    }

    inline bool file_solution::is_directory() const
    {
        return m_provider && m_provider->is_directory(m_handle);
    }

    inline bool file_solution::is_empty() const
    {
        return m_provider && m_provider->is_empty(m_handle);
    }

    inline bool file_solution::is_valid_path() const
    {
        return m_provider && m_provider->is_valid_path(m_handle);
    }

    inline bool file_solution::can_be_written() const
    {
        return m_provider && m_provider->can_be_written(m_handle);
    }

    inline bool file_solution::can_be_read() const
    {
        return m_provider && m_provider->can_be_read(m_handle);
    }

    inline bool file_solution::can_be_created() const
    {
        return m_provider && m_provider->can_be_created(m_handle);
    }

    inline bool file_solution::exists() const
    {
        return m_provider && m_provider->exists(m_handle);
    }

    inline filesystem_traits file_solution::filesystem_info() const
    {
        return m_provider ? m_provider->filesystem_info() : invalid_filesystem_traits;
    }

    inline void file_solution::set_access_hint(file_access_flags flags)
    {
        if (m_provider)
        {
            m_provider->set_access_hint(m_handle, flags);
        }
    }

    inline result<void> file_solution::create() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->create(m_handle);
    }

    inline result<void> file_solution::delete_entry(file_delete_flags flags) const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->delete_entry(m_handle, flags);
    }

    inline result<byte_view> file_solution::read() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->read(m_handle);
    }

    inline result<void> file_solution::write(byte_view data)
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->write(m_handle, data);
    }

    inline result<void> file_solution::append(byte_view data)
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->append(m_handle, data);
    }

    inline result<void> file_solution::flush() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->flush(m_handle);
    }
} // namespace rsl::fs
