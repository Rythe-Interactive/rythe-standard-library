#pragma once

#include "pointer.hpp"
#include "factory_storage.hpp"

namespace rsl
{
    template <typename TypedAllocator, typename T, factory_type Factory>
    class typed_allocator_impl
    {
    public:
        using value_type = T;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        
        [[nodiscard]] [[rythe_always_inline]] bool operator==(const typed_allocator_impl&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const typed_allocator_impl&) const noexcept = default;
        
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type type_size() const noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type* allocate(size_type count = 1) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        allocate(size_type count, size_type alignment) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        reallocate(value_type* ptr, size_type oldCount, size_type newCount) noexcept(factory_traits<factory_t>::noexcept_moveable);
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        reallocate(
                value_type* ptr,
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(factory_traits<factory_t>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(value_type* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(value_type* ptr, size_type count, size_type alignment) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr value_type* construct(
                value_type* ptr,
                size_type count = 1,
                Args&&... args
                )
            noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>);
        [[rythe_always_inline]] constexpr value_type* copy(
                value_type* dst,
                const value_type* src,
                size_type count = 1
                )
            noexcept(factory_traits<factory_t>::noexcept_moveable);
        [[rythe_always_inline]] constexpr value_type* move(
                value_type* dst,
                value_type* src,
                size_type count = 1
                )
            noexcept(factory_traits<factory_t>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void destroy(value_type* ptr, size_type count = 1) noexcept;

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        allocate_and_construct(
                size_type count = 1,
                Args&&... args
                )
            noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        allocate_aligned_and_construct(
                size_type count,
                size_type alignment,
                Args&&... args
                )
            noexcept(factory_traits<factory_t>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type*
        reallocate_and_construct(
                value_type* ptr,
                size_type oldCount,
                size_type newCount,
                Args&&... args
                ) noexcept(
            factory_traits<factory_t>::template noexcept_constructable<Args...> &&
            factory_traits<factory_t>::noexcept_moveable
        );

        template <typename... Args>
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr value_type* reallocate_aligned_and_construct(
                value_type* ptr,
                size_type oldCount,
                size_type newCount,
                size_type alignment,
                Args&&... args
                )
            noexcept(
                factory_traits<factory_t>::template noexcept_constructable<Args...> &&
                factory_traits<factory_t>::noexcept_moveable
            );

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(value_type* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
            destroy_and_deallocate_aligned(value_type* ptr, size_type count, size_type alignment) noexcept;

    private:
        RULE_OF_5_CONSTEXPR_NOEXCEPT(typed_allocator_impl)
        
        friend TypedAllocator;
        
        [[nodiscard]] [[rythe_always_inline]] constexpr TypedAllocator& self() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const TypedAllocator& self() const noexcept;
    };

    template <typename T, typed_factory_type Factory = default_factory<T>>
    class typed_allocator final : public typed_allocator_impl<typed_allocator<T, Factory>, T, Factory>
    {
        using base_type = typed_allocator_impl<typed_allocator, T, Factory>;

    public:
        using value_type = typename base_type::value_type;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;

        template <typename Other>
        using retarget = typed_allocator<Other, typename Factory::template retarget<Other>>;

        [[nodiscard]] [[rythe_always_inline]] bool operator==(const typed_allocator&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const typed_allocator&) const noexcept = default;

        [[rythe_always_inline]] explicit typed_allocator(allocator_storage baseAllocator)
                noexcept(is_nothrow_constructible_v<factory_storage_type>)
            : m_factory(),
              m_alloc(baseAllocator)
        {}

        [[rythe_always_inline]] explicit typed_allocator(allocator_storage baseAllocator,
                construct_type_signal_type<T>
                ) noexcept(is_nothrow_constructible_v<factory_storage_type>)
            : typed_allocator(baseAllocator)
        {}

        [[rythe_always_inline]] explicit typed_allocator(
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_copy_constructible_v<factory_storage_type>)
            : m_factory(factoryStorage), m_alloc(nullptr) {}

        [[rythe_always_inline]] typed_allocator(allocator_storage baseAllocator,
                const factory_storage_type& factoryStorage
                ) noexcept(is_nothrow_copy_constructible_v<
            factory_storage_type>)
            : m_factory(factoryStorage),
              m_alloc(baseAllocator)
        {}

        template <not_same_as<T> Other>
        [[rythe_always_inline]] typed_allocator(const retarget<Other>& other)
            : m_factory(other.get_factory_storage()),
              m_alloc(other.get_allocator_storage())
        {}

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage baseAllocator) noexcept
        {
            m_alloc = baseAllocator;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr memory_allocator& get_allocator() noexcept { return *m_alloc; }
        [[nodiscard]] [[rythe_always_inline]] constexpr const memory_allocator& get_allocator() const noexcept { return *m_alloc; }

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>) { m_factory = factoryStorage; }

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept { return *m_factory; }
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept { return *m_factory; }

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator_storage() noexcept { return m_alloc; }
        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator_storage() const noexcept
        {
            return m_alloc;
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept { return m_factory; }
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept
        {
            return m_factory;
        }

    private:
        factory_storage_type m_factory;
        allocator_storage m_alloc;
    };

    template <untyped_factory_type Factory = type_erased_factory>
    class type_erased_allocator
    {
    public:
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;

        template <typename Other>
        using retarget = type_erased_allocator<>;

         [[rythe_always_inline]] type_erased_allocator() noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator==(const type_erased_allocator&) const noexcept = default;
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const type_erased_allocator&) const noexcept = default;
        
        template <typename T>
        type_erased_allocator(allocator_storage baseAllocator, construct_type_signal_type<T>) noexcept(
                is_nothrow_constructible_v<factory_t, construct_type_signal_type<T>> &&
                is_nothrow_constructible_v<factory_storage_type, factory_t&&>);
        type_erased_allocator(const factory_storage_type& factoryStorage)
                noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);
        type_erased_allocator(allocator_storage baseAllocator, const factory_storage_type& factoryStorage)
                noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage baseAllocator) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr memory_allocator& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const memory_allocator& get_allocator() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator_storage() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] size_type type_size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] bool trivial_copy() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] id_type type_id() const noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        allocate(size_type count = 1) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        allocate(size_type count, size_type alignment) noexcept;
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        reallocate(
                void* ptr,
                size_type oldCount,
                size_type newCount
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        reallocate(
                void* ptr,
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void* construct(
                void* ptr,
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void* copy(
                void* dst,
                const void* src,
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::noexcept_copyable);
        [[rythe_always_inline]] constexpr void* move(
                void* dst,
                void* src,
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void destroy(void* ptr, size_type count = 1) noexcept;

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        allocate_and_construct(
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        allocate_aligned_and_construct(
                size_type count,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        reallocate_and_construct(
                void* ptr,
                size_type oldCount,
                size_type newCount
                ) noexcept(
            factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
        );

        [[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
        reallocate_aligned_and_construct(
                void* ptr,
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(
                factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable
            );

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(void* ptr, size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void
            destroy_and_deallocate_aligned(void* ptr, size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
        get_factory_storage() const noexcept;

    private:
        factory_storage_type m_factory;
        allocator_storage m_alloc;
    };

    namespace internal
    {
        template <typename T, factory_type Factory>
        struct select_typed_allocator;

        template <typename T, typed_factory_type Factory>
        struct select_typed_allocator<T, Factory>
        {
            using type = typed_allocator<T, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, untyped_factory_type Factory>
        struct select_typed_allocator<T, Factory>
        {
            using type = type_erased_allocator<Factory>;
            constexpr static bool is_untyped = true;
        };
    }
} // namespace rsl

#include "typed_allocator.inl"
