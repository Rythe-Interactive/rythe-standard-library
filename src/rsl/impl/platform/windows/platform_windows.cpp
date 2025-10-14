// ReSharper disable CppWrongIncludesOrder
// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "../../defines.hpp"

#if RYTHE_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOMINMAX
#define NOMCX
#define NOSERVICE
#define NOHELP
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#include "winplatformdef.h"
#include <libloaderapi.h>
#include <windef.h>
#include <winbase.h>
#include <processthreadsapi.h>
#include <process.h>

#include "../platform.hpp"

#include "../../containers/string.hpp"
#include "../../filesystem/filesystem_error.hpp"
#include "../../filesystem/path_util.hpp"
#include "../../threading/current_thread.inl"
#include "../../threading/thread.hpp"
#include "../../util/enum_flags.hpp"

namespace rsl
{
    struct native_win_directory_iterator_handle
    {
        managed_resource<HANDLE> directory;
        WIN32_FIND_DATAW findData;
    };

    NATIVE_API_TYPE_ACCESSORS(file, HANDLE)
    NATIVE_API_TYPE_ACCESSORS(directory_iterator, native_win_directory_iterator_handle*)
    NATIVE_API_TYPE_ACCESSORS(dynamic_library, HMODULE)
    NATIVE_API_TYPE_ACCESSORS(thread, HANDLE)

   [[rythe_always_inline]] static void set_file_access_mode(file& val, const file_access_mode mode) noexcept
    {
        val.m_accessMode = mode;
    }

    [[rythe_always_inline]] static void set_file_access_flags(file& val, const file_access_flags flags) noexcept
    {
        val.m_accessFlags = flags;
    }

    namespace
    {

        bool is_regular_file_attributes(const DWORD attributes)
        {
            constexpr static DWORD excludingAttributes =
                    FILE_ATTRIBUTE_DIRECTORY |
                    FILE_ATTRIBUTE_ARCHIVE |
                    FILE_ATTRIBUTE_DEVICE |
                    FILE_ATTRIBUTE_REPARSE_POINT;

            return (attributes & excludingAttributes) == 0u;
        }

        struct native_thread_name
        {
            dynamic_wstring wideName;
            dynamic_string name;
        };

        dynamic_map<thread_id, native_thread_name> thread_names;

        struct native_thread_context
        {
            pmu_allocator* allocator;
            dynamic_string name;
            platform::native_thread_start function;
            void* userData;
        };

        DWORD internal_native_thread_start(void* args)
        {
            native_thread_context& context = *static_cast<native_thread_context*>(args);
            pmu_allocator* allocator = context.allocator;

            current_thread::set_name(context.name);

            const uint32 result = context.function(context.userData);

            internal::default_destroy<native_thread_context>(&context, 1);
            allocator->deallocate(&context);

            return result;
        }

