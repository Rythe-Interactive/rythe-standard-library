#pragma once

#include "allocator_context.hpp"
#include "typed_allocator.hpp"

namespace rsl
{
    template <size_type Capacity, size_type Alignment>
    struct alignas(Alignment) static_capacity_storage
    {
        byte data[Capacity];

        template<typename T = byte>
        T* get_data_ptr() noexcept
        {
            return bit_cast<T*>(&data[0]);
        }

        template <typename T = byte>
        const T* get_data_ptr() const noexcept
        {
            return bit_cast<const T*>(&data[0]);
        }
    };

    template <size_type Alignment>
    struct static_capacity_storage<0ull, Alignment> {};

    namespace internal
    {
        struct alloc_and_factory_only_signal_type {};

        constexpr alloc_and_factory_only_signal_type alloc_and_factory_only_signal{};
    } // namespace internal

    template <typename UtilType, bool Untyped>
    class dynamic_memory_resource
    {
    public:
        using typed_alloc_type = internal::select_typed_allocator<UtilType, Untyped>::type;

        [[rythe_always_inline]] constexpr dynamic_memory_resource() noexcept
            : dynamic_memory_resource(allocator_context::globalAllocator) {}

        [[rythe_always_inline]] constexpr dynamic_memory_resource(
                internal::alloc_and_factory_only_signal_type, const dynamic_memory_resource& other)
                noexcept;

        dynamic_memory_resource(const dynamic_memory_resource&) = delete;
        dynamic_memory_resource(dynamic_memory_resource&&) = delete;

        dynamic_memory_resource& operator=(const dynamic_memory_resource&) = delete;
        dynamic_memory_resource& operator=(dynamic_memory_resource&&) = delete;

        virtual ~dynamic_memory_resource() = default;

        [[rythe_always_inline]] constexpr explicit dynamic_memory_resource(allocator_storage allocator) noexcept;

