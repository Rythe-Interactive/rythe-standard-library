#pragma once

#include "../util/primitives.hpp"

#include "../threading/thread.hpp"
#include "../threading/thread_id.hpp"
#include "../util/error_handling.hpp"
#include "../util/range.hpp"

#include "directory_iterator.hpp"
#include "dynamic_library.hpp"
#include "file.hpp"
#include "platform_error.hpp"

namespace rsl
{
    class thread;

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
        static result<iterator_view<directory_iterator>> iterate_directory(string_view absolutePath);
        static iterator_view<directory_iterator> iterate_directory(string_view absolutePath, platform_error& errc);
        static bool next_directory_entry(directory_iterator& iter);
        static result<dynamic_array<dynamic_string>> enumerate_directory(string_view absolutePath);

        static result<file> open_file(string_view absolutePath, file_access_mode mode, file_access_flags flags = file_access_flags::no_preference);
        static void close_file(file& file);
        static result<size_type> read_file_section(file file, mutable_byte_view target, byte_range range); // Read until end of range, or EOF.
        static result<size_type> read_file(file file, mutable_byte_view target, size_type offset = npos); // Read until EOF.
        static result<void> write_file(file file, byte_view data, size_type offset = npos);
        static result<void> append_file(file file, byte_view data);
        static result<void> truncate_file(file file, size_type offset = npos);
        static result<void> set_file_pointer(file file, diff_type offset); // negative is offset from file end.
        static result<size_type> get_file_pointer(file file);
        static result<void> skip_bytes(file file, size_type offset);
        static result<void> delete_file(file file, file_delete_flags flags = file_delete_flags::none);
        static result<uint64> get_file_size(string_view absolutePath);
        static result<uint64> get_file_size(file file);
        static result<void> rename_file(string_view oldAbsolutePath, string_view newAbsolutePath);
        static result<void> delete_file(string_view absolutePath, file_delete_flags flags = file_delete_flags::none);

        static result<file_info> get_file_info(string_view absolutePath) noexcept;
        static result<file_info> get_file_info(file file) noexcept;
    };
} // namespace rsl

#include "platform.inl"
