#pragma once

#include "platform.hpp"
#include "../containers/string.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"

#include "../threading/thread_id.hpp"
#include "../time/date_time.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
    class dynamic_library;
    class thread;
    class file;
    class directory_iterator;

    struct file_info
    {
        time::date lastWriteTimestamp;
        size_type size;
        bool isWritable;
        bool isDirectory;
        bool isFile;
    };

    enum struct [[rythe_closed_enum]] file_access_mode : uint8
    {
        read,
        exclusive_read,
        write_shared_read,
        write,
        append,
        read_write_append,
    };

    enum struct [[rythe_open_enum]] file_access_flags : uint8
    {
        no_preference = 0,
        async = 1 << 0,
        random = 1 << 1,
        sequential = 1 << 2,
    };
    RYTHE_BIT_FLAG_OPERATORS(file_access_flags)

    enum struct [[rythe_open_enum]] file_delete_flags : uint8
    {
        none      = 0,
        force     = 1 << 0,
        recursive = 1 << 1,
    };
    RYTHE_BIT_FLAG_OPERATORS(file_delete_flags)

    class platform
    {
    public:
        using native_thread_start = uint32(*)(void* userData);

        static dynamic_library load_library(cstring path);
        static void release_library(dynamic_library library);

        static void* get_symbol(dynamic_library library, cstring symbolName);

        static bool is_debugger_attached();

        static thread create_thread(
                native_thread_start startFunction,
                void* userData = nullptr,
                string_view name = "unknown thread"_sv,
                pmu_allocator& allocator = *allocator_context::globalAllocator
                );
        static uint32 destroy_thread(thread thread);

        static bool is_thread_active(thread thread);

        static thread_id get_current_thread_id();
        static thread_id get_thread_id(thread thread);
        static void yield_current_thread();
        static void sleep_current_thread(uint32 milliseconds);

        static void set_thread_name(thread thread, string_view name);
        static void set_thread_name(thread_id threadId, string_view name);
        static string_view get_thread_name(thread thread);
        static string_view get_thread_name(thread_id threadId);

        static dynamic_array<dynamic_string> enumerate_drives();
        static bool is_path_valid(string_view absolutePath);
        static bool is_regular_file(string_view absolutePath);
        static bool is_directory(string_view absolutePath);
        static bool is_path_writable(string_view absolutePath);
        static bool is_path_readable(string_view absolutePath);
        static bool is_file_writable(string_view absolutePath);
        static bool is_file_readable(string_view absolutePath);
        static bool does_path_entry_exist(string_view absolutePath);
        static result<directory_iterator> iterate_directory(string_view absolutePath);
        static bool next_directory_entry(directory_iterator& iter);
        static result<dynamic_array<dynamic_string>> enumerate_directory(string_view absolutePath);

        static result<file> open_file(string_view absolutePath, file_access_mode mode, file_access_flags flags = file_access_flags::no_preference);
        static result<void> close_file(file file);
        static result<size_type> read_file_section(file file, array_view<byte> target, size_type amountOfBytes, size_type offset = 0ull);
        static result<void> read_file(file file, array_view<byte> target, size_type offset = 0ull);
        static result<size_type> write_file(file file, size_type offset, array_view<const byte> data);
        static result<void> write_all_to_file(file file, size_type offset, array_view<const byte> data);
        static result<void> truncate_file(file file, size_type offset = 0ull);
        static result<uint64> get_file_size(string_view absolutePath);
        static result<uint64> get_file_size(file file);
        static result<void> flush_file_write_buffer(file file);
        static result<void> rename_file(string_view oldAbsolutePath, string_view newAbsolutePath);
        static result<void> delete_file(string_view absolutePath, file_delete_flags flags = file_delete_flags::none);

        static result<file_info> get_file_info(string_view absolutePath) noexcept;
        static result<file_info> get_file_info(file file) noexcept;
    };

    #if !defined(RYTHE_NATIVE_FILE_HANDLE_IMPL)
    #define RYTHE_NATIVE_FILE_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE)
    #define RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class file
    {
        using platform_specific_handle = RYTHE_NATIVE_FILE_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_NATIVE_FILE_HANDLE_DEFAULT_VALUE;

        friend class platform;

    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(file);

        bool operator==(const file& other) const;
        bool operator!=(const file& other) const { return !(*this == other); }

        [[nodiscard]] [[rythe_always_inline]] file_access_mode get_mode() const noexcept { return m_accessMode; }
        [[nodiscard]] [[rythe_always_inline]] file_access_flags get_flags() const noexcept { return m_accessFlags; }

    private:
        file_access_mode m_accessMode;
        file_access_flags m_accessFlags;
    };

    #if !defined(RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL)
    #define RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE)
    #define RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class directory_iterator
    {
        using platform_specific_handle = RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE;

        friend class platform;
    public:
        constexpr directory_iterator() noexcept = default;
        directory_iterator(directory_iterator&& other) noexcept;
        directory_iterator& operator=(directory_iterator&& other) noexcept;
        ~directory_iterator();

        result<dynamic_string> get_path() const;

        bool operator==(const directory_iterator& other) const;
        bool operator!=(const directory_iterator& other) const { return !(*this == other); }
    };

    constexpr directory_iterator directory_end = {};

    #if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL)
    #define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE)
    #define RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class dynamic_library
    {
    public:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(dynamic_library);

        bool operator==(const dynamic_library& other) const;
        bool operator!=(const dynamic_library& other) const { return !(*this == other); }

        template <typename T>
        T get_symbol(const cstring symbolName) const
        {
            return bit_cast<T>(platform::get_symbol(*this, symbolName));
        }

        operator bool() const { return m_handle; }

        void release()
        {
            platform::release_library(*this);
            m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;
        }

    private:
        using platform_specific_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;

        friend class platform;
    };
} // namespace rsl
