#include "../platform.hpp"

#if RYTHE_PLATFORM_LINUX

#include <dlfcn.h>
#include <errno.h>
#include <sched.h>
#include <pthread.h>
#include <utmpx.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

namespace rsl
{
    NATIVE_API_TYPE_ACCESSORS(dynamic_library, void*)

	dynamic_library platform::load_library(cstring path)
	{
		dynamic_library result;
		set_native_handle(result, dlopen(path, RTLD_NOW));
		return result;
	}

	void platform::release_library(const dynamic_library library)
	{
		dlclose(get_native_handle(library));
	}

	void* platform::get_symbol(const dynamic_library library, cstring symbolName)
	{
		return dlsym(get_native_handle(library), symbolName);
	}

	thread_id platform::get_current_thread_id()
	{
        // return thread_id{ .nativeId = static_cast<id_type>(syscall( SYS_gettid ) );
		return thread_id{ .nativeId = static_cast<id_type>(gettid()) };
	}

	void platform::yield_current_thread()
	{
		sched_yield();
	}

	void platform::sleep_current_thread(const uint32 milliseconds)
	{
		timespec sleepTime;
		timespec remainingTime;
		sleepTime.tv_sec = milliseconds / 1000u;
		sleepTime.tv_nsec = (milliseconds - (sleepTime.tv_sec * 1000u)) * 1000000u;

		while (true)
		{
			int32 result = nanosleep(&sleepTime, &remainingTime);

			int error = 0;
			if (result == -1)
			{
				error = errno;
			}

			if (result == 0 || error != EINTR)
			{
				break;
			}

			sleepTime = remainingTime;
		}
	}
} // namespace rsl

#endif
