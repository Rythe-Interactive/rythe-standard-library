#pragma once
#include "allocator_context.hpp"

namespace rsl
{
    template <typename T, typename... Args>
    T* allocate(memory_allocator& alloc, size_type count, Args&&... args)
        noexcept(default_factory<T>::template noexcept_constructable<Args...>)
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.allocate(count, forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T* allocate(memory_allocator& alloc, size_type count, size_type alignment, Args&&... args)
        noexcept(default_factory<T>::template noexcept_constructable<Args...>)
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.allocate(count, alignment, forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T* reallocate(memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args)
        noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable)
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.reallocate(ptr, oldCount, newCount, forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T* reallocate(
        memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
    ) noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable)
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.reallocate(ptr, oldCount, newCount, alignment, forward<Args>(args)...);
    }

    template <typename T>
    void deallocate(memory_allocator& alloc, T* ptr, size_type count) noexcept
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.deallocate(ptr, count);
    }

    template <typename T>
    void deallocate(memory_allocator& alloc, T* ptr, size_type count, size_type alignment) noexcept
    {
        using alloc_type = typed_allocator<T>;

        alloc_type wrappedAlloc{&alloc};
        return wrappedAlloc.deallocate(ptr, count, alignment);
    }
} // namespace rsl