        [[rythe_always_inline]] constexpr explicit dynamic_memory_resource(const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[rythe_always_inline]] constexpr dynamic_memory_resource(
                allocator_storage allocator, const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage allocator) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[nodiscard]] [[rythe_always_inline]] constexpr type_erased_factory& get_factory() noexcept
            requires(Untyped);
        [[nodiscard]] [[rythe_always_inline]] constexpr const type_erased_factory& get_factory() const noexcept
            requires(Untyped);

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(size_type oldCount, size_type newCount)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate(size_type oldCount, size_type newCount, size_type alignment)
                        noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0)
                noexcept(is_nothrow_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void copy(size_type count, size_type offset, const void* src)
                noexcept(is_nothrow_copy_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void move(size_type count, size_type offset, void* src)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(size_type count = 1)
                noexcept(is_nothrow_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                allocate_aligned_and_construct(size_type count, size_type alignment) noexcept(is_nothrow_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(size_type oldCount, size_type newCount)
                noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_aligned_and_construct(size_type oldCount, size_type newCount, size_type alignment)
                        noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>);

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

    template <typename UtilType = void>
    using untyped_dynamic_memory_resource = dynamic_memory_resource<UtilType, true>;

    template <typename T>
    class typed_dynamic_memory_resource : public dynamic_memory_resource<T, false>
    {
        using base_type = dynamic_memory_resource<T, false>;

    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;

        using dynamic_memory_resource<T, false>::dynamic_memory_resource;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(size_type count = 1, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                allocate_aligned_and_construct(size_type count, size_type alignment, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_and_construct(size_type oldCount, size_type newCount, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_aligned_and_construct(size_type oldCount, size_type newCount, size_type alignment, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);
    };

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment = alignof(UtilType)>
    class static_memory_resource
    {
    public:
        using factory_type = conditional_t<Untyped, type_erased_factory, factory<UtilType>>;

        constexpr static size_type buffer_size = BufferSize;

        [[rythe_always_inline]] constexpr static_memory_resource() noexcept = default;
        [[rythe_always_inline]] constexpr static_memory_resource(
                internal::alloc_and_factory_only_signal_type, const static_memory_resource& other) noexcept;

        static_memory_resource(const static_memory_resource&) = delete;
        static_memory_resource(static_memory_resource&&) = delete;
        static_memory_resource& operator=(const static_memory_resource&) = delete;
        static_memory_resource& operator=(static_memory_resource&&) = delete;

        virtual ~static_memory_resource() = default;

        [[rythe_always_inline]] constexpr explicit static_memory_resource(const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[rythe_always_inline]] constexpr void set_factory(const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[nodiscard]] [[rythe_always_inline]] constexpr factory_type& get_factory() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const factory_type& get_factory() const noexcept;

    protected:
        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0)
                noexcept(is_nothrow_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void copy(size_type count, size_type offset, const void* src)
                noexcept(is_nothrow_copy_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void move(size_type count, size_type offset, void* src)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[nodiscard]] [[rythe_always_inline]] static constexpr bool is_static_memory() noexcept { return true; }
        [[nodiscard]] [[rythe_always_inline]] static constexpr bool is_dynamic_memory() noexcept { return false; }

        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr UtilType* get_ptr_at(size_type offset) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const UtilType* get_ptr_at(size_type offset) const noexcept;
        [[rythe_always_inline]] static constexpr void set_ptr(UtilType* const&) noexcept {}

        [[rythe_no_unique_address]] factory_type m_factory;
        [[rythe_no_unique_address]] static_capacity_storage<BufferSize * sizeof(UtilType), Alignment> m_buffer{};
    };

    template <size_type BufferSize, typename UtilType = void, size_type Alignment = alignof(UtilType)>
    using untyped_static_memory_resource = static_memory_resource<BufferSize, UtilType, true, Alignment>;

    template <typename T, size_type BufferCount>
    class typed_static_memory_resource : public static_memory_resource<BufferCount, T, false>
    {
        using base_type = static_memory_resource<BufferCount, T, false>;

    public:
        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;
        constexpr static size_type buffer_count = BufferCount;

        using static_memory_resource<BufferCount, T, false>::static_memory_resource;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);
    };

    template <size_type BufferSize, typename UtilType, bool Untyped, size_type Alignment = alignof(UtilType)>
    class hybrid_memory_resource
    {
    public:
        using typed_alloc_type = internal::select_typed_allocator<UtilType, Untyped>::type;
        constexpr static size_type buffer_size = BufferSize;

        [[rythe_always_inline]] constexpr hybrid_memory_resource() noexcept
            : hybrid_memory_resource(allocator_context::globalAllocator)
        {}

        [[rythe_always_inline]] constexpr hybrid_memory_resource(
                internal::alloc_and_factory_only_signal_type, const hybrid_memory_resource& other) noexcept;
        [[rythe_always_inline]] constexpr hybrid_memory_resource(
                internal::alloc_and_factory_only_signal_type, hybrid_memory_resource&& other) noexcept;

        hybrid_memory_resource(const hybrid_memory_resource& other) = delete;
        hybrid_memory_resource(hybrid_memory_resource&&) = delete;
        hybrid_memory_resource& operator=(const hybrid_memory_resource&) = delete;
        hybrid_memory_resource& operator=(hybrid_memory_resource&&) = delete;

        virtual ~hybrid_memory_resource() = default;

        [[rythe_always_inline]] constexpr explicit hybrid_memory_resource(allocator_storage allocator) noexcept;

        [[rythe_always_inline]] constexpr explicit hybrid_memory_resource(const type_erased_factory& factoryStorage) noexcept
            requires(Untyped);

        [[rythe_always_inline]] constexpr hybrid_memory_resource(
                allocator_storage allocator, const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[rythe_always_inline]] constexpr void set_allocator(allocator_storage allocator) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage get_allocator() const noexcept;

        [[rythe_always_inline]] constexpr void set_factory(const type_erased_factory& factory) noexcept
            requires(Untyped);

        [[nodiscard]] [[rythe_always_inline]] constexpr type_erased_factory& get_factory() noexcept
            requires(Untyped);
        [[nodiscard]] [[rythe_always_inline]] constexpr const type_erased_factory& get_factory() const noexcept
            requires(Untyped);

    protected:
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count = 1) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate(size_type count, size_type alignment) noexcept;
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate(size_type oldCount, size_type newCount)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate(size_type oldCount, size_type newCount, size_type alignment)
                        noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void deallocate(size_type count = 1) noexcept;
        [[rythe_always_inline]] constexpr void deallocate(size_type count, size_type alignment) noexcept;

        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0)
                noexcept(is_nothrow_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void copy(size_type count, size_type offset, const void* src)
                noexcept(is_nothrow_copy_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void move(size_type count, size_type offset, void* src)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void destroy(size_type count = 1, size_type offset = 0) noexcept;

        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(size_type count = 1)
                noexcept(is_nothrow_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                allocate_aligned_and_construct(size_type count, size_type alignment) noexcept(is_nothrow_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void reallocate_and_construct(size_type oldCount, size_type newCount)
                noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_aligned_and_construct(size_type oldCount, size_type newCount, size_type alignment)
                        noexcept(is_nothrow_constructible_v<UtilType> && is_nothrow_move_constructible_v<UtilType>);

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

        [[rythe_always_inline]] constexpr void move_to_static_memory_and_deallocate(size_type count, size_type memoryCount)
                noexcept(is_nothrow_move_constructible_v<UtilType>);
        [[rythe_always_inline]] constexpr void set_ptr_to_static_memory() noexcept;

        static_capacity_storage<BufferSize * sizeof(UtilType), Alignment> m_buffer{};
        typed_alloc_type m_alloc;
        void* m_ptr = m_buffer.template get_data_ptr<void>();
    };

    template <size_type BufferSize, typename UtilType = void, size_type Alignment = alignof(UtilType)>
    using untyped_hybrid_memory_resource = hybrid_memory_resource<BufferSize, UtilType, true, Alignment>;

    template <typename T, size_type BufferCount>
    class typed_hybrid_memory_resource : public hybrid_memory_resource<BufferCount, T, false>
    {
        using base_type = hybrid_memory_resource<BufferCount, T, false>;

    public:
        using typed_alloc_type = typename base_type::typed_alloc_type;

        using value_type = T;
        using ref_type = add_lval_ref_t<T>;
        using const_ref_type = decorate_type_t<T, const_signal, lval_ref_signal>;
        using ptr_type = add_pointer_t<T>;
        using const_ptr_type = decorate_type_t<T, const_signal, pointer_signal>;

        constexpr static size_type buffer_count = BufferCount;

        using hybrid_memory_resource<BufferCount, T, false>::hybrid_memory_resource;

    protected:
        template <typename... Args>
        [[rythe_always_inline]] constexpr void construct(size_type count = 1, size_type offset = 0, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void allocate_and_construct(size_type count = 1, Args&&... args)
                noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                allocate_aligned_and_construct(size_type count, size_type alignment, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_and_construct(size_type oldCount, size_type newCount, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);

        template <typename... Args>
        [[rythe_allocating]] [[rythe_always_inline]] constexpr void
                reallocate_aligned_and_construct(size_type oldCount, size_type newCount, size_type alignment, Args&&... args)
                        noexcept(is_nothrow_constructible_v<T, Args...> && is_nothrow_move_constructible_v<T>);
    };

    namespace internal
    {
        template <typename T, size_type StaticStorageCount = 0ull, bool CanAllocate = true, bool Untyped = false, size_type Alignment = alignof(T)>
        struct select_memory_resource;

        template <typename T>
        struct select_memory_resource<T, 0ull, true, false>
        {
            using type = typed_dynamic_memory_resource<T>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, size_type Alignment>
        struct select_memory_resource<T, 0ull, true, true, Alignment>
        {
            using type = untyped_dynamic_memory_resource<T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, size_type StaticStorageCount>
        struct select_memory_resource<T, StaticStorageCount, true, false>
        {
            using type = typed_hybrid_memory_resource<T, StaticStorageCount>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, size_type StaticStorageCount, size_type Alignment>
        struct select_memory_resource<T, StaticStorageCount, true, true, Alignment>
        {
            using type = untyped_hybrid_memory_resource<StaticStorageCount, T>;
            constexpr static bool is_untyped = true;
        };

        template <typename T, size_type StaticStorageCount>
        struct select_memory_resource<T, StaticStorageCount, false, false>
        {
            using type = typed_static_memory_resource<T, StaticStorageCount>;
            constexpr static bool is_untyped = false;
        };

        template <typename T, size_type StaticStorageCount, size_type Alignment>
        struct select_memory_resource<T, StaticStorageCount, false, true, Alignment>
        {
            using type = untyped_static_memory_resource<StaticStorageCount, T, Alignment>;
            constexpr static bool is_untyped = true;
        };

        template <typename>
        struct is_hybrid_resource : false_type
        {};

        template <size_type BufferSize, typename UtilType>
        struct is_hybrid_resource<untyped_hybrid_memory_resource<BufferSize, UtilType>> : true_type
        {};

        template <typename T, size_type BufferCount>
        struct is_hybrid_resource<typed_hybrid_memory_resource<T, BufferCount>> : true_type
        {};

        template <typename T>
        constexpr bool is_hybrid_resource_v = is_hybrid_resource<T>::value;

        template <typename>
        struct is_static_resource : false_type
        {};

        template <size_type BufferSize, typename UtilType>
        struct is_static_resource<untyped_static_memory_resource<BufferSize, UtilType>> : true_type
        {};

        template <typename T, size_type BufferCount>
        struct is_static_resource<typed_static_memory_resource<T, BufferCount>> : true_type
        {};

        template <typename T>
        constexpr bool is_static_resource_v = is_static_resource<T>::value;

        template <typename>
        struct is_dynamic_resource : false_type
        {};

        template <typename UtilType>
        struct is_dynamic_resource<untyped_dynamic_memory_resource<UtilType>> : true_type
        {};

        template <typename T>
        struct is_dynamic_resource<typed_dynamic_memory_resource<T>> : true_type
        {};

        template <typename T>
        constexpr bool is_dynamic_resource_v = is_dynamic_resource<T>::value;

        template <typename T>
        constexpr bool has_allocator_v = !is_static_resource_v<T>;
    } // namespace internal
} // namespace rsl

#include "memory_resource_base.inl"
