#pragma once
#include "../util/common.hpp"

namespace rsl
{
    template <typename T>
    struct reference_wrapper
    {
        static_assert(is_object_v<T> || is_function_v<T>, "reference_wrapper<T> requires T to be an object type or a function type.");

        using type = T;

        [[rythe_always_inline]] constexpr reference_wrapper(T& value) noexcept : m_ptr(addressof(value)) {}


        [[nodiscard]] [[rythe_always_inline]] constexpr T& get() const noexcept { return *m_ptr; }
        [[rythe_always_inline]] constexpr operator T&() const noexcept { return *m_ptr; }

    private:
        T* m_ptr{};
    };

    template <typename T>
    reference_wrapper(T&) -> reference_wrapper<T>;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr reference_wrapper<T> ref(T& value) noexcept
    {
        return reference_wrapper<T>(value);
    }

    template <typename T>
    void ref(const T&&) = delete;

    template <class T>
    [[nodiscard]] [[rythe_always_inline]] constexpr reference_wrapper<T> ref(reference_wrapper<T> value) noexcept
    {
        return value;
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr reference_wrapper<const T> cref(const T& value) noexcept
    {
        return reference_wrapper<const T>(value);
    }

    template <typename T>
    void cref(const T&&) = delete;

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] constexpr reference_wrapper<const T> cref(reference_wrapper<T> value) noexcept
    {
        return value;
    }
}
