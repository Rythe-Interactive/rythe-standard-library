#pragma once

namespace rsl
{
    template <typename TypedAllocator, typename T, factory_type Factory>
    [[nodiscard]] [[rythe_always_inline]] constexpr size_type typed_allocator_impl<
        TypedAllocator, T, Factory>::type_size() const noexcept
    {
        return self().get_factory().type_size();
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::allocate(
            const size_type count
            ) noexcept
    {
        return bit_cast<value_type*>(self().get_allocator().allocate(count * type_size()));
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::allocate(
            const size_type count,
            size_type alignment
            ) noexcept
    {
        return bit_cast<value_type*>(self().get_allocator().allocate(count * type_size(), alignment));
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::reallocate(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
            value_type* mem =
                    bit_cast<value_type*>(self().get_allocator().reallocate(ptr, oldCount * type_size(), newCount * type_size()));

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount);
                if (mem) [[likely]]
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount);

            return mem;
        }
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr typename typed_allocator_impl<TypedAllocator, T, Factory>::value_type* typed_allocator_impl<
        TypedAllocator, T, Factory>::reallocate(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
            value_type* mem = static_cast<value_type*>(
                    self().get_allocator().reallocate(
                    ptr,
                    oldCount * type_size(),
                    newCount * type_size(),
                    alignment
                    ));

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount, alignment);
                if (mem) [[likely]]
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount, alignment);

