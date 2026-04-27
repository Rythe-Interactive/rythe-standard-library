#pragma once
#include "thread_id.hpp"

namespace rsl
{
    RYTHE_DECLARE_NATIVE_API_TYPE(thread)

	class thread
	{
	public:
		operator bool() const;

		thread_id get_id() const;
		uint32 join() const;

    private:
        RYTHE_NATIVE_API_TYPE_PRIVATE_MEMBERS(thread)
	};
}
