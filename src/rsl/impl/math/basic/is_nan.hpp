#pragma once

namespace rsl::math
{
    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr bool is_nan(const T value) noexcept
    {
        return value != value;
    } // NOLINT
} // namespace rsl::math
