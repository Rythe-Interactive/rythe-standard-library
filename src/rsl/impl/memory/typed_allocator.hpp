#pragma once

#include "allocator.hpp"
#include "factory.hpp"

namespace rsl
{
    template <typename T>
    class typed_allocator final
    {
    public:
        [[rythe_always_inline]] constexpr explicit typed_allocator(allocator_storage baseAllocator) noexcept
            : m_alloc(baseAllocator)
        {}

        [[rythe_always_inline]] constexpr explicit typed_allocator(allocator_storage baseAllocator, construct_type_signal_type<T>) noexcept
            : typed_allocator(baseAllocator)
        {}

        [[nodiscard]] [[rythe_always_inline]] bool operator==(const typed_allocator&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const typed_allocator&) const noexcept = default;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T* allocate(size_type count = 1) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                allocate(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                reallocate(T* ptr, size_type oldCount, size_type newCount) noexcept(is_nothrow_move_constructible_v<T>);
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                reallocate(T* ptr, size_type oldCount, size_type newCount, size_type alignment)
                        noexcept(is_nothrow_move_constructible_v<T>);

        [[rythe_always_inline]] constexpr void deallocate(T* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(T* ptr, size_type count, size_type alignment) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr T* construct(T* ptr, size_type count = 1, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);
        [[rythe_always_inline]] constexpr T* copy(T* dst, const T* src, size_type count = 1)
                noexcept(is_nothrow_move_constructible_v<T>);
        [[rythe_always_inline]] constexpr T* move(T* dst, T* src, size_type count = 1)
                noexcept(is_nothrow_move_constructible_v<T>);
        [[rythe_always_inline]] constexpr void destroy(T* ptr, size_type count = 1) noexcept;

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                allocate_and_construct(size_type count = 1, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                allocate_aligned_and_construct(size_type count, size_type alignment, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T*
                reallocate_and_construct(T* ptr, size_type oldCount, size_type newCount, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr T* reallocate_aligned_and_construct(
                T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(T* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
                destroy_and_deallocate_aligned(T* ptr, size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage baseAllocator) noexcept { m_alloc = baseAllocator; }
        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator() const noexcept { return m_alloc; }

        [[nodiscard]] [[rythe_always_inline]] size_type type_size() const noexcept { return sizeof(T); }

    private:
        allocator_storage m_alloc;
    };

    class type_erased_allocator
    {
    public:
        [[rythe_always_inline]] constexpr type_erased_allocator() noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const type_erased_allocator&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const type_erased_allocator&) const noexcept = default;

        template <typename T>
        [[rythe_always_inline]] constexpr type_erased_allocator(
                allocator_storage baseAllocator, construct_type_signal_type<T>) noexcept;
        [[rythe_always_inline]] constexpr type_erased_allocator(const type_erased_factory& factory) noexcept;
        [[rythe_always_inline]] constexpr type_erased_allocator(
                allocator_storage baseAllocator, const type_erased_factory& factory) noexcept;

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage baseAllocator) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(const type_erased_factory& factoryStorage) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr type_erased_factory& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const type_erased_factory& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] size_type type_size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] bool trivial_copy() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] id_type type_id() const noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void* allocate(size_type count = 1) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
                allocate(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
                reallocate(void* ptr, size_type oldCount, size_type newCount);
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
                reallocate(void* ptr, size_type oldCount, size_type newCount, size_type alignment);

        [[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] void* construct(void* ptr, size_type count = 1);
        [[rythe_always_inline]] void* copy(void* dst, const void* src, size_type count = 1);
        [[rythe_always_inline]] void* move(void* dst, void* src, size_type count = 1);
        [[rythe_always_inline]] void destroy(void* ptr, size_type count = 1) noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate_and_construct(size_type count = 1);

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
                allocate_aligned_and_construct(size_type count, size_type alignment);

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
                reallocate_and_construct(void* ptr, size_type oldCount, size_type newCount);

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
                reallocate_aligned_and_construct(void* ptr, size_type oldCount, size_type newCount, size_type alignment);

        [[rythe_always_inline]] void destroy_and_deallocate(void* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] void
                destroy_and_deallocate_aligned(void* ptr, size_type count, size_type alignment) noexcept;

    private:
        type_erased_factory m_factory;
        allocator_storage m_alloc;
    };

    namespace internal
    {
        template <typename T, bool Untyped = false>
        struct select_typed_allocator;

        template <typename T>
        struct select_typed_allocator<T, false>
        {
            using type = typed_allocator<T>;
            constexpr static bool is_untyped = false;
        };

        template <typename T>
        struct select_typed_allocator<T, true>
        {
            using type = type_erased_allocator;
            constexpr static bool is_untyped = true;
        };
    } // namespace internal
} // namespace rsl

#include "typed_allocator.inl"
