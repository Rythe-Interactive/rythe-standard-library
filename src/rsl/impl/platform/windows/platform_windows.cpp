// ReSharper disable CppWrongIncludesOrder
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
#include "../platform.hpp"

#define RYTHE_THREAD_HANDLE_IMPL HANDLE

#include "../../containers/string.hpp"
#include "../../threading/current_thread.inl"
#include "../../threading/thread.hpp"

#include <filesystem>

namespace rsl
{
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

    // TODO(Glyn): Use windows api.
    bool platform::is_regular_file(string_view absolutePath)
    {
        std::error_code ec;
        return std::filesystem::is_regular_file(std::filesystem::path(std::string_view(absolutePath.data(), absolutePath.size())), ec);
    }

    bool platform::is_directory(string_view absolutePath)
    {
        std::error_code ec;
        return std::filesystem::is_directory(std::filesystem::path(std::string_view(absolutePath.data(), absolutePath.size())), ec);
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

    // TODO(Glyn): Use windows api.
    bool platform::does_path_entry_exist(string_view absolutePath)
    {
        std::error_code ec;
        return std::filesystem::exists(std::filesystem::path(std::string_view(absolutePath.data(), absolutePath.size())), ec);
    }

    dynamic_array<dynamic_string> platform::enumerate_directory(string_view absolutePath)
    {
        dynamic_array<dynamic_string> result;
        for (const auto& entry : std::filesystem::directory_iterator(
                     std::filesystem::path(std::string_view(absolutePath.data(), absolutePath.size()))
                     ))
        {
            auto path = entry.path().string();
            result.push_back(dynamic_string::from_buffer(path.data(), path.size()));
        }

        return result;
    }
} // namespace rsl

#endif
