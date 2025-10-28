#pragma once

namespace rsl
{
    constexpr bool mode_available_for_write(const file_access_mode mode) noexcept
    {
        return mode == file_access_mode::write || mode == file_access_mode::read_write_append;
    }

    constexpr bool mode_available_for_append(const file_access_mode mode) noexcept
    {
        return mode == file_access_mode::append || mode == file_access_mode::read_write_append;
    }

    constexpr bool mode_available_for_read(const file_access_mode mode) noexcept
    {
        return mode == file_access_mode::read || mode == file_access_mode::exclusive_read ||
                mode == file_access_mode::write_shared_read || mode == file_access_mode::read_write_append;
    }

    inline bool file::operator!=(const file& other) const
    {
        return !(*this == other);
    }

    inline file_access_mode file::get_mode() const noexcept
    {
        return m_accessMode;
    }

    inline file_access_flags file::get_flags() const noexcept
    {
        return m_accessFlags;
    }

    inline file::operator bool() const noexcept
    {
        return m_handle != native_file::invalid;
    }
}
