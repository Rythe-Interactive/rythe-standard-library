#pragma once
#include "unique_object.hpp"

namespace rsl
{
    template <typename T>
    constexpr unique_object<T>::unique_object(
            nullptr_type
            ) noexcept
        : unique_rsc() {}

    template <typename T>
    unique_object<T>::unique_object(
            allocator_storage allocator
            )
        noexcept
        : unique_rsc(allocator) {}

    template <typename T>
    template <typename... Args>
    constexpr unique_object<T> unique_object<T>::create_in_place(
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(constructible_at_all<T>, "T needs to be constructible.");
        unique_object ret;
        ret.arm(rsl::forward<Args>(args)...);
        return ret;
    }

    template <typename T>
    template <typename... Args>
    unique_object<T> unique_object<T>::create_in_place_with_allocator(
            allocator_storage allocator,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<
            T, Args...>)
    {
        unique_object ret(allocator);
        ret.arm(forward<Args>(args)...);
        return ret;
    }

    template <typename T>
    constexpr unique_object<T>::unique_object(unique_object&& other) noexcept
        : unique_rsc(rsl::move(other)) {}

    template <typename T>
    template <typename OtherType>
        requires (is_pointer_assignable_v<T, OtherType>)
    constexpr unique_object<T>::unique_object(
            unique_object<OtherType>&& other
            ) noexcept
        : unique_rsc(internal::alloc_and_factory_only_signal, other)
    {
        if (other.template unique_object<OtherType>::unique_rsc::m_value.holds_value())
        {
            unique_rsc::m_value.emplace(
                    rsl::move(other.template unique_object<OtherType>::unique_rsc::m_value.value())
                    );
        }

        mem_rsc::set_ptr(other.template unique_object<OtherType>::mem_rsc::get_ptr());
        other.template unique_object<OtherType>::mem_rsc::set_ptr(nullptr);
    }

    template <typename T>
    constexpr unique_object<T>& unique_object<T>::operator=(unique_object&& other) noexcept
    {
        unique_rsc::operator=(rsl::move(other));
        return *this;
    }

    template <typename T>
    template <typename OtherType>
        requires (is_pointer_assignable_v<T, OtherType>)
    constexpr unique_object<T>& unique_object<T>::operator=(
            unique_object<OtherType>&& other
            ) noexcept
    {
        unique_rsc::disarm();

        mem_rsc::set_allocator(other.mem_rsc::get_allocator());
        mem_rsc::set_factory(other.mem_rsc::get_factory());
        unique_rsc::m_value.emplace(rsl::move(other.unique_rsc::m_value.value()));

        mem_rsc::set_ptr(other.mem_rsc::get_ptr());
        other.mem_rsc::set_ptr(nullptr);

        return *this;
    }

    template <typename T>
    void unique_object<T>::deleter::operator()(T* mem) noexcept
    {
        factory<T>::destroy(mem, 1);
        allocator->deallocate(mem, factory<T>::type_size());
    }

    template <typename T>
    template <typename... Args>
    constexpr void unique_object<T>::arm(
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        if (is_constant_evaluated())
        {
            T* ptr = new T(forward<Args>(args)...);
            unique_rsc::arm(deleter{ .allocator = this->get_allocator() }, ptr);
        }
        else
        {
            T* ptr = static_cast<T*>(this->get_allocator()->allocate(sizeof(T)));
            factory<T>::construct(ptr, 1, forward<Args>(args)...);
            unique_rsc::arm(deleter{ .allocator = this->get_allocator() }, ptr);
        }
    }
}
