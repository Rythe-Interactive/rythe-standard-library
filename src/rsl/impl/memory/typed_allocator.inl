#pragma once

namespace rsl
{
    template <typename T>
    constexpr T* typed_allocator<T>::allocate(const size_type count) noexcept
    {
        return bit_cast<T*>(m_alloc->allocate(count * sizeof(T)));
    }

    template <typename T>
    constexpr T* typed_allocator<T>::allocate(const size_type count, size_type alignment) noexcept
    {
        return bit_cast<T*>(m_alloc->allocate(count * sizeof(T), alignment));
    }

    template <typename T>
    constexpr T* typed_allocator<T>::reallocate(T* ptr, const size_type oldCount, const size_type newCount)
            noexcept(is_nothrow_move_constructible_v<T>)
    {
        if constexpr (is_trivially_copyable_v<T>)
        {
            T* mem = bit_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T)));

            return mem;
        }
        else
        {
            T* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount);
                if (mem) [[likely]] { move(mem, ptr, oldCount); }
            }

            deallocate(ptr, oldCount);

            return mem;
        }
    }

    template <typename T>
    constexpr T* typed_allocator<T>::reallocate(T* ptr, const size_type oldCount, const size_type newCount, const size_type alignment)
            noexcept(is_nothrow_move_constructible_v<T>)
    {
        if constexpr (is_trivially_copyable_v<T>)
        {
            T* mem = static_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T), alignment));

            return mem;
        }
        else
        {
            T* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount, alignment);
                if (mem) [[likely]] { move(mem, ptr, oldCount); }
            }

            deallocate(ptr, oldCount, alignment);

            return mem;
        }
    }

    template <typename T>
    constexpr void typed_allocator<T>::deallocate(T* ptr, const size_type count) noexcept
    {
        m_alloc->deallocate(ptr, count * sizeof(T));
    }

    template <typename T>
    constexpr void typed_allocator<T>::deallocate(T* ptr, const size_type count, size_type alignment) noexcept
    {
        m_alloc->deallocate(ptr, count * sizeof(T), alignment);
    }

    template <typename T>
    template <typename... Args>
    constexpr T* typed_allocator<T>::construct(T* ptr, const size_type count, Args&&... args)
            noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        return factory<T>::construct(ptr, count, rsl::forward<Args>(args)...);
    }

    template <typename T>
    constexpr T* typed_allocator<T>::copy(T* dst, const T* src, const size_type count) noexcept(is_nothrow_move_constructible_v<T>)
    {
        return factory<T>::copy(dst, src, count);
    }

    template <typename T>
    constexpr T* typed_allocator<T>::move(T* dst, T* src, const size_type count) noexcept(is_nothrow_move_constructible_v<T>)
    {
        return factory<T>::move(dst, src, count);
    }

    template <typename T>
    constexpr void typed_allocator<T>::destroy(T* ptr, size_type count) noexcept
    {
        factory<T>::destroy(ptr, count);
    }

    template <typename T>
    template <typename... Args>
    constexpr T* typed_allocator<T>::allocate_and_construct(const size_type count, Args&&... args)
            noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        T* mem = allocate(count);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename T>
    template <typename... Args>
    constexpr T* typed_allocator<T>::allocate_aligned_and_construct(const size_type count, const size_type alignment, Args&&... args)
            noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        T* mem = allocate(count, alignment);
        return construct(mem, count, forward<Args>(args)...);
    }

    template <typename T>
    template <typename... Args>
    constexpr T*
            typed_allocator<T>::reallocate_and_construct(T* ptr, const size_type oldCount, const size_type newCount, Args&&... args)
                    noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
    {
        if constexpr (is_trivially_copyable_v<T>)
        {
            T* mem = static_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T)));

            if (newCount > oldCount) [[likely]] { construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...); }

            return mem;
        }
        else
        {
            T* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount);
                if (mem) [[likely]] { move(mem, ptr, oldCount); }
            }

            deallocate(ptr, oldCount);

            if (newCount > oldCount) [[likely]] { construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...); }

            return mem;
        }
    }

    template <typename T>
    template <typename... Args>
    constexpr T* typed_allocator<T>::reallocate_aligned_and_construct(
            T* ptr, const size_type oldCount, const size_type newCount, const size_type alignment, Args&&... args)
            noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>)
    {
        if constexpr (is_trivially_copyable_v<T>)
        {
            T* mem = static_cast<T*>(m_alloc->reallocate(ptr, oldCount * sizeof(T), newCount * sizeof(T), alignment));

            if (newCount > oldCount) [[likely]] { construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...); }

            return mem;
        }
        else
        {
            T* mem = nullptr;

            if (newCount != 0) [[likely]]
            {
                mem = allocate(newCount, alignment);
                if (mem) [[likely]] { move(mem, ptr, oldCount); }
            }

            deallocate(ptr, oldCount, alignment);

            if (newCount > oldCount) [[likely]] { construct(mem + oldCount, oldCount - newCount, forward<Args>(args)...); }

            return mem;
        }
    }

    template <typename T>
    constexpr void typed_allocator<T>::destroy_and_deallocate(T* ptr, const size_type count) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count);
    }

    template <typename T>
    constexpr void
            typed_allocator<T>::destroy_and_deallocate_aligned(T* ptr, const size_type count, const size_type alignment) noexcept
    {
        destroy(ptr, count);
        deallocate(ptr, count, alignment);
    }

    template <typename T>
    constexpr type_erased_allocator::type_erased_allocator(allocator_storage baseAllocator, construct_type_signal_type<T>) noexcept
        : m_factory(construct_type_signal<T>),
          m_alloc(baseAllocator)
    {}

    constexpr type_erased_allocator::type_erased_allocator(const type_erased_factory& factoryStorage) noexcept
        : m_factory(factoryStorage),
          m_alloc()
    {}

    constexpr type_erased_allocator::type_erased_allocator(
            allocator_storage baseAllocator, const type_erased_factory& factoryStorage) noexcept
        : m_factory(factoryStorage),
          m_alloc(baseAllocator)
    {}

    constexpr void type_erased_allocator::set_allocator(allocator_storage baseAllocator) noexcept
    {
        m_alloc = baseAllocator;
    }

    constexpr allocator_storage type_erased_allocator::get_allocator() const noexcept
    {
        return m_alloc;
    }

    constexpr void type_erased_allocator::set_factory(const type_erased_factory& factoryStorage)
            noexcept(is_nothrow_copy_assignable_v<type_erased_factory>)
    {
        m_factory = factoryStorage;
    }

    constexpr type_erased_factory& type_erased_allocator::get_factory() noexcept
    {
        return m_factory;
    }

    constexpr const type_erased_factory& type_erased_allocator::get_factory() const noexcept
    {
        return m_factory;
    }

    inline size_type type_erased_allocator::type_size() const noexcept
    {
        return m_factory.type_size();
    }

    inline bool type_erased_allocator::trivial_copy() const noexcept
    {
        return m_factory.trivial_copy();
    }

    inline id_type type_erased_allocator::type_id() const noexcept
    {
        return m_factory.type_id();
    }

    constexpr void* type_erased_allocator::allocate(const size_type count) noexcept
    {
        return m_alloc->allocate(count * type_size());
    }

    constexpr void* type_erased_allocator::allocate(const size_type count, const size_type alignment) noexcept
    {
        return m_alloc->allocate(count * type_size(), alignment);
    }

    inline void* type_erased_allocator::reallocate(void* ptr, const size_type oldCount, const size_type newCount)
    {
        const size_type typeSize = type_size();
        const bool canTriviallyCopy = m_factory.trivial_copy();

        if (canTriviallyCopy) { return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize); }

        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize);
            if (mem) [[likely]] { m_factory.move(mem, ptr, oldCount); }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize);

        return mem;
    }

    inline void*
            type_erased_allocator::reallocate(void* ptr, const size_type oldCount, const size_type newCount, const size_type alignment)
    {
        size_type typeSize = type_size();
        bool canTriviallyCopy = m_factory.trivial_copy();

        if (canTriviallyCopy) { return m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment); }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize, alignment);
            if (mem) [[likely]] { m_factory.move(mem, ptr, oldCount); }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize, alignment);

        return mem;
    }

    constexpr void type_erased_allocator::deallocate(void* ptr, const size_type count) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size());
    }

    constexpr void type_erased_allocator::deallocate(void* ptr, const size_type count, const size_type alignment) noexcept
    {
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }

    inline void* type_erased_allocator::construct(void* ptr, const size_type count)
    {
        return m_factory.construct(ptr, count);
    }

    inline void* type_erased_allocator::copy(void* dst, const void* src, const size_type count)
    {
        return m_factory.copy(dst, src, count);
    }

    inline void* type_erased_allocator::move(void* dst, void* src, const size_type count)
    {
        return m_factory.move(dst, src, count);
    }

    inline void type_erased_allocator::destroy(void* ptr, size_type count) noexcept
    {
        m_factory.destroy(ptr, count);
    }

    inline void* type_erased_allocator::allocate_and_construct(const size_type count)
    {
        void* mem = m_alloc->allocate(count * type_size());
        return m_factory.construct(mem, count);
    }

    inline void* type_erased_allocator::allocate_aligned_and_construct(const size_type count, const size_type alignment)
    {
        void* mem = m_alloc->allocate(count * type_size(), alignment);
        return m_factory.construct(mem, count);
    }

    inline void* type_erased_allocator::reallocate_and_construct(void* ptr, const size_type oldCount, const size_type newCount)
    {
        const size_type typeSize = type_size();
        bool canTriviallyCopy = m_factory.trivial_copy();

        if (canTriviallyCopy)
        {
            void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize);

            if (newCount > oldCount) [[likely]] { m_factory.construct(advance(mem, oldCount * typeSize), oldCount - newCount); }

            return mem;
        }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize);
            if (mem) [[likely]] { m_factory.move(mem, ptr, oldCount); }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize);

        if (newCount > oldCount) [[likely]] { m_factory.construct(advance(mem, oldCount * typeSize), oldCount - newCount); }

        return mem;
    }

    inline void* type_erased_allocator::reallocate_aligned_and_construct(
            void* ptr, const size_type oldCount, const size_type newCount, const size_type alignment)
    {
        size_type typeSize = type_size();
        bool canTriviallyCopy = m_factory.trivial_copy();

        if (canTriviallyCopy)
        {
            void* mem = m_alloc->reallocate(ptr, oldCount * typeSize, newCount * typeSize, alignment);

            if (newCount > oldCount) [[likely]] { m_factory.construct(advance(mem, oldCount * typeSize), oldCount - newCount); }

            return mem;
        }
        void* mem = nullptr;

        if (newCount != 0) [[likely]]
        {
            mem = m_alloc->allocate(newCount * typeSize, alignment);
            if (mem) [[likely]] { m_factory.move(mem, ptr, oldCount); }
        }

        m_alloc->deallocate(ptr, oldCount * typeSize, alignment);

        if (newCount > oldCount) [[likely]] { m_factory.construct(advance(mem, oldCount * typeSize), oldCount - newCount); }

        return mem;
    }

    inline void type_erased_allocator::destroy_and_deallocate(void* ptr, size_type count) noexcept
    {
        m_factory.destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size());
    }

    inline void
            type_erased_allocator::destroy_and_deallocate_aligned(void* ptr, const size_type count, const size_type alignment) noexcept
    {
        m_factory.destroy(ptr, count);
        m_alloc->deallocate(ptr, count * type_size(), alignment);
    }
} // namespace rsl