            return mem;
        }
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr void typed_allocator_impl<TypedAllocator, T, Factory>::deallocate(value_type* ptr, const size_type count) noexcept
    {
        self().get_allocator().deallocate(ptr, count * type_size());
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr void typed_allocator_impl<TypedAllocator, T, Factory>::deallocate(
            value_type* ptr,
            const size_type count,
            size_type alignment
            ) noexcept
    {
        self().get_allocator().deallocate(ptr, count * type_size(), alignment);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    template <typename... Args>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::construct(
            value_type* ptr,
            const size_type count,
            Args&&... args
            )
        noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        return self().get_factory().construct(ptr, count, rsl::forward<Args>(args)...);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::copy(
            value_type* dst,
            const value_type* src,
            const size_type count
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        return self().get_factory().copy(dst, src, count);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::move(
            value_type* dst,
            value_type* src,
            const size_type count
            )
        noexcept(factory_traits<factory_t>::noexcept_moveable)
    {
        return self().get_factory().move(dst, src, count);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr void typed_allocator_impl<TypedAllocator, T, Factory>::destroy(value_type* ptr, size_type count) noexcept
    {
        self().get_factory().destroy(ptr, count);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    template <typename... Args>
    constexpr T*
    typed_allocator_impl<TypedAllocator, T, Factory>::allocate_and_construct(
            const size_type count,
            Args&&... args
            )
        noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        value_type* mem = allocate(count);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    template <typename... Args>
    constexpr T* typed_allocator_impl<
        TypedAllocator, T, Factory>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment,
            Args&&... args
            ) noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>)
    {
        value_type* mem = allocate(count, alignment);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    template <typename... Args>
    constexpr T* typed_allocator_impl<TypedAllocator, T, Factory>::reallocate_and_construct(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount,
            Args&&... args
            )
        noexcept(
            factory_traits<factory_t>::template noexcept_constructable<Args...> &&
            factory_traits<factory_t>::noexcept_moveable
        )
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
            value_type* mem =
                    static_cast<value_type*>(self().get_allocator().reallocate(ptr, oldCount * type_size(), newCount * type_size()));

            if (newCount > oldCount) [[likely]]
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount);
                if (mem) [[likely]]
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount);

            if (newCount > oldCount) [[likely]]
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    template <typename... Args>
    constexpr T* typed_allocator_impl<
        TypedAllocator, T, Factory>::reallocate_aligned_and_construct(
            value_type* ptr,
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment,
            Args&&... args
            )
        noexcept(
            factory_traits<factory_t>::template noexcept_constructable<Args...> &&
            factory_traits<factory_t>::noexcept_moveable
        )
    {
        if constexpr (is_trivially_copyable_v<value_type>)
        {
            value_type* mem = static_cast<value_type*>(
                    self().get_allocator().reallocate(
                    ptr,
                    oldCount * type_size(),
                    newCount * type_size(),
                    alignment
                    ));

            if (newCount > oldCount) [[likely]]
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
        else
        {
            value_type* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount, alignment);
                if (mem) [[likely]]
                {
                    move(mem, ptr, oldCount);
                }
            }

            deallocate(ptr, oldCount, alignment);

            if (newCount > oldCount) [[likely]]
            {
                construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...);
            }

            return mem;
        }
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr void typed_allocator_impl<TypedAllocator, T, Factory>::destroy_and_deallocate(
            value_type* ptr,
            const size_type count
            ) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr void typed_allocator_impl<TypedAllocator, T, Factory>::destroy_and_deallocate_aligned(
            value_type* ptr,
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count, alignment);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr TypedAllocator& typed_allocator_impl<TypedAllocator, T, Factory>::self() noexcept
    {
        return static_cast<TypedAllocator&>(*this);
    }

    template <typename TypedAllocator, typename T, factory_type Factory>
    constexpr const TypedAllocator& typed_allocator_impl<TypedAllocator, T, Factory>::self() const noexcept
    {
        return static_cast<const TypedAllocator&>(*this);
    }

    template <untyped_factory_type Factory>
    template <typename T>
    type_erased_allocator<
        Factory>::type_erased_allocator(
            pointer<memory_allocator> baseAllocator,
            construct_type_signal_type<T>
            )
        noexcept(is_nothrow_constructible_v<factory_t, construct_type_signal_type<T>> && is_nothrow_constructible_v<factory_storage_type, factory_t&&>)
        : m_factory(factory_t(construct_type_signal<T>)),
          m_alloc(baseAllocator)
    {}

    template <untyped_factory_type Factory>
    type_erased_allocator<Factory>::type_erased_allocator(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
        : m_factory(factoryStorage),
          m_alloc() {}

    template <untyped_factory_type Factory>
    type_erased_allocator<Factory>::type_erased_allocator(
            pointer<memory_allocator> baseAllocator,
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>
    )
        : m_factory(factoryStorage),
          m_alloc(baseAllocator)
    {}

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::set_allocator(pointer<memory_allocator> baseAllocator) noexcept
    {
        m_alloc = baseAllocator;
    }

    template <untyped_factory_type Factory>
    constexpr memory_allocator& type_erased_allocator<Factory>::get_allocator() noexcept
    {
        return *m_alloc;
    }

    template <untyped_factory_type Factory>
    constexpr const memory_allocator& type_erased_allocator<Factory>::get_allocator() const noexcept
    {
        return *m_alloc;
    }

    template <untyped_factory_type Factory>
    constexpr pointer<memory_allocator> type_erased_allocator<Factory>::get_allocator_storage() noexcept
    {
        return m_alloc;
    }

    template <untyped_factory_type Factory>
    constexpr pointer<const memory_allocator> type_erased_allocator<Factory>::get_allocator_storage() const noexcept
    {
        return m_alloc;
    }

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::set_factory(
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_factory = factoryStorage;
    }

    template <untyped_factory_type Factory>
    constexpr Factory& type_erased_allocator<Factory>::get_factory() noexcept
    {
        return *m_factory;
    }

    template <untyped_factory_type Factory>
    constexpr const Factory& type_erased_allocator<Factory>::get_factory() const noexcept
    {
        return *m_factory;
    }

    template <untyped_factory_type Factory>
    size_type type_erased_allocator<Factory>::type_size() const noexcept
    {
        return m_factory->type_size();
    }

    template <untyped_factory_type Factory>
    bool type_erased_allocator<Factory>::trivial_copy() const noexcept
    {
        return can_trivially_copy(*m_factory);
    }

    template <untyped_factory_type Factory>
    id_type type_erased_allocator<Factory>::type_id() const noexcept
    {
        return m_factory->type_id();
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::allocate(const size_type count) noexcept
    {
        return m_alloc->allocate(count * type_size());
    }

    template <untyped_factory_type Factory>
    constexpr void*
    type_erased_allocator<Factory>::allocate(const size_type count, const size_type alignment) noexcept
    {
        return m_alloc->allocate(count * type_size(), alignment);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::reallocate(
            void* ptr,
            const size_type oldCount,
            const size_type newCount
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        const size_type typeSize = type_size();
        const bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize);
        }

        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize);
            if (mem) [[likely]]
            {
                m_factory->move(mem, ptr, oldCount);
            }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize);

        return mem;
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::reallocate(
            void* ptr,
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        size_type typeSize = type_size();
        bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment);
        }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize, alignment);
            if (mem) [[likely]]
            {
                m_factory->move(mem, ptr, oldCount);
            }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize, alignment);

        return mem;
    }

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::deallocate(void* ptr, const size_type count) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size());
    }

    template <untyped_factory_type Factory>
    constexpr void
        type_erased_allocator<Factory>::deallocate(void* ptr, const size_type count, const size_type alignment) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::construct(
            void* ptr,
            const size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        return m_factory->construct(ptr, count);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::copy(
            void* dst,
            const void* src,
            const size_type count
            )
        noexcept(factory_traits<Factory>::noexcept_copyable)
    {
        return m_factory->copy(dst, src, count);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::move(
            void* dst,
            void* src,
            const size_type count
            )
        noexcept(factory_traits<Factory>::noexcept_moveable)
    {
        return m_factory->move(dst, src, count);
    }

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::destroy(void* ptr, size_type count) noexcept
    {
        m_factory->destroy(ptr, count);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::allocate_and_construct(
            const size_type count
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        void* mem = m_alloc->allocate(count * type_size());
        return m_factory->construct(mem, count);
    }

    template <untyped_factory_type Factory>
    constexpr void*
    type_erased_allocator<Factory>::allocate_aligned_and_construct(
            const size_type count,
            const size_type alignment
            )
        noexcept(factory_traits<Factory>::template noexcept_constructable<>)
    {
        void* mem = m_alloc->allocate(count * type_size(), alignment);
        return m_factory->construct(mem, count);
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::reallocate_and_construct(
            void* ptr,
            const size_type oldCount,
            const size_type newCount
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        const size_type typeSize = type_size();
        bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize);

            if (newCount > oldCount) [[likely]]
            {
                m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
            }

            return mem;
        }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize);
            if (mem) [[likely]]
            {
                m_factory->move(mem, ptr, oldCount);
            }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize);

        if (newCount > oldCount) [[likely]]
        {
            m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
        }

        return mem;
    }

    template <untyped_factory_type Factory>
    constexpr void* type_erased_allocator<Factory>::reallocate_aligned_and_construct(
            void* ptr,
            const size_type oldCount,
            const size_type newCount,
            const size_type alignment
            ) noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable)
    {
        size_type typeSize = type_size();
        bool canTriviallyCopy = can_trivially_copy(*m_factory);

        if (canTriviallyCopy)
        {
            void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment);

            if (newCount > oldCount) [[likely]]
            {
                m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
            }

            return mem;
        }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize, alignment);
            if (mem) [[likely]]
            {
                m_factory->move(mem, ptr, oldCount);
            }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize, alignment);

        if (newCount > oldCount) [[likely]]
        {
            m_factory->construct(advance(mem, oldCount * typeSize), oldCount - newCount);
        }

        return mem;
    }

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::destroy_and_deallocate(void* ptr, size_type count) noexcept
    {
        m_factory->destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size());
    }

    template <untyped_factory_type Factory>
    constexpr void type_erased_allocator<Factory>::destroy_and_deallocate_aligned(
            void* ptr,
            const size_type count,
            const size_type alignment
            ) noexcept
    {
        m_factory->destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }

    template <untyped_factory_type Factory>
    constexpr factory_storage<Factory>& type_erased_allocator<Factory>::get_factory_storage() noexcept
    {
        return m_factory;
    }

    template <untyped_factory_type Factory>
    constexpr const factory_storage<Factory>& type_erased_allocator<Factory>::get_factory_storage() const noexcept
    {
        return m_factory;
    }
} // namespace rsl
