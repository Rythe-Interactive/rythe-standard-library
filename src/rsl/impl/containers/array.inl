#pragma once

namespace rsl
{
    template <typename T, size_type StaticCapacity, bool CanResize, bool CanAllocate>
    constexpr basic_dynamic_array<T, StaticCapacity, CanResize, CanAllocate>::basic_dynamic_array(
            const container_base& src
            ) noexcept(container_base::copy_construct_container_noexcept)
        : container_base(src) {}

    template <typename T, size_type StaticCapacity, bool CanResize, bool CanAllocate>
    constexpr basic_dynamic_array<T, StaticCapacity, CanResize, CanAllocate>::basic_dynamic_array(
            container_base&& src
            ) noexcept(container_base::move_construct_container_noexcept)
        : container_base(rsl::move(src)) {}
} // namespace rsl
