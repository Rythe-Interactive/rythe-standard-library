#pragma once
#include "thread_id.hpp"

namespace rsl
{
    DECLARE_NATIVE_API_TYPE(thread)

	class thread
	{
	public:
		operator bool() const;

		thread_id get_id() const;
		uint32 join() const;

    private:
        NATIVE_API_TYPE_PRIVATE_MEMBERS(thread)
	};
}
