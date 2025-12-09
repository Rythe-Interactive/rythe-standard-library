#pragma once

#include "../threading/thread_id.hpp"
#include "../time/time_point.hpp"
#include "../util/source_location.hpp"

#include "severity.hpp"

namespace rsl::log
{
	struct message
	{
		string_view loggerName;
		thread_id threadId;

		time::point32 timestamp;
		source_location sourceLocation;
		log::severity severity;
		string_view msg;
		fmt::format_args formatArgs;
	    bool appendNewLine;
	};
}