        platform_error translate_platform_error(const uint32 error) noexcept
        {
            switch (error)
            {
                case ERROR_SUCCESS: return platform_error::no_error;
                case ERROR_FILE_NOT_FOUND: return platform_error::file_not_found;
                case ERROR_PATH_NOT_FOUND: return platform_error::file_not_found;
                case ERROR_ACCESS_DENIED: return platform_error::no_permission;
                case ERROR_INVALID_HANDLE: return platform_error::invalid_argument;
                case ERROR_NOT_ENOUGH_MEMORY: return platform_error::out_of_memory;
                case ERROR_OUTOFMEMORY: return platform_error::out_of_memory;
                case ERROR_HANDLE_EOF: return platform_error::eof_reached;
                case ERROR_ALREADY_EXISTS: return platform_error::already_exists;
                case ERROR_DISK_FULL: return platform_error::disk_full;
                case ERROR_NOT_SUPPORTED: return platform_error::not_supported;
                case ERROR_BROKEN_PIPE: return platform_error::broken_pipe;
                case ERROR_INVALID_NAME: return platform_error::invalid_argument;
                case STILL_ACTIVE: return platform_error::temporary_still_running;
                case ERROR_DIR_NOT_EMPTY: return platform_error::directory_not_empty;
                case ERROR_SHARING_VIOLATION: return platform_error::sharing_violation;
                case ERROR_OPERATION_ABORTED: return platform_error::canceled;
                case ERROR_IO_PENDING: return platform_error::temporary_time_out;
                case ERROR_BUFFER_OVERFLOW: return platform_error::buffer_too_small;
                case ERROR_TOO_MANY_LINKS: return platform_error::too_many_links;
                case ERROR_NO_NETWORK: return platform_error::network_down;
                case ERROR_CONNECTION_REFUSED: return platform_error::connection_refused;
                case ERROR_ADDRESS_ALREADY_ASSOCIATED: return platform_error::address_in_use;
                case ERROR_ADDRESS_NOT_ASSOCIATED: return platform_error::address_not_available;
                case ERROR_CONNECTION_INVALID: return platform_error::no_connection;
                case ERROR_NETWORK_UNREACHABLE: return platform_error::network_unreachable;
                case ERROR_GRACEFUL_DISCONNECT: return platform_error::connection_closed;
                case ERROR_FILENAME_EXCED_RANGE: return platform_error::path_too_long;
                default: return platform_error::generic_error;
            }
        }

        uint64 combine_dwords(const DWORD lower, const DWORD upper) noexcept
        {
            return (static_cast<uint64>(upper) << 32ull) | static_cast<uint64>(lower);
        }

        pair<DWORD, DWORD> split_dwords(const uint64 value) noexcept
        {
            return { static_cast<DWORD>(value & 0xFFFFFFFFu), static_cast<DWORD>((value >> 32ull) & 0xFFFFFFFFu) };
        }

        time::date translate_timestamp(const FILETIME fileTime) noexcept
        {
            const uint64 windowsTime = combine_dwords(fileTime.dwLowDateTime, fileTime.dwHighDateTime);
            return { .epochTime = static_cast<int64>(windowsTime / 10000000ull - 11644473600ull) };
        }

        result<file> open_file_impl(const wstring_view absolutePath, const file_access_mode mode, const file_access_flags flags)
        {
            DWORD accessMode;
            DWORD creationDisposition;
            DWORD shareMode = FILE_SHARE_READ;
            DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

            switch (mode)
            {
                case file_access_mode::read:
                {
                    accessMode = GENERIC_READ;
                    creationDisposition = OPEN_EXISTING;
                    shareMode |= FILE_SHARE_DELETE;
                    break;
                }
                case file_access_mode::write_shared_read:
                {
                    accessMode = GENERIC_READ;
                    creationDisposition = OPEN_EXISTING;
                    shareMode |= FILE_SHARE_WRITE | FILE_SHARE_DELETE;
                    break;
                }
                case file_access_mode::exclusive_read:
                {
                    accessMode = GENERIC_READ | GENERIC_WRITE;
                    creationDisposition = OPEN_EXISTING;
                    break;
                }
                case file_access_mode::write:
                {
                    accessMode = GENERIC_WRITE;
                    creationDisposition = CREATE_ALWAYS;
                    break;
                }
                case file_access_mode::append:
                {
                    accessMode = GENERIC_WRITE;
                    creationDisposition = OPEN_ALWAYS;
                    break;
                }
                case file_access_mode::read_write_append:
                {
                    accessMode = GENERIC_READ | GENERIC_WRITE;
                    creationDisposition = OPEN_ALWAYS;
                    break;
                }
                default:
                {
                    return make_error(platform_error::invalid_argument, "Unsupported file_access_mode.");
                }
            }

            if (enum_flags::has_flag(flags, file_access_flags::async))
            {
                flagsAndAttributes |= FILE_FLAG_OVERLAPPED;
            }

            if (enum_flags::has_flag(flags, file_access_flags::random))
            {
                flagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
            }

            if (enum_flags::has_flag(flags, file_access_flags::sequential))
            {
                flagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            }

            HANDLE fileHandle = CreateFileW(
                    absolutePath.data(),
                    accessMode,
                    shareMode,
                    NULL,
                    creationDisposition,
                    flagsAndAttributes,
                    NULL
                    );

            if (fileHandle == INVALID_HANDLE_VALUE)
            {
                return make_error(translate_platform_error(GetLastError()));
            }

            if (mode == file_access_mode::append ||
                mode == file_access_mode::read_write_append)
            {
                LARGE_INTEGER largePosition;
                largePosition.QuadPart = 0u;
                if (!SetFilePointerEx(fileHandle, largePosition, nullptr, FILE_END))
                {
                    const platform_error error = translate_platform_error(GetLastError());
                    CloseHandle(fileHandle);
                    return make_error(error);
                }
            }

            file result;
            set_native_handle(result, fileHandle);
            set_file_access_mode(result, mode);
            set_file_access_flags(result, flags);

            return result;
        }

