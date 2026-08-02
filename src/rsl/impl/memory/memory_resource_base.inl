#pragma once
#include "memory_resource_base.hpp"

namespace rsl
{
    template <typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource<UtilType, Untyped>::dynamic_memory_resource(
            internal::alloc_and_factory_only_signal_type, const dynamic_memory_resource& other)
            noexcept
        : m_alloc(other.m_alloc)
    {}

    template <typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource<UtilType, Untyped>::dynamic_memory_resource(allocator_storage allocator) noexcept
        : m_alloc(allocator, construct_type_signal<UtilType>)
    {}

    template <typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource<UtilType, Untyped>::dynamic_memory_resource(const type_erased_factory& factory) noexcept
        requires(Untyped)
        : m_alloc(factory)
    {}

    template <typename UtilType, bool Untyped>
    constexpr dynamic_memory_resource<UtilType, Untyped>::dynamic_memory_resource(
            allocator_storage allocator,
            const type_erased_factory& factory
            ) noexcept
        requires(Untyped)
        : m_alloc(allocator, factory)
    {}

    template <typename UtilType, bool Untyped>
    constexpr void
            dynamic_memory_resource<UtilType, Untyped>::set_allocator(allocator_storage allocator) noexcept
    {
        m_alloc.set_allocator(allocator);
    }

