#pragma once

namespace rsl::filesystem
{
    inline const archive* file_solution::get_provider() const noexcept
    {
        return m_provider;
    }

    inline archive* file_solution::get_provider() noexcept
    {
        return m_provider;
    }

    inline void file_solution::release()
    {
        if (!m_provider)
        {
            return;
        }

        m_provider->release_solution(this);
        m_provider = nullptr;
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

    inline filesystem_traits file_solution::filesystem_info() const
    {
        if (!m_provider)
        {
            return invalid_filesystem_traits;
        }

        return m_provider->filesystem_info();
    }

    inline result<void> file_solution::open_file_for_read() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->open_file_for_read(this);
    }

    inline result<void> file_solution::open_file_for_write()
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->open_file_for_write(this);
    }

    inline result<void> file_solution::close_file() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->close_file(this);
    }

    inline result<void> file_solution::flush_file() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        return m_provider->flush_file(this);
    }
}