        result<size_type> write_file_impl(const HANDLE fileHandle, const byte_view data, const size_type offset)
        {
            auto [low, high] = split_dwords(offset);
            OVERLAPPED overlapped;
            overlapped.Offset = low;
            overlapped.OffsetHigh = high;

            DWORD bytesWritten = 0;
            if (!WriteFile(fileHandle, data.data(), static_cast<uint32>(data.size()), &bytesWritten, &overlapped))
            {
                const platform_error error = translate_platform_error(GetLastError());
                if (error != platform_error::no_error)
                {
                    return make_error(error);
                }
            }
            return static_cast<size_type>(bytesWritten);
        }
    }

    dynamic_library platform::load_library(const cstring path)
    {
        dynamic_library result;
        set_native_handle(result, LoadLibraryA(path));
        return result;
    }

    void platform::release_library(const dynamic_library library)
    {
        FreeLibrary(get_native_handle(library));
    }

    void* platform::get_symbol(const dynamic_library library, const cstring symbolName)
    {
        return bit_cast<void*>(GetProcAddress(get_native_handle(library), symbolName));
    }

    bool platform::is_debugger_attached()
    {
        return IsDebuggerPresent() == TRUE;
    }

    thread platform::create_thread(
            const native_thread_start startFunction,
            void* userData,
            const string_view name,
            pmu_allocator& allocator
            )
    {
        rsl_assert_always(startFunction);

        thread result;

        native_thread_context* threadContext = allocator.allocate<native_thread_context>();
        if (!threadContext)
        {
            return result;
        }

        internal::default_construct<native_thread_context>(threadContext, 1);

        threadContext->allocator = &allocator;
        threadContext->name = dynamic_string::from_view(name);
        threadContext->function = startFunction;
        threadContext->userData = userData;

        const HANDLE threadHandle = CreateThread(
                nullptr,
                0,
                &internal_native_thread_start,
                threadContext,
                CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION,
                nullptr
                );

        if (!threadHandle)
        {
            internal::default_destroy<native_thread_context>(threadContext, 1);
            allocator.deallocate(threadContext);
            return result;
        }

        set_native_handle(result, threadHandle);

        ResumeThread(threadHandle);

        return result;
    }

    uint32 platform::destroy_thread(const thread thread)
    {
        const HANDLE threadHandle = get_native_handle(thread);
        WaitForSingleObject(threadHandle, INFINITE);

        DWORD exitCode = 0u;
        GetExitCodeThread(threadHandle, &exitCode);

        CloseHandle(threadHandle);

        return static_cast<uint32>(exitCode);
    }