    template <typename UtilType, bool Untyped>
    constexpr allocator_storage dynamic_memory_resource<UtilType, Untyped>::get_allocator() const noexcept
    {
        return m_alloc.get_allocator();
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::set_factory(const type_erased_factory& factory) noexcept
        requires(Untyped)
    {
        m_alloc.set_factory(factory);
    }

    template <typename UtilType, bool Untyped>
    constexpr type_erased_factory& dynamic_memory_resource<UtilType, Untyped>::get_factory() noexcept
        requires(Untyped)
    {
        return m_alloc.get_factory();
    }

    template <typename UtilType, bool Untyped>
    constexpr const type_erased_factory& dynamic_memory_resource<UtilType, Untyped>::get_factory() const noexcept
        requires(Untyped)
    {
        return m_alloc.get_factory();
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::allocate(const size_type count) noexcept
    {
        m_ptr = m_alloc.allocate(count);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::allocate(
            const size_type count,
            const size_type alignment
            )
        noexcept
    {
        m_ptr = m_alloc.allocate(count, alignment);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::reallocate(
            const size_type oldCount,
            const size_type newCount
            )
        noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount);
    }

    template <typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource<UtilType, Untyped>::reallocate(
                const size_type oldCount,
                const size_type newCount,
                const size_type alignment
                )
            noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.reallocate(get_ptr(), oldCount, newCount, alignment);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::deallocate(const size_type count) noexcept
    {
        m_alloc.deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource<UtilType, Untyped>::deallocate(
                const size_type count,
                const size_type alignment
                ) noexcept
    {
        m_alloc.deallocate(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::construct(
            const size_type count,
            const size_type offset
            )
        noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_alloc.construct(get_ptr_at(offset), count);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(is_nothrow_copy_constructible_v<UtilType>)
    {
        m_alloc.copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        m_alloc.move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <typename UtilType, bool Untyped>
    constexpr void
        dynamic_memory_resource<UtilType, Untyped>::destroy(
                const size_type count,
                const size_type offset
                ) noexcept
    {
        m_alloc.destroy(get_ptr_at(offset), count);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::allocate_and_construct(
            const size_type count
            )
        noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.allocate_and_construct(count);
    }

    template <typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource<UtilType, Untyped>::allocate_aligned_and_construct(
                const size_type count,
                const size_type alignment
                )
            noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
    }

    template <typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void
        dynamic_memory_resource<UtilType, Untyped>::reallocate_and_construct(
                const size_type oldCount,
                const size_type newCount
                )
            noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.reallocate_and_construct(get_ptr(), oldCount, newCount);
    }

    template <typename UtilType, bool Untyped>
    template <typename... Args>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            )
        noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.reallocate_aligned_and_construct(get_ptr(), oldCount, newCount, alignment);
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::destroy_and_deallocate(
            const size_type count
            ) noexcept
    {
        m_alloc.destroy_and_deallocate(get_ptr(), count);
        m_ptr = nullptr;
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::destroy_and_deallocate_aligned(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_alloc.destroy_and_deallocate_aligned(get_ptr(), count, alignment);
        m_ptr = nullptr;
    }

    template <typename UtilType, bool Untyped>
    constexpr UtilType* dynamic_memory_resource<UtilType, Untyped>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(m_ptr);
    }

    template <typename UtilType, bool Untyped>
    constexpr const UtilType* dynamic_memory_resource<UtilType, Untyped>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_ptr);
    }

    template <typename UtilType, bool Untyped>
    constexpr UtilType* dynamic_memory_resource<UtilType, Untyped>::get_ptr_at(const size_type offset) noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <typename UtilType, bool Untyped>
    constexpr const UtilType* dynamic_memory_resource<UtilType, Untyped>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <typename UtilType, bool Untyped>
    constexpr void dynamic_memory_resource<UtilType, Untyped>::set_ptr(UtilType* const& ptr) noexcept
    {
        m_ptr = ptr;
    }

    template <typename T>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T>::construct(
                size_type count,
                size_type offset,
                Args&&... args
                )
            noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_alloc.construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <typename T>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T>::allocate_and_construct(
            size_type count,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_and_construct(count, rsl::forward<Args>(args)...);
    }

    template <typename T>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T>::allocate_aligned_and_construct(
            size_type count,
            size_type alignment,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_aligned_and_construct(count, alignment, rsl::forward<Args>(args)...);
    }

    template <typename T>
    template <typename... Args>
    constexpr void
        typed_dynamic_memory_resource<T>::reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args
                )
            noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                rsl::forward<Args>(args)...
                );
    }

    template <typename T>
    template <typename... Args>
    constexpr void typed_dynamic_memory_resource<T>::reallocate_aligned_and_construct(
            size_type oldCount,
            size_type newCount,
            size_type alignment,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
    {
        base_type::m_ptr = base_type::m_alloc.reallocate_aligned_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                alignment,
                rsl::forward<Args>(args)...
                );
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::static_memory_resource(
            internal::alloc_and_factory_only_signal_type,
            const static_memory_resource& other
            ) noexcept
        : m_factory(other.m_factory)
    {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::static_memory_resource(
            const type_erased_factory& factory) noexcept
        requires(Untyped)
        : m_factory(factory)
    {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::set_factory(
            const type_erased_factory& factory) noexcept
        requires(Untyped)
    {
        m_factory = factory;
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr typename static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::factory_type&
            static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_factory() noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const typename static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::factory_type&
            static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_factory() const noexcept
    {
        return m_factory;
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::construct(
            const size_type count,
            const size_type offset
            ) noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_factory.construct(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(is_nothrow_copy_constructible_v<UtilType>)
    {
        m_factory.copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        m_factory.move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::destroy(
            const size_type count,
            const size_type offset
            ) noexcept
    {
        m_factory.destroy(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr() noexcept
    {
        return m_buffer.template get_data_ptr<UtilType>();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr() const noexcept
    {
        return m_buffer.template get_data_ptr<UtilType>();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr_at(
            const size_type offset
            ) noexcept
    {
        return advance(get_ptr(), offset * m_factory.type_size());
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* static_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_factory.type_size());
    }

    template <typename T, size_type BufferSize>
    template <typename... Args>
    constexpr void typed_static_memory_resource<T, BufferSize>::construct(
            const size_type count,
            const size_type offset,
            Args&&... args
            )
        noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_factory.construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::hybrid_memory_resource(
            internal::alloc_and_factory_only_signal_type,
            const hybrid_memory_resource& other
            ) noexcept
        : m_alloc(other.m_alloc) {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::hybrid_memory_resource(
            internal::alloc_and_factory_only_signal_type,
            hybrid_memory_resource&& other
            ) noexcept
        : m_alloc(rsl::move(other.m_alloc)) {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::hybrid_memory_resource(
            allocator_storage allocator
            ) noexcept : m_alloc(allocator) {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::hybrid_memory_resource(
            const type_erased_factory& factory) noexcept
        requires(Untyped)
        : m_alloc(factory)
    {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::hybrid_memory_resource(
            allocator_storage allocator,
            const type_erased_factory& factory) noexcept
        requires(Untyped)
        : m_alloc(allocator, factory)
    {}

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::set_allocator(
            allocator_storage allocator
            ) noexcept
    {
        m_alloc.set_allocator(allocator);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr allocator_storage hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_allocator() const noexcept
    {
        return m_alloc.get_allocator();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::set_factory(
            const type_erased_factory& factory) noexcept
        requires(Untyped)
    {
        m_alloc.set_factory(factory);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr type_erased_factory&
            hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_factory() noexcept
        requires(Untyped)
    {
        return m_alloc.get_factory();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const type_erased_factory&
            hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_factory() const noexcept
        requires(Untyped)
    {
        return m_alloc.get_factory();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::allocate(const size_type count) noexcept
    {
        m_ptr = m_alloc.allocate(count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::allocate(
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_ptr = m_alloc.allocate(count, alignment);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::reallocate(
            const size_type oldCount,
            const size_type newCount
            ) noexcept(is_nothrow_move_constructible_v<UtilType>)
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
                    m_alloc.move(get_ptr(), m_buffer.template get_data_ptr<UtilType>(), oldCount);
                }
            }
        }
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::reallocate(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(is_nothrow_move_constructible_v<UtilType>)
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
                    m_alloc.move(m_ptr, m_buffer.template get_data_ptr<UtilType>(), oldCount);
                }
            }
        }
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::deallocate(
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

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::deallocate(
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

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::construct(
            const size_type count,
            const size_type offset
            ) noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_alloc.construct(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::copy(
            const size_type count,
            const size_type offset,
            const void* src
            ) noexcept(is_nothrow_copy_constructible_v<UtilType>)
    {
        m_alloc.copy(get_ptr_at(offset), static_cast<const UtilType*>(src), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::move(
            const size_type count,
            const size_type offset,
            void* src
            ) noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        m_alloc.move(get_ptr_at(offset), static_cast<UtilType*>(src), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::destroy(
            const size_type count,
            const size_type offset
            ) noexcept
    {
        m_alloc.destroy(get_ptr_at(offset), count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::allocate_and_construct(
            const size_type count
            ) noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.allocate_and_construct(count);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    template <typename... Args>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment
            ) noexcept(is_nothrow_constructible_v<UtilType>)
    {
        m_ptr = m_alloc.allocate_aligned_and_construct(count, alignment);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    template <typename... Args>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::reallocate_and_construct(
            const size_type oldCount,
            const size_type newCount
            ) noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>)
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_ptr = m_alloc.reallocate_and_construct(get_ptr(), oldCount, newCount);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    template <typename... Args>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>)
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_ptr = m_alloc.reallocate_aligned_and_construct(get_ptr(), oldCount, newCount, alignment);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::destroy_and_deallocate(
            const size_type count
            ) noexcept
    {
        rsl_assert_invalid_operation(is_dynamic_memory());
        m_alloc.destroy_and_deallocate(get_ptr(), count);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::destroy_and_deallocate_aligned(
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

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr bool hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::is_static_memory() const noexcept
    {
        return m_ptr >= m_buffer.template get_data_ptr<UtilType>() && m_ptr < m_buffer.template get_data_ptr<UtilType>() + BufferSize;
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr bool hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::is_dynamic_memory() const noexcept
    {
        return !is_static_memory();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr() noexcept
    {
        return bit_cast<UtilType*>(m_ptr);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr() const noexcept
    {
        return bit_cast<const UtilType*>(m_ptr);
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr_at(
            const size_type offset
            ) noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_ptr_at(
            const size_type offset
            ) const noexcept
    {
        return advance(get_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::set_ptr(UtilType* const& ptr) noexcept
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

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_static_ptr() noexcept
    {
        return m_buffer.template get_data_ptr<UtilType>();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::
    get_static_ptr() const noexcept
    {
        return m_buffer.template get_data_ptr<UtilType>();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_static_ptr_at(
            const size_type offset
            ) noexcept
    {
        rsl_assert_out_of_range((offset * m_alloc.type_size()) < BufferSize);
        return advance(get_static_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr const UtilType* hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::get_static_ptr_at(
            const size_type offset
            ) const noexcept
    {
        rsl_assert_out_of_range((offset * m_alloc.type_size()) < BufferSize);
        return advance(get_static_ptr(), offset * m_alloc.type_size());
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::move_to_static_memory_and_deallocate(
            const size_type count,
            const size_type memoryCount
            )
        noexcept(is_nothrow_move_constructible_v<UtilType>)
    {
        rsl_assert_invalid_operation(count * m_alloc.type_size() <= BufferSize);
        UtilType* dynamicPtr = get_ptr();
        m_alloc.move(get_static_ptr(), dynamicPtr, count);
        m_alloc.destroy(dynamicPtr, count);
        m_alloc.deallocate(dynamicPtr, memoryCount);
        set_ptr_to_static_memory();
    }

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment>
    constexpr void hybrid_memory_resource<BufferSize, UtilType, Untyped, Alignment>::set_ptr_to_static_memory() noexcept
    {
        set_ptr(m_buffer.template get_data_ptr<UtilType>());
    }

    template <typename T, size_type BufferCount>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount>::construct(
            const size_type count,
            const size_type offset,
            Args&&... args
            ) noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_alloc.construct(base_type::get_ptr_at(offset), count, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount>::allocate_and_construct(
            const size_type count,
            Args&&... args
            ) noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_and_construct(count, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment,
            Args&&... args
            ) noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        base_type::m_ptr = base_type::m_alloc.allocate_aligned_and_construct(count, alignment, rsl::forward<Args>(args)...);
    }

    template <typename T, size_type BufferCount>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount>::reallocate_and_construct(
            const size_type oldCount,
            const size_type newCount,
            Args&&... args
            ) noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
    {
        rsl_assert_invalid_operation(base_type::is_dynamic_memory());
        base_type::m_ptr = base_type::m_alloc.reallocate_and_construct(
                base_type::get_ptr(),
                oldCount,
                newCount,
                rsl::forward<Args>(args)...
                );
    }

    template <typename T, size_type BufferCount>
    template <typename... Args>
    constexpr void typed_hybrid_memory_resource<T, BufferCount>::reallocate_aligned_and_construct(
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment,
            Args&&... args
            ) noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
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
