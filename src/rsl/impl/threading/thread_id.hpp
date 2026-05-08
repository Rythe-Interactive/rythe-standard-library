#pragma once

namespace rsl
{
    struct thread_id
    {
        id_type nativeId;

        [[nodiscard]] [[rythe_always_inline]] constexpr auto operator<=>(const thread_id&) const = default;
    };
}
