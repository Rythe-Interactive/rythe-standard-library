#pragma once

namespace rsl
{
    template <typename... Args>
    [[nodiscard]] [[rythe_always_inline]] dynamic_string format(fmt::format_string<Args...> fmt, Args&&... args)
    {
        dynamic_string result;
        fmt::format_to(back_inserter(result), fmt, rsl::forward<Args>(args)...);
        return result;
    }
} // namespace rsl