    bool platform::is_thread_active(const thread thread)
    {
        const HANDLE threadHandle = get_native_handle(thread);
        if (!threadHandle)
        {
            return false;
        }

        const DWORD waitResult = WaitForSingleObject(threadHandle, 0);
        DWORD exitCode = 0u;
        const bool exitCodeResult = GetExitCodeThread(threadHandle, &exitCode);

        const bool threadIsInactive = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_FAILED) && (
            exitCode != STILL_ACTIVE || !exitCodeResult);
        return !threadIsInactive;
    }

    thread_id platform::get_current_thread_id()
    {
        return thread_id{ .nativeId = static_cast<id_type>(GetCurrentThreadId()) };
    }

    thread_id platform::get_thread_id(const thread thread)
    {
        return thread_id{ .nativeId = static_cast<id_type>(GetThreadId(get_native_handle(thread))) };
    }

    void platform::yield_current_thread()
    {
        SwitchToThread();
    }

    void platform::sleep_current_thread(const uint32 milliseconds)
    {
        Sleep(milliseconds);
    }

    void platform::set_thread_name(const thread thread, const string_view name)
    {
        set_thread_name(thread.get_id(), name);
    }

    void platform::set_thread_name(const thread_id threadId, const string_view name)
    {
        dynamic_wstring& wideName = thread_names.emplace_or_replace(
                threadId,
                native_thread_name{ .wideName = to_utf16(name), .name = dynamic_string::from_view(name) }
                ).wideName;
        [[maybe_unused]] HRESULT _ = SetThreadDescription(GetCurrentThread(), wideName.data());
    }

    string_view platform::get_thread_name(const thread thread)
    {
        return get_thread_name(thread.get_id());
    }

    string_view platform::get_thread_name(const thread_id threadId)
    {
        if (native_thread_name* result = thread_names.find(threadId))
        {
            return result->name;
        }

        native_thread_name nativeThreadName;
        nativeThreadName.name = to_string(threadId.nativeId);
        nativeThreadName.wideName = to_utf16(nativeThreadName.name);

        return thread_names.emplace(threadId, move(nativeThreadName)).name;
    }

    dynamic_array<dynamic_string> platform::enumerate_drives()
    {
        char buffer[512];
        const DWORD charCount = GetLogicalDriveStringsA(512, buffer);
        if (!charCount)
        {
            return {};
        }

        dynamic_array<dynamic_string> result;
        const char* start = buffer;
        size_type count = 0ull;
        for (size_type i = 0; i < charCount; i++)
        {
            if (buffer[i] == '\0')
            {
                result.push_back(dynamic_string::from_buffer(start, count));
                start += count + 1ull;
                count = 0ull;
            }
            else
            {
                ++count;
            }
        }

        return result;
    }

    bool platform::is_path_valid(const string_view absolutePath)
    {
        if (linear_search_collection(absolutePath, "<>\"|?*"_sv) != npos)
        {
            return false;
        }

        if (absolutePath.back() == ' ' || absolutePath.back() == '.')
        {
            return false;
        }

        if (linear_search_collection(
                absolutePath,
                "\x00\x01\x02\x03\x04\x05\x06\x07\x09\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x30\x31"_sv
                ) != npos)
        {
            return false;
        }

        return true;
    }

    bool platform::is_regular_file(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isFile;
    }

    bool platform::is_directory(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isDirectory;
    }

    bool platform::is_path_writable(const string_view absolutePath)
    {
        result<file_info> result = get_file_info(absolutePath);
        if (!result.reduce_and_discard())
        {
            return false;
        }

        return result->isWritable;
    }

    bool platform::is_path_readable(const string_view absolutePath)
    {
        return get_file_info(absolutePath).reduce_and_discard();
    }

    bool platform::is_file_writable(const string_view absolutePath)
    {
        if (!is_path_valid(absolutePath))
        {
            return false;
        }

        if (is_regular_file(absolutePath))
        {
            const DWORD attr = GetFileAttributesA(absolutePath.data());
            if (attr != INVALID_FILE_ATTRIBUTES)
            {
                if (attr & FILE_ATTRIBUTE_READONLY)
                {
                    return false;
                }
                const HANDLE h = CreateFileA(absolutePath.data(),GENERIC_WRITE,NULL,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); // NOLINT
                if (h == INVALID_HANDLE_VALUE)
                {
                    if (GetLastError() == ERROR_SHARING_VIOLATION)
                    {
                        return false;
                    }
                }
                CloseHandle(h);
            }
            return true;
        }

        if (is_directory(absolutePath) && does_path_entry_exist(absolutePath))
        {
            return false;
        }

        return true;
    }

    bool platform::is_file_readable(string_view absolutePath)
    {
        if (!does_path_entry_exist(absolutePath))
        {
            return false;
        }

        const HANDLE h = CreateFileA(absolutePath.data(),GENERIC_READ,NULL,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL); // NOLINT
        if (h == INVALID_HANDLE_VALUE)
        {
            if (GetLastError() == ERROR_SHARING_VIOLATION)
            {
                return false;
            }
        }
        CloseHandle(h);

        return true;
    }

    bool platform::does_path_entry_exist(const string_view absolutePath)
    {
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));
        return GetFileAttributesW(widePath.data()) != INVALID_FILE_ATTRIBUTES;
    }

    result<iterator_view<directory_iterator>> platform::iterate_directory(const string_view absolutePath)
    {
        platform_error errc;
        iterator_view<directory_iterator> result = iterate_directory(absolutePath, errc);
        if (errc != platform_error::no_error)
        {
            return make_error(errc);
        }
        return result;
    }

    iterator_view<directory_iterator> platform::iterate_directory(const string_view absolutePath, platform_error& errc)
    {
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        WIN32_FIND_DATAW findData;
        const HANDLE directory = FindFirstFileW(widePath.data(), &findData);

        const platform_error error = translate_platform_error(GetLastError());
        if (error != platform_error::no_error)
        {
            FindClose(directory);
            errc = error;
            return {};
        }

        errc = platform_error::no_error;

        directory_iterator startIterator;
        set_native_handle(
                startIterator,
                new native_win_directory_iterator_handle{ .directory = managed_resource<HANDLE>(FindClose, directory),
                                                          .findData = findData }
                );

        if (!next_directory_entry(startIterator))
        {
            return {};
        }

        return iterator_view(startIterator, directory_end);
    }

    bool platform::next_directory_entry(directory_iterator& iter)
    {
        if (iter == directory_end)
        {
            return false;
        }

        auto& [directory, findData] = *get_native_handle(iter);

        if (!FindNextFileW(*directory, &findData))
        {
            iter = directory_end;
            return false;
        }

        return true;
    }

    result<dynamic_array<dynamic_string>> platform::enumerate_directory(const string_view absolutePath)
    {
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        dynamic_array<dynamic_string> result;

        WIN32_FIND_DATAW findData;
        const HANDLE directory = FindFirstFileW(widePath.data(), &findData);

        while (FindNextFileW(directory, &findData))
        {
            result.push_back(fs::standardize(to_utf8(findData.cFileName)));
        }

        const platform_error error = translate_platform_error(GetLastError());

        FindClose(directory);

        if (error != platform_error::no_error)
        {
            return make_error(error);
        }

        return result;
    }

    result<file> platform::open_file(const string_view absolutePath, const file_access_mode mode, const file_access_flags flags)
    {
        return open_file_impl(to_utf16(fs::localize(absolutePath)), mode, flags);
    }

    void platform::close_file(file& file)
    {
        file.close();
    }

    result<size_type> platform::read_file_section([[maybe_unused]] file file, [[maybe_unused]] mutable_byte_view target, [[maybe_unused]] byte_range range)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::read_file([[maybe_unused]] file file, [[maybe_unused]] mutable_byte_view target, [[maybe_unused]] size_type offset)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<size_type> platform::write_file_section([[maybe_unused]] file file, [[maybe_unused]] byte_view data, [[maybe_unused]] byte_range range)
    {
        // TODO: implement
        rsl_assert_unimplemented();
        return error;
    }

    result<void> platform::write_file(const file file, const byte_view data, const size_type offset)
    {
        size_type bytesWritten = 0ull;
        size_type writeOffset = offset;
        while (bytesWritten < data.size())
        {
            result<size_type> result = write_file_impl(get_native_handle(file), data, writeOffset);

            if (result.has_errors())
            {
                return result.propagate();
            }

            bytesWritten += result.value();
            writeOffset += result.value();
        }

        return okay;
    }

    result<void> platform::truncate_file([[maybe_unused]] file file, [[maybe_unused]] size_type offset)
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
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        const HANDLE fileHandle = CreateFileW(
                widePath.data(),
                0u,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS,
                NULL
                );
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            return make_error(translate_platform_error(GetLastError()));
        }

        BY_HANDLE_FILE_INFORMATION information;
        if (GetFileInformationByHandle(fileHandle, &information) == FALSE)
        {
            const platform_error error = translate_platform_error(GetLastError());
            CloseHandle(fileHandle);

            return make_error(error);
        }
        CloseHandle(fileHandle);

        return file_info{
                    .lastWriteTimestamp = translate_timestamp(information.ftLastWriteTime),
                    .size = combine_dwords(information.nFileSizeLow, information.nFileSizeHigh),
                    .isWritable = (information.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0u,
                    .isDirectory = (information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0u,
                    .isFile = is_regular_file_attributes(information.dwFileAttributes),
                };
    }

    result<file_info> platform::get_file_info(const file file) noexcept
    {
        BY_HANDLE_FILE_INFORMATION information;
        if (GetFileInformationByHandle(get_native_handle(file), &information) == FALSE)
        {
            return make_error(translate_platform_error(GetLastError()));
        }

        return file_info{
                    .lastWriteTimestamp = translate_timestamp(information.ftLastWriteTime),
                    .size = combine_dwords(information.nFileSizeLow, information.nFileSizeHigh),
                    .isWritable = (information.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0u,
                    .isDirectory = (information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0u,
                    .isFile = is_regular_file_attributes(information.dwFileAttributes),
                };
    }

    bool file::operator==(const file& other) const
    {
        return m_handle == other.m_handle;
    }

    void file::close()
    {
        CloseHandle(get_native_handle(*this));
        m_handle = native_file::invalid;
    }

    directory_iterator::directory_iterator(directory_iterator&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = native_directory_iterator::invalid;
    }

    directory_iterator::directory_iterator(const directory_iterator& other) noexcept
    {
        if(other.m_handle != native_directory_iterator::invalid)
        {
            set_native_handle(*this, new native_win_directory_iterator_handle(*get_native_handle(other)));
        }
    }

    directory_iterator& directory_iterator::operator=(directory_iterator&& other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = native_directory_iterator::invalid;
        return *this;
    }

    directory_iterator& directory_iterator::operator=(const directory_iterator& other) noexcept
    {
        native_win_directory_iterator_handle* nativeHandle = get_native_handle(*this);

        if (nativeHandle)
        {
            delete nativeHandle;
            m_handle = native_directory_iterator::invalid;
        }

        if (other.m_handle != native_directory_iterator::invalid)
        {
            set_native_handle(*this, new native_win_directory_iterator_handle(*get_native_handle(other)));
        }

        return *this;
    }

    directory_iterator::~directory_iterator()
    {
        native_win_directory_iterator_handle* nativeHandle = get_native_handle(*this);
        if (nativeHandle)
        {
            delete nativeHandle;
            m_handle = native_directory_iterator::invalid;
        }
    }

    dynamic_string directory_iterator::get_path() const
    {
        return fs::standardize(to_utf8(get_native_handle(*this)->findData.cFileName));
    }

    bool directory_iterator::is_regular_file() const
    {
        return is_regular_file_attributes(get_native_handle(*this)->findData.dwFileAttributes);
    }

    bool directory_iterator::is_directory() const
    {
        return (get_native_handle(*this)->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0u;
    }

    bool directory_iterator::is_writable() const
    {
        return (get_native_handle(*this)->findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0u;
    }

    result<file> directory_iterator::open_file(const file_access_mode mode, const file_access_flags flags) const
    {
        return open_file_impl(get_native_handle(*this)->findData.cFileName, mode, flags);
    }

    bool directory_iterator::operator==(const directory_iterator& other) const
    {
        return m_handle == other.m_handle;
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
