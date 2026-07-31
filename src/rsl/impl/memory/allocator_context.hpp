#pragma once

#include "typed_allocator.hpp"

namespace rsl
{
    struct allocator_context
    {
        static heap_allocator defaultGlobalAllocator;
        static allocator_storage globalAllocator;
        static heap_allocator defaultThreadSpecificAllocator;
        static thread_local allocator_storage threadSpecificAllocator;
    };

    struct global_allocator_scope
    {
        global_allocator_scope(allocator_storage allocator)
            : m_previousAllocator(allocator_context::globalAllocator)
        {
            allocator_context::globalAllocator = allocator;
        }
        global_allocator_scope(const global_allocator_scope&) = delete;
        ~global_allocator_scope() { allocator_context::globalAllocator = m_previousAllocator; }

    private:
        allocator_storage m_previousAllocator;
    };

    struct thread_local_allocator_scope
    {
        thread_local_allocator_scope(allocator_storage allocator)
            : m_previousAllocator(allocator_context::threadSpecificAllocator)
        {
            allocator_context::threadSpecificAllocator = allocator;
        }
        thread_local_allocator_scope(const thread_local_allocator_scope&) = delete;
        ~thread_local_allocator_scope() { allocator_context::threadSpecificAllocator = m_previousAllocator; }

    private:
        allocator_storage m_previousAllocator;
    };

    template <typename T, typename... Args>
    [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
    allocate(memory_allocator& alloc, size_type count = 1, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);

    template <typename T, typename... Args>
    [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
    allocate(memory_allocator& alloc, size_type count, size_type alignment, Args&&... args)
        noexcept(is_nothrow_constructible_v<T, Args...>);

    template <typename T, typename... Args>
    [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
    reallocate(memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args)
        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

    template <typename T, typename... Args>
    [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* reallocate(
        memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
    ) noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

    template <typename T>
    [[rythe_always_inline]] void deallocate(memory_allocator& alloc, T* ptr, size_type count = 1) noexcept;

    template <typename T>
    [[rythe_always_inline]] void
    deallocate(memory_allocator& alloc, T* ptr, size_type count, size_type alignment) noexcept;
} // namespace rsl

#include "allocator_context.inl"
