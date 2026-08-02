#pragma once
#include "unique_resource.hpp"

namespace rsl
{
    template <typename T, internal::unique_deleter_type<T> Deleter>
    void internal::unique_payload<T, Deleter>::destroy(void* value) noexcept
    {
        if (deleter)
        {
            deleter(*static_cast<T*>(value));
        }
    }

    template <typename T>
    constexpr unique_resource<T>::unique_resource(nullptr_type)
        noexcept
        : mem_rsc() {}

    template <typename T>
    constexpr unique_resource<T>::unique_resource(const allocator_storage allocator)
        noexcept
        : mem_rsc(allocator) {}

    template <typename T>
    template <typename OtherT>
    constexpr unique_resource<T>::unique_resource(
            internal::alloc_and_factory_only_signal_type,
            const unique_resource<OtherT>& other
        ) noexcept
        : mem_rsc(internal::alloc_and_factory_only_signal, other), m_value() {}

    template <typename T>
    template <internal::unique_deleter_type<T> Deleter, typename... Args>
    constexpr unique_resource<T>::unique_resource(Deleter deleter, Args&&... args)
        noexcept(is_nothrow_constructible_v<T, Args...>)
        : mem_rsc()
    {
        arm(deleter, forward<Args>(args)...);
    }

    template <typename T>
    template <internal::unique_deleter_type<T> Deleter, typename... Args>
    constexpr unique_resource<T>::unique_resource(
            const allocator_storage allocator,
            Deleter deleter,
            Args&&... args
        )
        noexcept(is_nothrow_constructible_v<T, Args...>)
        : mem_rsc(allocator)
    {
        arm(deleter, forward<Args>(args)...);
    }

    template <typename T>
    constexpr unique_resource<T>::unique_resource(unique_resource&& other) noexcept
        : mem_rsc(other.mem_rsc::get_allocator(), other.mem_rsc::get_factory()), m_value(rsl::move(other.m_value))
    {
        mem_rsc::set_ptr(other.mem_rsc::get_ptr());
        other.mem_rsc::set_ptr(nullptr);
    }

    template <typename T>
    constexpr unique_resource<T>::~unique_resource() noexcept
    {
        disarm();
    }

    template <typename T>
    constexpr unique_resource<T>& unique_resource<T>::operator=(unique_resource&& other) noexcept
    {
        disarm();

        mem_rsc::set_allocator(other.mem_rsc::get_allocator());
        mem_rsc::set_factory(other.mem_rsc::get_factory());
        m_value = rsl::move(other.m_value);

        mem_rsc::set_ptr(other.mem_rsc::get_ptr());
        other.mem_rsc::set_ptr(nullptr);

        return *this;
    }

    template <typename T>
    template <internal::unique_deleter_type<T> Deleter, typename... Args>
    constexpr void unique_resource<T>::arm(Deleter deleter, Args&&... args)
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        m_value.emplace(rsl::forward<Args>(args)...);

        mem_rsc::set_factory(type_erased_factory(construct_type_signal<internal::unique_payload<T, Deleter>>));
        mem_rsc::allocate_and_construct(1);
        bit_cast<internal::unique_payload<T, Deleter>*>(mem_rsc::get_ptr())->deleter = deleter;
    }

    template <typename T>
    constexpr bool unique_resource<T>::is_armed() const noexcept
    {
        return mem_rsc::get_ptr();
    }

    template <typename T>
    constexpr void unique_resource<T>::disarm() noexcept
    {
        if (!is_armed())
        {
            return;
        }

        mem_rsc::get_ptr()->destroy(get());

        m_value.reset();
        mem_rsc::destroy_and_deallocate();
        mem_rsc::set_ptr(nullptr);
    }
}
