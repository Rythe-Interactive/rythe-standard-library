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

#define RYTHE_DYNAMIC_LIBRARY_HANDLE_IMPL HMODULE
#define RYTHE_NATIVE_FILE_HANDLE_IMPL HANDLE
#define RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL rsl::native_win_directory_iterator_handle*

namespace rsl
{
    struct native_win_directory_iterator_handle;
}

#include "../platform.hpp"
#include "../platform_error.hpp"

#define RYTHE_THREAD_HANDLE_IMPL HANDLE

#include "../../containers/string.hpp"
#include "../../filesystem/path_util.hpp"
#include "../../threading/current_thread.inl"
#include "../../threading/thread.hpp"
#include "../../util/enum_flags.hpp"

namespace rsl
{
    struct native_win_directory_iterator_handle
    {
        HANDLE directory;
        WIN32_FIND_DATAW findData;
    };

    namespace
    {
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
            return static_cast<uint64>(upper << 32ull) | static_cast<uint64>(lower);
        }

        time::date translate_timestamp(const FILETIME fileTime) noexcept
        {
            const uint64 windowsTime = combine_dwords(fileTime.dwLowDateTime, fileTime.dwHighDateTime);
            return { .epochTime = static_cast<int64>(windowsTime / 10000000ull - 11644473600ull) };
        }
    }

    dynamic_library platform::load_library(const cstring path)
    {
        dynamic_library result;
        result.m_handle = ::LoadLibraryA(path);
        return result;
    }

    void platform::release_library(const dynamic_library library)
    {
        ::FreeLibrary(library.m_handle);
    }

    void* platform::get_symbol(const dynamic_library library, const cstring symbolName)
    {
        return bit_cast<void*>(::GetProcAddress(library.m_handle, symbolName));
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

        const HANDLE threadHandle = ::CreateThread(
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

        result.m_handle = threadHandle;

        ::ResumeThread(threadHandle);

        return result;
    }

    uint32 platform::destroy_thread(const thread thread)
    {
        ::WaitForSingleObject(thread.m_handle, INFINITE);

        DWORD exitCode = 0u;
        ::GetExitCodeThread(thread.m_handle, &exitCode);

        ::CloseHandle(thread.m_handle);

        return static_cast<uint32>(exitCode);
    }

    bool platform::is_thread_active(const thread thread)
    {
        if (!thread.m_handle)
        {
            return false;
        }

        const DWORD waitResult = ::WaitForSingleObject(thread.m_handle, 0);
        DWORD exitCode = 0u;
        const bool exitCodeResult = ::GetExitCodeThread(thread.m_handle, &exitCode);

        const bool threadIsInactive = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_FAILED) && (
            exitCode != STILL_ACTIVE || !exitCodeResult);
        return !threadIsInactive;
    }

    thread_id platform::get_current_thread_id()
    {
        return thread_id{ .nativeId = static_cast<id_type>(::GetCurrentThreadId()) };
    }

    thread_id platform::get_thread_id(const thread thread)
    {
        return thread_id{ .nativeId = static_cast<id_type>(::GetThreadId(thread.m_handle)) };
    }

    void platform::yield_current_thread()
    {
        ::SwitchToThread();
    }

    void platform::sleep_current_thread(const uint32 milliseconds)
    {
        ::Sleep(milliseconds);
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
        [[maybe_unused]] HRESULT _ = ::SetThreadDescription(::GetCurrentThread(), wideName.data());
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

        return thread_names.emplace(threadId, rsl::move(nativeThreadName)).name;
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
        return ::GetFileAttributesW(widePath.data()) != INVALID_FILE_ATTRIBUTES;
    }

    result<directory_iterator> platform::iterate_directory(const string_view absolutePath)
    {
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        WIN32_FIND_DATAW findData;
        const HANDLE directory = FindFirstFileW(widePath.data(), &findData);

        const platform_error error = translate_platform_error(GetLastError());
        if (error != platform_error::no_error)
        {
            FindClose(directory);
            return make_error(error);
        }

        directory_iterator result;
        result.m_handle = new native_win_directory_iterator_handle{ .directory = directory, .findData = findData };

        if (!next_directory_entry(result))
        {
            return {};
        }

        return result;
    }

    bool platform::next_directory_entry(directory_iterator& iter)
    {
        if (iter == directory_end)
        {
            return false;
        }

        auto& [directory, findData] = *iter.m_handle;

        if (!FindNextFileW(directory, &findData))
        {
            FindClose(directory);
            delete iter.m_handle;
            iter.m_handle = nullptr;
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
            bool hasEncodingError;
            result.push_back(fs::standardize(to_utf8(findData.cFileName, &hasEncodingError)));
            if (hasEncodingError)
            {
                FindClose(directory);
                return make_error(platform_error::encoding_error);
            }
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

        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        file result;
        result.m_accessMode = mode;
        result.m_accessFlags = flags;

        result.m_handle = CreateFileW(widePath.data(), accessMode, shareMode, NULL, creationDisposition, flagsAndAttributes, NULL);

        if (result.m_handle == INVALID_HANDLE_VALUE)
        {
            return make_error(translate_platform_error(GetLastError()));
        }

        if (mode == file_access_mode::append ||
            mode == file_access_mode::read_write_append)
        {
            LARGE_INTEGER largePosition;
            largePosition.QuadPart = 0u;
            if (!SetFilePointerEx(result.m_handle, largePosition, nullptr, FILE_END))
            {
                const platform_error error = translate_platform_error(GetLastError());
                CloseHandle(result.m_handle);
                return make_error(error);
            }
        }

        return result;
    }

    result<void> platform::close_file(file file) {}

    result<size_type> platform::read_file_section(file file, array_view<byte> target, size_type amountOfBytes, size_type offset) {}

    result<void> platform::read_file(file file, array_view<byte> target, size_type offset) {}

    result<size_type> platform::write_file(file file, size_type offset, array_view<byte const> data) {}

    result<void> platform::write_all_to_file(file file, size_type offset, array_view<byte const> data) {}

    result<void> platform::truncate_file(file file, size_type offset) {}

    result<uint64> platform::get_file_size(string_view absolutePath) {}

    result<uint64> platform::get_file_size(file file) {}

    result<void> platform::flush_file_write_buffer(file file) {}

    result<void> platform::rename_file(string_view oldAbsolutePath, string_view newAbsolutePath) {}

    result<void> platform::delete_file(string_view absolutePath, file_delete_flags flags) {}


    result<file_info> platform::get_file_info(const string_view absolutePath) noexcept
    {
        dynamic_wstring widePath = to_utf16(fs::localize(absolutePath));

        const HANDLE fileHandle = ::CreateFileW(
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
                    .isFile = (information.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_ARCHIVE |
                        FILE_ATTRIBUTE_DEVICE | FILE_ATTRIBUTE_REPARSE_POINT)) == 0u,
                };
    }

    result<file_info> platform::get_file_info(const file file) noexcept
    {
        BY_HANDLE_FILE_INFORMATION information;
        if (GetFileInformationByHandle(file.m_handle, &information) == FALSE)
        {
            return make_error(translate_platform_error(GetLastError()));
        }

        return file_info{
                    .lastWriteTimestamp = translate_timestamp(information.ftLastWriteTime),
                    .size = combine_dwords(information.nFileSizeLow, information.nFileSizeHigh),
                    .isWritable = (information.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0u,
                    .isDirectory = false,
                };
    }

    bool file::operator==(const file& other) const
    {
        return m_handle == other.m_handle;
    }

    directory_iterator::directory_iterator(directory_iterator&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    directory_iterator& directory_iterator::operator=(directory_iterator&& other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        return *this;
    }

    directory_iterator::~directory_iterator()
    {
        if (m_handle)
        {
            FindClose(m_handle->directory);
            delete m_handle;
            m_handle = nullptr;
        }
    }

    result<dynamic_string> directory_iterator::get_path() const
    {
        bool hasEncodingError;
        dynamic_string result = fs::standardize(to_utf8(m_handle->findData.cFileName, &hasEncodingError));
        if (hasEncodingError)
        {
            return make_error(platform_error::encoding_error);
        }

        return result;
    }

    bool directory_iterator::operator==(const directory_iterator& other) const
    {
        return m_handle == other.m_handle;
    }

    bool dynamic_library::operator==(const dynamic_library& other) const
    {
        return m_handle == other.m_handle;
    }
} // namespace rsl

#endif
