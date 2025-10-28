#pragma once

namespace rsl
{
    inline void platform::set_thread_name(const thread thread, const string_view name)
    {
        set_thread_name(get_thread_id(thread), name);
    }

    inline string_view platform::get_thread_name(const thread thread)
    {
        return get_thread_name(get_thread_id(thread));
    }

    inline bool platform::is_regular_file(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isFile;
    }

    inline bool platform::is_directory(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isDirectory;
    }

    inline bool platform::is_path_writable(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isWritable;
    }

    inline result<iterator_view<directory_iterator>> platform::iterate_directory(const string_view absolutePath)
    {
        platform_error errc;
        iterator_view<directory_iterator> result = iterate_directory(absolutePath, errc);
        if (errc != platform_error::no_error)
        {
            return make_error(errc);
        }
        return result;
    }

    inline void platform::close_file(file& file)
    {
        file.close();
    }

    inline result<void> platform::append_file(const file file, const byte_view data)
    {
        return write_file(file, data, npos);
    }

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
