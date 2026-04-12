#pragma once
#include <new>

#include "../util/concepts.hpp"

namespace rsl
{
    class memory_allocator
    {
    public:
        virtual bool is_valid() const noexcept { return true; } // NOLINT

        virtual ~memory_allocator() = default;
        [[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size) noexcept = 0;
        [[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size, size_type alignment) noexcept = 0;

        [[nodiscard]] [[rythe_allocating]] virtual void* reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept = 0;
        [[nodiscard]] [[rythe_allocating]] virtual void*
                reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept = 0;

        virtual void deallocate(void* ptr, size_type size) noexcept = 0;
        virtual void deallocate(void* ptr, size_type size, size_type alignment) noexcept = 0;

        template <not_same_as<void> T>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* allocate() noexcept
        {
            return static_cast<T*>(allocate(sizeof(T), alignof(T)));
        }

        template <not_same_as<void> T>
        [[rythe_always_inline]] void deallocate(T* ptr) noexcept
        {
            deallocate(static_cast<void*>(ptr), sizeof(T), alignof(T));
        }
    };

    class heap_allocator : public memory_allocator
    {
    public:
        using value_type = void;

        [[nodiscard]] [[rythe_allocating]] void* allocate(size_type size) noexcept override;
        [[nodiscard]] [[rythe_allocating]] void* allocate(size_type size, size_type alignment) noexcept override;

        [[nodiscard]] [[rythe_allocating]] void* reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept override;
        [[nodiscard]] [[rythe_allocating]] void*
                reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept override;

        void deallocate(void* ptr, size_type size) noexcept override;
        void deallocate(void* ptr, size_type size, size_type alignment) noexcept override;
    };

#if !defined(RSL_DEFAULT_ALLOCATOR_OVERRIDE)
    using default_allocator = heap_allocator;
#else
    using default_allocator = RSL_DEFAULT_ALLOCATOR_OVERRIDE;
#endif
} // namespace rsl
