#pragma once

#include "allocator_storage.hpp"
#include "factory_storage.hpp"
#include "mock_allocator.hpp"
#include "typed_allocator.hpp"

namespace rsl
{
    template <typename T, size_type Capacity>
    struct static_capacity_storage
    {
        using value_type = T;
        T data[Capacity];
    };

    template <typename T>
    struct static_capacity_storage<T, 0ull>
    {
        using value_type = T;
    };

    template <size_type Capacity>
    struct static_capacity_storage<void, Capacity>
    {
        using value_type = byte;
        byte data[Capacity];
    };

    template <>
    struct static_capacity_storage<void, 0ull>
    {
        using value_type = byte;
    };

    namespace internal
    {
        struct alloc_and_factory_only_signal_type {};

        constexpr alloc_and_factory_only_signal_type alloc_and_factory_only_signal{};
    }

    template <allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    class dynamic_memory_resource_base
    {
    public:
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        using typed_alloc_type = internal::select_typed_allocator<UtilType, Alloc, Factory>::type;

        [[rythe_always_inline]] constexpr dynamic_memory_resource_base()
            noexcept(is_nothrow_constructible_v<typed_alloc_type>) = default;
        [[rythe_always_inline]] constexpr dynamic_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                const dynamic_memory_resource_base& other
                )
            noexcept(is_nothrow_copy_constructible_v<typed_alloc_type>);
        [[rythe_always_inline]] constexpr dynamic_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                dynamic_memory_resource_base&& other
                )
            noexcept(is_nothrow_move_constructible_v<typed_alloc_type>);

        dynamic_memory_resource_base(const dynamic_memory_resource_base&) = delete;
        dynamic_memory_resource_base(dynamic_memory_resource_base&&) = delete;

        dynamic_memory_resource_base& operator=(const dynamic_memory_resource_base&) = delete;
        dynamic_memory_resource_base& operator=(dynamic_memory_resource_base&&) = delete;

        virtual ~dynamic_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>);

        [[rythe_always_inline]] constexpr explicit dynamic_memory_resource_base(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr dynamic_memory_resource_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr void set_allocator(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type& get_allocator_storage() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept;

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment
                ) noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const void* src
                )
            noexcept(factory_traits<Factory>::noexcept_copyable);
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                void* src
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_aligned_and_construct(
                size_type count,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(
                size_type oldCount,
                size_type newCount
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable);

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void destroy_and_deallocate_aligned(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] static constexpr bool is_static_memory() noexcept { return false; }
        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_dynamic_memory() const noexcept { return m_ptr; }

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr_at(size_type offset) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr_at(size_type offset) const noexcept;
        [[rythe_always_inline]] constexpr void set_ptr(UtilType* const& ptr) noexcept;

        typed_alloc_type m_alloc;
        void* m_ptr = nullptr;
    };

    template <allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory, typename UtilType = void>
    class untyped_dynamic_memory_resource : public dynamic_memory_resource_base<Alloc, Factory, UtilType, true>
    {
        using base_type = dynamic_memory_resource_base<Alloc, Factory, UtilType, true>;

    public:
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;
        using typed_alloc_type = typename base_type::typed_alloc_type;

        using dynamic_memory_resource_base<Alloc, Factory, UtilType, true>::dynamic_memory_resource_base;
    };

    template <typename T, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
    class typed_dynamic_memory_resource : public dynamic_memory_resource_base<Alloc, Factory, T, false>
    {
        using base_type = dynamic_memory_resource_base<Alloc, Factory, T, false>;

    public:
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;
        using typed_alloc_type = typename base_type::typed_alloc_type;

        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;

        using dynamic_memory_resource_base<Alloc, Factory, T, false>::dynamic_memory_resource_base;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(
                size_type count = 1,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_aligned_and_construct(
                size_type count,
                size_type alignment,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args
                ) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment,
                Args&&... args
                ) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable);
    };

    template <size_type BufferSize, factory_type Factory, typename UtilType, bool Untyped>
    class static_memory_resource_base
    {
    public:
        using allocator_storage_type = allocator_storage<mock_allocator>;
        using allocator_t = mock_allocator;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        constexpr static size_type buffer_size = BufferSize;

        [[rythe_always_inline]] constexpr static_memory_resource_base()
            noexcept(is_nothrow_constructible_v<factory_storage_type>) = default;
        [[rythe_always_inline]] constexpr static_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                const static_memory_resource_base& other
                )
            noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);
        [[rythe_always_inline]] constexpr static_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                static_memory_resource_base&& other
                )
            noexcept(is_nothrow_move_constructible_v<factory_storage_type>);

        static_memory_resource_base(const static_memory_resource_base&) = delete;
        static_memory_resource_base(static_memory_resource_base&&) = delete;
        static_memory_resource_base& operator=(const static_memory_resource_base&) = delete;
        static_memory_resource_base& operator=(static_memory_resource_base&&) = delete;

        virtual ~static_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit static_memory_resource_base(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr explicit static_memory_resource_base(
                factory_storage_type&& factoryStorage
                )
            noexcept(is_nothrow_move_constructible_v<factory_storage_type>);

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept;

    protected:
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const void* src
                )
            noexcept(factory_traits<Factory>::noexcept_copyable);
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                void* src
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[nodiscard]] [[rythe_always_inline]] static constexpr bool is_static_memory() noexcept { return true; }
        [[nodiscard]] [[rythe_always_inline]] static constexpr bool is_dynamic_memory() noexcept { return false; }

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr_at(size_type offset) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr_at(size_type offset) const noexcept;
        [[rythe_always_inline]] static constexpr void set_ptr(UtilType* const&) noexcept {}

        factory_storage_type m_factory;
        using static_storage = static_capacity_storage<conditional_t<Untyped, void, UtilType>, BufferSize>;
        static_storage m_buffer{};
    };

    template <size_type BufferSize, untyped_factory_type Factory = type_erased_factory, typename UtilType = void>
    class untyped_static_memory_resource : public static_memory_resource_base<BufferSize, Factory, UtilType, true>
    {
        using base_type = static_memory_resource_base<BufferSize, Factory, UtilType, true>;

    public:
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;

        using static_memory_resource_base<BufferSize, Factory, UtilType, true>::static_memory_resource_base;
    };

    template <typename T, size_type BufferCount, factory_type Factory = default_factory<T>>
    class typed_static_memory_resource : public static_memory_resource_base<BufferCount * sizeof(T), Factory, T, false>
    {
        using base_type = static_memory_resource_base<BufferCount * sizeof(T), Factory, T, false>;

    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;
        constexpr static size_type buffer_count = BufferCount;

        using static_memory_resource_base<BufferCount * sizeof(T), Factory, T, false>::static_memory_resource_base;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);
    };


    template <size_type BufferSize, allocator_type Alloc, factory_type Factory, typename UtilType, bool Untyped>
    class hybrid_memory_resource_base
    {
    public:
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<Factory>;
        using factory_t = Factory;
        using typed_alloc_type = internal::select_typed_allocator<UtilType, Alloc, Factory>::type;
        constexpr static size_type buffer_size = BufferSize;

        [[rythe_always_inline]] constexpr hybrid_memory_resource_base()
            noexcept(is_nothrow_constructible_v<typed_alloc_type>) = default;
        [[rythe_always_inline]] constexpr hybrid_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                const hybrid_memory_resource_base& other
                )
            noexcept(is_nothrow_copy_constructible_v<factory_storage_type>);
        [[rythe_always_inline]] constexpr hybrid_memory_resource_base(
                internal::alloc_and_factory_only_signal_type,
                hybrid_memory_resource_base&& other
                )
            noexcept(is_nothrow_move_constructible_v<factory_storage_type>);

        hybrid_memory_resource_base(const hybrid_memory_resource_base& other) = delete;
        hybrid_memory_resource_base(hybrid_memory_resource_base&&) = delete;
        hybrid_memory_resource_base& operator=(const hybrid_memory_resource_base&) = delete;
        hybrid_memory_resource_base& operator=(hybrid_memory_resource_base&&) = delete;

        virtual ~hybrid_memory_resource_base() = default;

        [[rythe_always_inline]] constexpr explicit hybrid_memory_resource_base(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&>);

        [[rythe_always_inline]] constexpr explicit hybrid_memory_resource_base(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr hybrid_memory_resource_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<typed_alloc_type, const allocator_storage_type&, const factory_storage_type&>);

        [[rythe_always_inline]] constexpr void set_allocator(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type& get_allocator_storage() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type& get_factory_storage() const noexcept;

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(
                size_type oldCount,
                size_type newCount,
                size_type alignment
                ) noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);
        [[rythe_always_inline]] constexpr void copy(
                size_type count,
                size_type offset,
                const void* src
                )
            noexcept(factory_traits<Factory>::noexcept_copyable);
        [[rythe_always_inline]] constexpr void move(
                size_type count,
                size_type offset,
                void* src
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(
                size_type count = 1
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_aligned_and_construct(
                size_type count,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(
                size_type oldCount,
                size_type newCount
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<> && factory_traits<Factory>::noexcept_moveable);

        [[rythe_always_inline]] constexpr void destroy_and_deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void destroy_and_deallocate_aligned(size_type count, size_type alignment) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_static_memory() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool is_dynamic_memory() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr_at(size_type offset) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr_at(size_type offset) const noexcept;
        [[rythe_always_inline]] constexpr void set_ptr(UtilType* const& ptr) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_static_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_static_ptr() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_static_ptr_at(size_type offset) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_static_ptr_at(size_type offset) const noexcept;

        [[rythe_always_inline]] constexpr void move_to_static_memory_and_deallocate(
                size_type count,
                size_type memoryCount
                )
            noexcept(factory_traits<Factory>::noexcept_moveable);
        [[rythe_always_inline]] constexpr void set_ptr_to_static_memory() noexcept;

        static_capacity_storage<conditional_t<Untyped, void, UtilType>, BufferSize> m_buffer{};
        typed_alloc_type m_alloc;
        void* m_ptr = m_buffer.data;
    };

    template <
        size_type BufferSize,
        allocator_type Alloc = default_allocator,
        untyped_factory_type Factory = type_erased_factory,
        typename UtilType = void>
    class untyped_hybrid_memory_resource : public hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, true>
    {
        using base_type = hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, true>;

    public:
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;
        using typed_alloc_type = typename base_type::typed_alloc_type;

        using hybrid_memory_resource_base<BufferSize, Alloc, Factory, UtilType, true>::hybrid_memory_resource_base;
    };

    template <typename T, size_type BufferCount, allocator_type Alloc = default_allocator, factory_type Factory = default_factory<T>>
    class typed_hybrid_memory_resource : public hybrid_memory_resource_base<BufferCount * sizeof(T), Alloc, Factory, T, false>
    {
        using base_type = hybrid_memory_resource_base<BufferCount * sizeof(T), Alloc, Factory, T, false>;

    public:
        using allocator_storage_type = typename base_type::allocator_storage_type;
        using allocator_t = typename base_type::allocator_t;
        using factory_storage_type = typename base_type::factory_storage_type;
        using factory_t = typename base_type::factory_t;
        using typed_alloc_type = typename base_type::typed_alloc_type;

        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;

        constexpr static size_type buffer_count = BufferCount;

        using hybrid_memory_resource_base<BufferCount * sizeof(T), Alloc, Factory, T, false>::hybrid_memory_resource_base;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(
                size_type count = 1,
                size_type offset = 0,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(
                size_type count = 1,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_aligned_and_construct(
                size_type count,
                size_type alignment,
                Args&&... args
                )
            noexcept(factory_traits<Factory>::template noexcept_constructable<Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(
                size_type oldCount,
                size_type newCount,
                Args&&... args
                ) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_aligned_and_construct(
                size_type oldCount,
                size_type newCount,
                size_type alignment,
                Args&&... args
                ) noexcept(
            factory_traits<Factory>::template noexcept_constructable<Args...> && factory_traits<Factory>::noexcept_moveable);
    };

    namespace internal
    {
        template <
            typename T,
            allocator_type Alloc,
            factory_type Factory,
            size_type StaticStorageCount = 0ull,
            bool CanAllocate = !is_same_v<Alloc, mock_allocator>,
            bool Untyped = untyped_factory_type<Factory>>
        struct select_memory_resource;

        template <typename T, allocator_type Alloc, factory_type Factory>
        struct select_memory_resource<T, Alloc, Factory, 0ull, true, false>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = typed_dynamic_memory_resource<T, Alloc, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, allocator_type Alloc, factory_type Factory>
        struct select_memory_resource<T, Alloc, Factory, 0ull, true, true>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = untyped_dynamic_memory_resource<Alloc, Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, true, false>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = typed_hybrid_memory_resource<T, StaticStorageCount, Alloc, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, allocator_type Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, true, true>
        {
            static_assert(!is_same_v<Alloc, mock_allocator>);
            using type = untyped_hybrid_memory_resource<StaticStorageCount * sizeof(T), Alloc, Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, factory_type Factory, size_type StaticStorageCount, bool CanAllocate>
        struct select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, CanAllocate, false>
        {
            static_assert(CanAllocate == false, "Allocating using a mock_allocator is not possible.");
            using type = typed_static_memory_resource<T, StaticStorageCount, Factory>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, factory_type Factory, size_type StaticStorageCount, bool CanAllocate>
        struct select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, CanAllocate, true>
        {
            static_assert(CanAllocate == false, "Allocating using a mock_allocator is not possible.");
            using type = untyped_static_memory_resource<StaticStorageCount * sizeof(T), Factory, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, not_same_as<mock_allocator> Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, false, false>
                : public select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, false, false> {};

        template <typename T, not_same_as<mock_allocator> Alloc, factory_type Factory, size_type StaticStorageCount>
        struct select_memory_resource<T, Alloc, Factory, StaticStorageCount, false, true>
                : public select_memory_resource<T, mock_allocator, Factory, StaticStorageCount, false, true> {};

        template <typename>
        struct is_hybrid_resource : false_type {};

        template <
            size_type BufferSize,
            allocator_type Alloc,
            untyped_factory_type Factory,
            typename UtilType>
        struct is_hybrid_resource<untyped_hybrid_memory_resource<BufferSize, Alloc, Factory, UtilType>> : true_type {};

        template <typename T, size_type BufferCount, allocator_type Alloc, factory_type Factory>
        struct is_hybrid_resource<typed_hybrid_memory_resource<T, BufferCount, Alloc, Factory>> : true_type {};

        template <typename T>
        constexpr bool is_hybrid_resource_v = is_hybrid_resource<T>::value;

        template <typename>
        struct is_static_resource : false_type {};

        template <
            size_type BufferSize,
            untyped_factory_type Factory,
            typename UtilType>
        struct is_static_resource<untyped_static_memory_resource<BufferSize, Factory, UtilType>> : true_type {};

        template <typename T, size_type BufferCount, factory_type Factory>
        struct is_static_resource<typed_static_memory_resource<T, BufferCount, Factory>> : true_type {};

        template <typename T>
        constexpr bool is_static_resource_v = is_static_resource<T>::value;

        template <typename>
        struct is_dynamic_resource : false_type {};

        template <
            allocator_type Alloc,
            untyped_factory_type Factory,
            typename UtilType>
        struct is_dynamic_resource<untyped_dynamic_memory_resource<Alloc, Factory, UtilType>> : true_type {};

        template <typename T, allocator_type Alloc, factory_type Factory>
        struct is_dynamic_resource<typed_dynamic_memory_resource<T, Alloc, Factory>> : true_type {};

        template <typename T>
        constexpr bool is_dynamic_resource_v = is_dynamic_resource<T>::value;

        template <typename T>
        constexpr bool has_factory_v = true;

        template <typename T>
        constexpr bool has_allocator_v = !is_static_resource_v<T>;

        template <typename MemRsc>
        void move_alloc_and_factory(MemRsc& dst, MemRsc&& src)
        {
            if constexpr (internal::has_allocator_v<MemRsc>)
            {
                dst.set_allocator(rsl::move(src.get_allocator_storage()));
            }
            if constexpr (internal::has_factory_v<MemRsc>)
            {
                dst.set_factory(rsl::move(src.get_factory_storage()));
            }
        }

        template <typename MemRsc>
        void copy_alloc_and_factory(MemRsc& dst, const MemRsc& src)
        {
            if constexpr (internal::has_allocator_v<MemRsc>)
            {
                dst.set_allocator(src.get_allocator_storage());
            }
            if constexpr (internal::has_factory_v<MemRsc>)
            {
                dst.set_factory(src.get_factory_storage());
            }
        }
    } // namespace internal
}     // namespace rsl

#include "memory_resource_base.inl"
