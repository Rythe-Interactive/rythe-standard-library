#include "../../defines.hpp"

#if RYTHE_PLATFORM_EXAMPLE

#include "../platform.hpp"

#include "../../containers/string.hpp"
#include "../../filesystem/filesystem_error.hpp"
#include "../../filesystem/path_util.hpp"
#include "../../threading/current_thread.inl"
#include "../../threading/thread.hpp"
#include "../../util/enum_flags.hpp"

namespace rsl
{
    NATIVE_API_TYPE_ACCESSORS(dynamic_library, void*)
    NATIVE_API_TYPE_ACCESSORS(directory_iterator, void*)
    NATIVE_API_TYPE_ACCESSORS(dynamic_library, void*)
    NATIVE_API_TYPE_ACCESSORS(thread, void*)

    [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_mode(file& val, const file_access_mode mode) noexcept
    {
        val.m_accessMode = mode;
    }

   [[maybe_unused]] [[rythe_always_inline]] static void set_file_access_flags(file& val, const file_access_flags flags) noexcept
    {
        val.m_accessFlags = flags;
    }

	dynamic_library platform::load_library(cstring path)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
	}

	void platform::release_library(const dynamic_library library)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
	}

	void* platform::get_symbol(const dynamic_library library, cstring symbolName)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return nullptr;
	}

    bool platform::is_debugger_attached()
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    thread platform::create_thread(
                const native_thread_start startFunction,
                void* userData,
                const string_view name,
                pmu_allocator& allocator
                )
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
    }

    uint32 platform::destroy_thread(const thread thread)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return 1u;
    }

    bool platform::is_thread_active(const thread thread)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

	thread_id platform::get_current_thread_id()
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
	}

	void platform::yield_current_thread()
    {
        // TODO: Implement
        rsl_assert_unimplemented();
	}

	void platform::sleep_current_thread(const uint32 milliseconds)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
	}

    void platform::set_thread_name(const thread_id threadId, const string_view name)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
    }

    string_view platform::get_thread_name(const thread_id threadId)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
    }

    dynamic_array<dynamic_string> platform::enumerate_drives()
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
    }

    bool platform::is_path_valid(const string_view absolutePath)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool platform::is_path_readable(const string_view absolutePath)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool platform::is_file_writable(const string_view absolutePath)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool platform::is_file_readable(string_view absolutePath)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool platform::does_path_entry_exist(const string_view absolutePath)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    iterator_view<directory_iterator> platform::iterate_directory(const string_view absolutePath, platform_error& errc)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return {};
    }

    bool platform::next_directory_entry(directory_iterator& iter)
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    result<dynamic_array<dynamic_string>> platform::enumerate_directory(const string_view absolutePath)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<file> platform::open_file(const string_view absolutePath, const file_access_mode mode, const file_access_flags flags)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<size_type> platform::read_file_section([[maybe_unused]] file file, [[maybe_unused]] mutable_byte_view target, [[maybe_unused]] byte_range range)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<size_type> platform::read_file([[maybe_unused]] file file, [[maybe_unused]] mutable_byte_view target, [[maybe_unused]] size_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::write_file(const file file, const byte_view data, const size_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::truncate_file([[maybe_unused]] file file, [[maybe_unused]] size_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::set_file_pointer([[maybe_unused]] file file, [[maybe_unused]] diff_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<size_type> platform::get_file_pointer([[maybe_unused]] file file)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::skip_bytes([[maybe_unused]] file file, [[maybe_unused]] size_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::delete_file([[maybe_unused]] file file, [[maybe_unused]] file_delete_flags flags)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<uint64> platform::get_file_size([[maybe_unused]] string_view absolutePath)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<uint64> platform::get_file_size([[maybe_unused]] file file)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::rename_file([[maybe_unused]] string_view oldAbsolutePath, [[maybe_unused]] string_view newAbsolutePath)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::delete_file([[maybe_unused]] string_view absolutePath, [[maybe_unused]] file_delete_flags flags)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<file_info> platform::get_file_info(const string_view absolutePath) noexcept
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<file_info> platform::get_file_info(const file file) noexcept
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    bool file::operator==(const file& other) const
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return false;
    }

    void file::close()
    {
        // TODO: implement
        rsl_assert_unimplemented();
    }

    directory_iterator::directory_iterator(directory_iterator&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = native_directory_iterator::invalid;
    }

    directory_iterator::directory_iterator(const directory_iterator& other) noexcept
    {
        // TODO: implement
        rsl_assert_unimplemented();
    }

    directory_iterator& directory_iterator::operator=(directory_iterator&& other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = native_directory_iterator::invalid;
        return *this;
    }

    directory_iterator& directory_iterator::operator=(const directory_iterator& other) noexcept
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return *this;
    }

    directory_iterator::~directory_iterator()
    {
        // TODO: implement
        rsl_assert_unimplemented();
    }

    dynamic_string directory_iterator::get_path() const
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return {};
    }

    bool directory_iterator::is_regular_file() const
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool directory_iterator::is_directory() const
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    bool directory_iterator::is_writable() const
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    result<file> directory_iterator::open_file(const file_access_mode mode, const file_access_flags flags) const
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    bool directory_iterator::operator==(const directory_iterator& other) const
    {
        // TODO: Implement
        rsl_assert_unimplemented();
        return false;
    }

    const directory_entry& directory_iterator::operator*() const
    {
        return m_entry;
    }

    const directory_entry* directory_iterator::operator->() const
    {
        return &m_entry;
    }

    directory_iterator directory_iterator::operator++(int)
    {
        directory_iterator result = *this;
        platform::next_directory_entry(*this);
        return result;
    }

    directory_iterator& directory_iterator::operator++()
    {
        platform::next_directory_entry(*this);
        return *this;
    }

    bool dynamic_library::operator==(const dynamic_library& other) const
    {
        return m_handle == other.m_handle;
    }
} // namespace rsl

#endif
