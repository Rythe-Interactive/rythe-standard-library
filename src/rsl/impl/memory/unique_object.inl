#pragma once
#include "unique_object.hpp"

namespace rsl
{
    template <typename T, statically_optional_typed_factory_type Factory>
    constexpr unique_object<T, Factory>::unique_object(
            nullptr_type
            ) noexcept(is_nothrow_constructible_v<mem_rsc>)
        : unique_rsc(), m_factory() {}

    template <typename T, statically_optional_typed_factory_type Factory>
    unique_object<T, Factory>::unique_object(
            allocator_storage allocator
            )
        noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage>)
        : unique_rsc(allocator), m_factory() {}

    template <typename T, statically_optional_typed_factory_type Factory>
    unique_object<T, Factory>::unique_object(
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<mem_rsc>)
        : unique_rsc(), m_factory(factoryStorage) {}

    template <typename T, statically_optional_typed_factory_type Factory>
    unique_object<T, Factory>::unique_object(
            allocator_storage allocator,
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage>)
        : unique_rsc(allocator), m_factory(factoryStorage) {}

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename... Args>
    constexpr unique_object<T, Factory> unique_object<T, Factory>::create_in_place(
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(constructible_at_all<T>, "T needs to be constructible.");
        unique_object ret;
        ret.arm(rsl::forward<Args>(args)...);
        return ret;
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename... Args>
    unique_object<T, Factory> unique_object<T, Factory>::create_in_place_with_allocator(
            allocator_storage allocator,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage> && is_nothrow_constructible_v<
            T, Args...>)
    {
        unique_object ret(allocator);
        ret.arm(forward<Args>(args)...);
        return ret;
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename... Args>
    unique_object<T, Factory> unique_object<T, Factory>::create_in_place_alloc_factory(
            allocator_storage allocator,
            const factory_storage_type& factoryStorage,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<mem_rsc, allocator_storage> && is_nothrow_constructible_v<T, Args...>)
    {
        unique_object ret(allocator, factoryStorage);
        ret.arm(forward<Args>(args)...);
        return ret;
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    constexpr unique_object<T, Factory>::unique_object(unique_object&& other) noexcept
        : unique_rsc(rsl::move(other)), m_factory(other.m_factory) {}

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename OtherType, statically_optional_typed_factory_type OtherFactory>
        requires (is_pointer_assignable_v<T, OtherType>)
    constexpr unique_object<T, Factory>::unique_object(
            unique_object<OtherType, OtherFactory>&& other
            ) noexcept
        : unique_rsc(internal::alloc_and_factory_only_signal, other),
          m_factory(other.get_factory_storage())
    {
        if (other.template unique_object<OtherType, OtherFactory>::unique_rsc::m_value.holds_value())
        {
            unique_rsc::m_value.emplace(
                    rsl::move(other.template unique_object<OtherType, OtherFactory>::unique_rsc::m_value.value())
                    );
        }

        mem_rsc::set_ptr(other.template unique_object<OtherType, OtherFactory>::mem_rsc::get_ptr());
        other.template unique_object<OtherType, OtherFactory>::mem_rsc::set_ptr(nullptr);
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    constexpr unique_object<T, Factory>& unique_object<T, Factory>::operator=(unique_object&& other) noexcept
    {
        unique_rsc::operator=(rsl::move(other));
        m_factory = rsl::move(other.get_factory_storage());
        return *this;
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename OtherType, statically_optional_typed_factory_type OtherFactory>
        requires (is_pointer_assignable_v<T, OtherType>)
    constexpr unique_object<T, Factory>& unique_object<T, Factory>::operator=(
            unique_object<OtherType, OtherFactory>&& other
            ) noexcept
    {
        unique_rsc::disarm();

        mem_rsc::set_allocator(other.mem_rsc::get_allocator_storage());
        mem_rsc::set_factory(other.mem_rsc::get_factory_storage());
        unique_rsc::m_value.emplace(rsl::move(other.unique_rsc::m_value.value()));

        m_factory = rsl::move(other.get_factory_storage());

        mem_rsc::set_ptr(other.mem_rsc::get_ptr());
        other.mem_rsc::set_ptr(nullptr);

        return *this;
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    void unique_object<T, Factory>::deleter::operator()(T* mem) noexcept
    {
        factory->destroy(mem, 1);
        allocator->deallocate(mem, factory->type_size());
    }

    template <typename T, statically_optional_typed_factory_type Factory>
    template <typename... Args>
    constexpr void unique_object<T, Factory>::arm(
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
        requires (Factory::valid_factory)
    {
        if (is_constant_evaluated())
        {
            T* ptr = new T(forward<Args>(args)...);
            unique_rsc::arm(deleter{ .factory = m_factory, .allocator = this->get_allocator_storage() }, ptr);
        }
        else
        {
            T* ptr = static_cast<T*>(this->get_allocator().allocate(m_factory->type_size()));
            m_factory->construct(ptr, 1, forward<Args>(args)...);
            unique_rsc::arm(deleter{ .factory = m_factory, .allocator = this->get_allocator_storage() }, ptr);
        }
    }
}
