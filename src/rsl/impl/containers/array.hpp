#pragma once

#include "../rsl_core.hpp"

#include "contiguous_container_base.hpp"

namespace rsl
{
    template <typename T, size_type StaticCapacity = 0ull, bool CanResize = true, bool CanAllocate = true>
    class basic_dynamic_array :
        public contiguous_container_base<T, T*, const T*, contiguous_container_info<false, StaticCapacity, CanResize, CanAllocate>>
    {
    public:
        using container_base =
                contiguous_container_base<T, T*, const T*, contiguous_container_info<false, StaticCapacity, CanResize, CanAllocate>>;
        using mem_rsc = typename container_base::mem_rsc;
        using value_type = T;
        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;

        using contiguous_container_base<T, T*, const T*, contiguous_container_info<false, StaticCapacity, CanResize, CanAllocate>>::
                contiguous_container_base;

        [[rythe_always_inline]] constexpr basic_dynamic_array(const container_base& src)
                noexcept(container_base::copy_construct_container_noexcept);
        [[rythe_always_inline]] constexpr basic_dynamic_array(container_base&& src)
                noexcept(container_base::move_construct_container_noexcept);

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator[];
        using container_base::operator=;
    };

    template <typename T>
    using dynamic_array = basic_dynamic_array<T>;

    template <typename T, size_type StaticCapacity>
    using hybrid_array = basic_dynamic_array<T, StaticCapacity>;

    template <typename T, size_type StaticCapacity>
    using static_array = basic_dynamic_array<T, StaticCapacity, true, false>;

    template <typename T, size_type StaticCapacity>
    using array = basic_dynamic_array<T, StaticCapacity, false, false>;
} // namespace rsl

#include "array.inl"
