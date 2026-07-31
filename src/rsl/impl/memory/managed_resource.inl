#pragma once
#include "managed_resource.hpp"

namespace rsl
{
    template <typename T, internal::managed_deleter_type<T> Deleter>
    void internal::managed_payload<T, Deleter>::destroy(void* value) noexcept
    {
        if (deleter)
        {
            deleter(*static_cast<T*>(value));
            deleter = nullptr;
        }
    }

    template <typename T>
    constexpr managed_resource<T>::managed_resource(nullptr_type)
        noexcept(is_nothrow_constructible_v<ref_counter>)
        : ref_counter() {}

    template <typename T>
    managed_resource<T>::managed_resource(allocator_storage allocator)
        noexcept(is_nothrow_constructible_v<ref_counter, allocator_storage>)
        : ref_counter(allocator) {}

    template <typename T>
    template <internal::managed_deleter_type<T> Deleter, typename... Args>
    constexpr managed_resource<T>::managed_resource(Deleter deleter, Args&&... args)
        noexcept(is_nothrow_constructible_v<ref_counter> && is_nothrow_constructible_v<T, Args...>)
        : ref_counter()
    {
        arm(deleter, forward<Args>(args)...);
    }

    template <typename T>
    template <internal::managed_deleter_type<T> Deleter, typename... Args>
    managed_resource<T>::managed_resource(allocator_storage allocator, Deleter deleter, Args&&... args)
        noexcept(is_nothrow_constructible_v<ref_counter, allocator_storage> && is_nothrow_constructible_v<
            T, Args...>)
        : ref_counter(allocator)
    {
        arm(deleter, forward<Args>(args)...);
    }

    template <typename T>
    constexpr managed_resource<T>::~managed_resource() noexcept
    {
        disarm_impl();
    }

    template <typename T>
    template <internal::managed_deleter_type<T> Deleter, typename... Args>
    constexpr void managed_resource<T>::arm(Deleter deleter, Args&&... args)
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        m_value.emplace(forward<Args>(args)...);

        ref_counter::set_factory(type_erased_factory(construct_type_signal<internal::managed_payload<T, Deleter>>));
        ref_counter::arm();

        bit_cast<internal::managed_payload<T, Deleter>*>(mem_rsc::get_ptr())->deleter = deleter;
    }

    template <typename T>
    constexpr void managed_resource<T>::disarm_impl() noexcept
    {
        if (ref_counter::is_armed() && ref_counter::is_free())
        {
            mem_rsc::get_ptr()->destroy(get());
        }
    }

    template <typename T>
    void managed_resource<T>::on_disarm() noexcept
    {
        disarm_impl();
    }

} // namespace rsl
