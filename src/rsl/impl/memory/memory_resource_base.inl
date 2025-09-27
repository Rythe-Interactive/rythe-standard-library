#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            const dynamic_memory_resource_base& other
            ) noexcept(is_nothrow_copy_constructible_v<typed_alloc_type>)
        : m_alloc(other.m_alloc) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            dynamic_memory_resource_base&& other
            ) noexcept(is_nothrow_move_constructible_v<typed_alloc_type>)
        : m_alloc(rsl::move(other.m_alloc)) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const allocator_storage_type& allocStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>)
        : m_alloc(allocStorage, construct_type_signal<UtilType>) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>)
        : m_alloc(factoryStorage) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::dynamic_memory_resource_base(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>)
        : m_alloc(allocStorage, factoryStorage) {}

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_allocator(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
    {
        m_alloc.set_allocator(allocStorage);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr Alloc& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator() noexcept
    {
        return m_alloc.get_allocator();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const Alloc& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator() const noexcept
    {
        return m_alloc.get_allocator();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_alloc.set_factory(factoryStorage);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr Factory& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory() noexcept
    {
        return m_alloc.get_factory();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const Factory& dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory() const noexcept
    {
        return m_alloc.get_factory();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator_storage() noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_allocator_storage() const noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory_storage() noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_factory_storage() const noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate(const size_type count) noexcept
    {
        m_ptr = m_alloc.allocate(count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate(
                const size_type count,
                const size_type alignment
                ) noexcept
    {
        m_ptr = m_alloc.allocate(count, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate(
            const size_type oldCount,
            const size_type newCount
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate(
                const size_type oldCount,
                const size_type newCount,
                const size_type alignment
                )
            noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::deallocate(const size_type count) noexcept
    {
        m_alloc.deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::deallocate(
                const size_type count,
                const size_type alignment
                ) noexcept
    {
        m_alloc.deallocate(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::construct(
            const size_type count,
            const size_type offset
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_alloc.construct(get_ptr_at(offset), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(factory_traits<Factory>::noexcept_copyable)
    {
        m_alloc.copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_alloc.move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy(
                const size_type count,
                const size_type offset
                ) noexcept
    {
        m_alloc.destroy(get_ptr_at(offset), count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate_and_construct(
            const size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_and_construct(count);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::allocate_aligned_and_construct(
                const size_type count,
                const size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate_and_construct(
                const size_type oldCount,
                const size_type newCount
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate_and_construct(get_ptr(), oldCount, newCount);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        m_ptr = m_alloc.reallocate_aligned_and_construct(get_ptr(), oldCount, newCount, alignment);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate(
            const size_type count
            ) noexcept
    {
        m_alloc.destroy_and_deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate_aligned(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_alloc.destroy_and_deallocate_aligned(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(m_ptr);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_ptr);
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr_at(const size_type offset) noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource_base<Alloc, Factory, UtilType, Untyped>::set_ptr(UtilType* const& ptr) noexcept
    {
        m_ptr = ptr;
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T, Alloc, Factory>::construct(
                size_type count,
                size_type offset,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_alloc.construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::allocate_and_construct(
            size_type count,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_and_construct(count, rsl::forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::allocate_aligned_and_construct(
            size_type count,
            size_type alignment,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_aligned_and_construct(count, alignment, rsl::forward<Args>(args)...);
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T, Alloc, Factory>::reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                rsl::forward<Args>(args)...
                );
    }

    template <typename T, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T, Alloc, Factory>::reallocate_aligned_and_construct(
            size_type oldCount,
            size_type newCount,
            size_type alignment,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_aligned_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                alignment,
                rsl::forward<Args>(args)...
                );
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            const static_memory_resource_base& other
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_factory(other.m_factory) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            static_memory_resource_base&& other
            ) noexcept(is_nothrow_move_constructible_v<factory_storage_type>)
        : m_factory(rsl::move(other.m_factory)) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_factory(factoryStorage) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::static_memory_resource_base(
            factory_storage_type&& factoryStorage
            ) noexcept(is_nothrow_move_constructible_v<factory_storage_type>)
        : m_factory(rsl::move(factoryStorage)) {}

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::set_factory(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_factory = factoryStorage;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_t& static_memory_resource_base<
        BufferSize, Factory, UtilType, Untyped>::get_factory() noexcept
    {
        return *m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_t&
    static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory() const noexcept
    {
        return *m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_storage_type&
    static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory_storage() noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::factory_storage_type&
    static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_factory_storage() const noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::construct(
            const size_type count,
            const size_type offset
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_factory->construct(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(factory_traits<Factory>::noexcept_copyable)
    {
        m_factory->copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_factory->move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::destroy(
            const size_type count,
            const size_type offset
            ) noexcept
    {
        m_factory->destroy(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(static_cast<static_storage::value_type*>(m_buffer.data));
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(static_cast<const static_storage::value_type*>(m_buffer.data));
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) noexcept
    {
        return advance(get_ptr(), offset * m_factory->type_size());
    }

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* static_memory_resource_base<BufferSize, Factory, UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_factory->type_size());
    }

    template <typename T, size_type BufferSize, factory_type Factory>
    template <typename... Args>
    constexpr void typed_static_memory_resource<T, BufferSize, Factory>::construct(
            const size_type count,
            const size_type offset,
            Args&&... args
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_factory->construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::hybrid_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            const hybrid_memory_resource_base& other
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_alloc(other.m_alloc) {}

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::hybrid_memory_resource_base(
            internal::alloc_and_factory_only_signal_type,
            hybrid_memory_resource_base&& other
            ) noexcept(is_nothrow_move_constructible_v<factory_storage_type>)
        : m_alloc(rsl::move(other.m_alloc)) {}

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::hybrid_memory_resource_base(
            const allocator_storage_type& allocStorage
            ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>) : m_alloc(allocStorage) {}

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::hybrid_memory_resource_base(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>) : m_alloc(factoryStorage) {}

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::hybrid_memory_resource_base(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>)
        : m_alloc(allocStorage, factoryStorage) {}

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::set_allocator(
            const allocator_storage_type& allocStorage
            ) noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
    {
        m_alloc.set_allocator(allocStorage);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocator_t&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_allocator() noexcept
    {
        return m_alloc.get_allocator();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocator_t&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_allocator() const noexcept
    {
        return m_alloc.get_allocator();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::set_factory(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_alloc.set_factory(factoryStorage);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::factory_t&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_factory() noexcept
    {
        return m_alloc.get_factory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::factory_t&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_factory() const noexcept
    {
        return m_alloc.get_factory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_allocator_storage() noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocator_storage_type&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_allocator_storage() const noexcept
    {
        return m_alloc.get_allocator_storage();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_factory_storage() noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const typename hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::factory_storage_type&
    hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_factory_storage() const noexcept
    {
        return m_alloc.get_factory_storage();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocate(const size_type count) noexcept
    {
        m_ptr = m_alloc.allocate(count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocate(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_ptr = m_alloc.allocate(count, alignment);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::reallocate(
            const size_type oldCount,
            const size_type newCount
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        if (is_dynamic_memory()) [[likely]]
        {
            m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount);
        }
        else
        {
            if (newCount != 0ull) [[likely]]
            {
                m_ptr = m_alloc.allocate(newCount);
                if (m_ptr) [[likely]]
                {
                    m_alloc.move(get_ptr(), m_buffer.data, oldCount);
                }
            }
        }
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::reallocate(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        if (is_dynamic_memory()) [[likely]]
        {
            m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount, alignment);
        }
        else
        {
            if (newCount != 0ull) [[likely]]
            {
                m_ptr = m_alloc.allocate(newCount, alignment);
                if (m_ptr) [[likely]]
                {
                    m_alloc.move(m_ptr, m_buffer.data, oldCount);
                }
            }
        }
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::deallocate(
            const size_type count
            ) noexcept
    {
        if (is_static_memory()) [[unlikely]]
        {
            return;
        }

        m_alloc.deallocate(get_ptr(), count);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::deallocate(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        if (is_static_memory()) [[unlikely]]
        {
            return;
        }

        m_alloc.deallocate(get_ptr(), count, alignment);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::construct(
            const size_type count,
            const size_type offset
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_alloc.construct(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(factory_traits<Factory>::noexcept_copyable)
    {
        m_alloc.copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        m_alloc.move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::destroy(
            const size_type count,
            const size_type offset
            ) noexcept
    {
        m_alloc.destroy(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocate_and_construct(
            const size_type count
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_and_construct(count);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::reallocate_and_construct(
            const size_type oldCount,
            const size_type newCount
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_ptr = m_alloc.reallocate_and_construct(get_ptr(), oldCount, newCount);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_ptr = m_alloc.reallocate_aligned_and_construct(get_ptr(), oldCount, newCount, alignment);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate(
            const size_type count
            ) noexcept
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_alloc.destroy_and_deallocate(get_ptr(), count);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::destroy_and_deallocate_aligned(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        if (is_dynamic_memory()) [[likely]]
        {
            m_alloc.destroy_and_deallocate_aligned(get_ptr(), count, alignment);
            set_ptr_to_static_memory();
        }
        else
        {
            m_alloc.destroy(get_ptr(), count);
        }
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr bool hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::is_static_memory() const noexcept
    {
        return m_ptr >= m_buffer.data && m_ptr < m_buffer.data + BufferSize;
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr bool hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::is_dynamic_memory() const noexcept
    {
        return !is_static_memory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(m_ptr);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_ptr);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::set_ptr(UtilType* const& ptr) noexcept
    {
        if (ptr == nullptr) [[unlikely]]
        {
            set_ptr_to_static_memory();
        }
        else
        {
            m_ptr = ptr;
        }
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_static_ptr() noexcept
    {
        return bit_cast<UtilType*>(static_cast<decltype(m_buffer)::value_type*>(m_buffer.data));
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::
    get_static_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_buffer.data);
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_static_ptr_at(
            const size_type offset
            ) noexcept
    {
        rsl_assert_out_of_range((offset * m_alloc.type_size()) < BufferSize);
        return advance(get_static_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr const UtilType* hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::get_static_ptr_at(
            const size_type offset
            ) const noexcept
    {
        rsl_assert_out_of_range((offset * m_alloc.type_size()) < BufferSize);
        return advance(get_static_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::move_to_static_memory_and_deallocate(
            const size_type count,
            const size_type memoryCount
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        rsl_assert_invalid_operation(count * m_alloc.type_size() <= BufferSize);
        UtilType* dynamicPtr = get_ptr();
        m_alloc.move(get_static_ptr(), dynamicPtr, count);
        m_alloc.destroy(dynamicPtr, count);
        m_alloc.deallocate(dynamicPtr, memoryCount);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    constexpr void hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, Untyped>::set_ptr_to_static_memory() noexcept
    {
        set_ptr(m_buffer.data);
    }

    template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>::construct(
            const size_type count,
            const size_type offset,
            Args&&... args
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_alloc.construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>::allocate_and_construct(
            const size_type count,
            Args&&... args
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_and_construct(count, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment,
            Args&&... args
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_aligned_and_construct(count, alignment, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>::reallocate_and_construct(
            const size_type oldCount,
            const size_type newCount,
            Args&&... args
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
    {
        rsl_assert_invalid_operation(base_type::is_dynamic_memory());
        base_type::m_ptr = base_type::m_alloc.reallocate_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                rsl::forward<Args>(args)...
                );
    }

    template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment,
            Args&&... args
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable)
    {
        rsl_assert_invalid_operation(base_type::is_dynamic_memory());
        base_type::m_ptr = base_type::m_alloc.reallocate_aligned_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                alignment,
                rsl::forward<Args>(args)...
                );
    }
} // namespace rsl
