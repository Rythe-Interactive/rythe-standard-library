#pragma once

namespace rsl
{
    inline dynamic_string directory_entry::get_path() const
    {
        return m_directoryIterator->get_path();
    }

    inline bool directory_entry::is_regular_file() const
    {
        return m_directoryIterator->is_regular_file();
    }

    inline bool directory_entry::is_directory() const
    {
        return m_directoryIterator->is_directory();
    }

    inline bool directory_entry::is_writable() const
    {
        return m_directoryIterator->is_writable();
    }

    inline result<file> directory_entry::open_file(const file_access_mode mode, const file_access_flags flags) const
    {
        return m_directoryIterator->open_file(mode, flags);
    }

    template <typename T>
    T dynamic_library::get_symbol(const cstring symbolName) const
    {
        return bit_cast<T>(platform::get_symbol(*this, symbolName));
    }

    inline void dynamic_library::release()
    {
        platform::release_library(*this);
        m_handle = native_dynamic_library::invalid;
    }
}
