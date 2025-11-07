#pragma once

#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace rsl
{
    template <typename... Args>
    [[nodiscard]] [[rythe_always_inline]] dynamic_string format(fmt::format_string<Args...> fmt, Args&&... args);
}
