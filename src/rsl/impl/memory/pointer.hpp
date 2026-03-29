#pragma once

namespace rsl
{
    /// Non owning pointer to an object managed somewhere else.
    template<typename T>
    struct pointer final
    {
        T* ptr;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator pointer<const T>() const noexcept { return { ptr }; }
        [[nodiscard]] [[rythe_always_inline]] constexpr operator bool() const noexcept { return ptr; }

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(T* other) const noexcept { return ptr == other; }
        [[nodiscard]] [[rythe_always_inline]] constexpr const T* operator->() const noexcept { return ptr; }
        [[nodiscard]] [[rythe_always_inline]] constexpr T* operator->() noexcept { return ptr; }
        [[nodiscard]] [[rythe_always_inline]] constexpr const T& operator*() const noexcept { return *ptr; }
        [[nodiscard]] [[rythe_always_inline]] constexpr T& operator*() noexcept { return *ptr; }
    };
}
