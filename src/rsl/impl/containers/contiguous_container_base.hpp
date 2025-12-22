#pragma once
#include "../memory/memory_resource_base.hpp"
#include "../util/container_util.hpp"

#include "views.hpp"

namespace rsl
{
    template <bool UsePostFix, size_type StaticCapacity, bool CanAllocate, bool CanResize>
    struct contiguous_container_info
    {
        static_assert(!CanAllocate || (CanAllocate && CanResize), "Allocation without resizing is not possible");
        constexpr static bool use_post_fix = UsePostFix;
        constexpr static size_type static_capacity = StaticCapacity;
        constexpr static bool can_allocate = CanAllocate;
        constexpr static bool can_resize = CanResize;
    };

    template <typename T, typename ValueType>
    concept variadic_item_type = explicitly_convertible_to<T, ValueType> || (container_like<T> && explicitly_convertible_to<
        container_value_type<T>, ValueType>) || (is_array_v<T> && explicitly_convertible_to<array_value_t<T>, ValueType>);

    namespace internal
    {
        template <bool CanResize, bool UsePostFix, size_type StaticCapacity>
        constexpr size_type calculate_initial_size()
        {
            if constexpr (CanResize)
            {
                return 0ull;
            }
            else if constexpr (UsePostFix)
            {
                return StaticCapacity == 0ull ? 0ull : StaticCapacity - 1ull;
            }
            else
            {
                return StaticCapacity;
            }
        }
    }

    // TODO(Glyn): the allocators and factory template parameters are more trouble than they are worth.
    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    class contiguous_container_base
            : public internal::select_memory_resource<T, Alloc, Factory, ContiguousContainerInfo::static_capacity,
                                                      ContiguousContainerInfo::can_allocate>::type
    {
        static_assert(is_complete_v<T>);
    public:
        using mem_rsc = typename internal::select_memory_resource<
            T, Alloc, Factory, ContiguousContainerInfo::static_capacity, ContiguousContainerInfo::can_allocate>::type;
        using value_type = T;
        using iterator_type = Iter;
        using const_iterator_type = ConstIter;
        using reverse_iterator_type = reverse_iterator<iterator_type>;
        using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;
        using view_type = rsl::array_view<value_type, iterator_type, const_iterator_type>;
        using const_view_type = rsl::array_view<const value_type, const_iterator_type>;
        using allocator_storage_type = typename mem_rsc::allocator_storage_type;
        using allocator_t = typename mem_rsc::allocator_t;
        using factory_storage_type = typename mem_rsc::factory_storage_type;
        using factory_t = typename mem_rsc::factory_t;

        constexpr static bool use_post_fix = ContiguousContainerInfo::use_post_fix;
        constexpr static size_type static_capacity = ContiguousContainerInfo::static_capacity;
        constexpr static bool can_allocate = ContiguousContainerInfo::can_allocate;
        constexpr static bool can_resize = ContiguousContainerInfo::can_resize;

        constexpr static bool view_hash_identical = true;

    protected:
        constexpr static bool copy_assign_noexcept = is_nothrow_copy_assignable_v<value_type>;
        constexpr static bool copy_construct_noexcept = is_nothrow_copy_constructible_v<value_type>;
        constexpr static bool move_assign_noexcept = is_nothrow_move_assignable_v<value_type>;
        constexpr static bool move_construct_noexcept = is_nothrow_move_constructible_v<value_type>;

        constexpr static bool copy_construct_container_noexcept = is_nothrow_copy_constructible_v<mem_rsc>;
        constexpr static bool move_construct_container_noexcept = is_nothrow_move_constructible_v<mem_rsc>;

        template <typename... Types>
        constexpr static bool noexcept_construct_from_all = (is_nothrow_constructible_v<T, Types> && ...);

    public:
        [[rythe_always_inline]] constexpr contiguous_container_base() noexcept(is_nothrow_constructible_v<mem_rsc>);
        [[rythe_always_inline]] constexpr contiguous_container_base(
                const contiguous_container_base& src
                ) noexcept(copy_construct_container_noexcept);
        [[rythe_always_inline]] constexpr contiguous_container_base(
                contiguous_container_base&& src
                ) noexcept(move_construct_container_noexcept);
        constexpr virtual ~contiguous_container_base();

        [[rythe_always_inline]] explicit constexpr contiguous_container_base(
                const allocator_storage_type& allocStorage
                )
            noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
            requires(can_allocate);
        [[rythe_always_inline]] explicit constexpr contiguous_container_base(
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>);
        [[rythe_always_inline]] constexpr contiguous_container_base(
                const allocator_storage_type& allocStorage,
                const factory_storage_type& factoryStorage
                )
            noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
            requires(can_allocate);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_value(value_type& src) noexcept;
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_array(
                const value_type (& arr)[N]
                )
            noexcept(copy_construct_noexcept);
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_array(
                value_type (&& arr)[N]
                ) noexcept(move_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_buffer(
                const value_type* ptr,
                size_type count
                ) noexcept(copy_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base move_from_buffer(
                const value_type* ptr,
                size_type count
                ) noexcept(move_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_view(
                const_view_type src
                ) noexcept(copy_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_string_length(
                const T* str,
                T terminator = T{}
                ) noexcept
            requires char_type<T>;

        template <variadic_item_type<T>... ItemTypes>
        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base from_variadic_items(
                ItemTypes&&... items
                ) noexcept(noexcept_construct_from_all<ItemTypes...>);

        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base create_reserved(size_type capacity) noexcept
            requires(can_allocate);

        template <typename... Args>
        [[nodiscard]] [[rythe_always_inline]] constexpr static contiguous_container_base create_in_place(
                size_type count,
                Args&&... args
                ) noexcept(construct_noexcept<Args...>);

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

        [[rythe_always_inline]] constexpr contiguous_container_base& operator=(
                const contiguous_container_base& src
                )
            noexcept(copy_assign_noexcept && copy_construct_noexcept);
        [[rythe_always_inline]] constexpr contiguous_container_base& operator=(contiguous_container_base&& src) noexcept;

        template <typename... Args>
        [[rythe_always_inline]] constexpr void resize(
                size_type newSize,
                Args&&... args
                )
            noexcept(construct_noexcept<Args...> && move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr void reserve(size_type newCapacity) noexcept(move_construct_noexcept)
            requires(can_allocate);
        [[rythe_always_inline]] constexpr void reset() noexcept
            requires(can_resize);
        [[rythe_always_inline]] constexpr void shrink_to_fit() noexcept(move_construct_noexcept)
            requires(can_allocate);

        [[rythe_always_inline]] constexpr void push_back(const value_type& value) noexcept(copy_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr void push_back(value_type&& value) noexcept(move_construct_noexcept)
            requires(can_resize);

        template <typename... Args>
        [[rythe_always_inline]] constexpr value_type& emplace_back(
                Args&&... args
                )
            noexcept(construct_noexcept<Args...> && move_construct_noexcept)
            requires(can_resize);

        [[rythe_always_inline]] constexpr void pop_back() noexcept
            requires(can_resize);

        [[rythe_always_inline]] constexpr void reduce(size_type count = 1ull) noexcept
            requires(can_resize);

        [[rythe_always_inline]] constexpr void clear() noexcept
            requires(can_resize);

        [[rythe_always_inline]] constexpr void assign(size_type count, const value_type& value)
            requires(can_resize);
        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void assign(InputIt first, InputIt last)
            requires(can_resize);
        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void assign(const value_type* ptr, size_type count)
            requires(can_resize);
        template <size_type N>
        [[rythe_always_inline]] constexpr void assign(const value_type (& src)[N])
            requires(can_resize || (N == static_capacity));
        template <size_type N>
        [[rythe_always_inline]] constexpr void assign(value_type (&& src)[N])
            requires(can_resize || (N == static_capacity));

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type iterator_at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type iterator_at(size_type i) const noexcept;

        [[rythe_always_inline]] constexpr size_type append(
                array_view<const value_type> other
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type append(
                move_signal,
                array_view<value_type> other
                )
            noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type append(
                const value_type& value
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type append(value_type&& value) noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type append(
                size_type count,
                const value_type& value
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr size_type append(
                InputIt first,
                InputIt last
                )
            noexcept(move_construct_noexcept && construct_noexcept<iter_read_t<InputIt>>)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type append(
                const value_type* ptr,
                size_type count
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <size_type N>
        [[rythe_always_inline]] constexpr size_type append(
                const value_type (& src)[N]
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <size_type N>
        [[rythe_always_inline]] constexpr size_type append(value_type (&& src)[N]) noexcept(move_construct_noexcept)
            requires(can_resize);

        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                array_view<const value_type> other
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                move_signal,
                array_view<value_type> other
                )
            noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                const value_type& value
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type insert(size_type pos, value_type&& value) noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                size_type count,
                const value_type& value
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                InputIt first,
                InputIt last
                )
            noexcept(move_construct_noexcept && construct_noexcept<iter_read_t<InputIt>>)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                const value_type* ptr,
                size_type count
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <size_type N>
        [[rythe_always_inline]] constexpr size_type insert(
                size_type pos,
                const value_type (& src)[N]
                )
            noexcept(move_construct_noexcept && copy_construct_noexcept)
            requires(can_resize);
        template <size_type N>
        [[rythe_always_inline]] constexpr size_type insert(size_type pos, value_type (&& src)[N]) noexcept(move_construct_noexcept)
            requires(can_resize);

        // If it's possible to do a bulk erasure, then erase_shift in bulk might be faster. Try both and test!
        [[rythe_always_inline]] constexpr size_type erase_swap(size_type pos) noexcept(move_construct_noexcept)
            requires(can_resize);

        // Unless specifically required, use erase_shift for bulk erasures.
        // Effectively the same as erase_shift, but reverses the order of shifted elements.
        [[rythe_always_inline]] constexpr size_type erase_swap(const_view_type view) noexcept(move_construct_noexcept)
            requires(can_resize);
        // Unless specifically required, use erase_shift for bulk erasures.
        // Effectively the same as erase_shift, but reverses the order of shifted elements.
        [[rythe_always_inline]] constexpr size_type erase_swap(size_type pos, size_type count) noexcept(move_construct_noexcept)
            requires(can_resize);

        // Returns amount of items removed, specific location of erasure is not possible to reconstruct.
        template <typename Comp>
        [[rythe_always_inline]] constexpr size_type erase_swap(const Comp& comparable) noexcept(move_construct_noexcept)
            requires equality_comparable_with<Comp, T> && can_resize;

        // Returns amount of items removed, specific location of erasure is not possible to reconstruct.
        template <typename Func>
        [[rythe_always_inline]] constexpr size_type erase_swap(Func&& comparer) noexcept(move_construct_noexcept)
            requires invocable<Func, bool(ConstIter)> && can_resize;

        // Unless specifically required, use erase_swap for single erasures.
        // If it's possible to do a bulk erasure, then erase_shift in bulk might be faster. Try both and test!
        [[rythe_always_inline]] constexpr size_type erase_shift(size_type pos) noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type erase_shift(const_view_type view) noexcept(move_construct_noexcept)
            requires(can_resize);
        [[rythe_always_inline]] constexpr size_type erase_shift(size_type pos, size_type count) noexcept(move_construct_noexcept)
            requires(can_resize);

        // Unless specifically required use erase_swap for selective erasures.
        // Depending on your use case erase_swap might be faster. Try both and test!
        // Returns amount of items removed, specific location of erasure is not possible to reconstruct.
        template <typename Comp>
        [[rythe_always_inline]] constexpr size_type erase_shift(const Comp& comparable) noexcept(move_construct_noexcept)
            requires equality_comparable_with<Comp, T> && can_resize;

        // Unless specifically required use erase_swap for selective erasures.
        // Depending on your use case erase_swap might be faster. Try both and test!
        // Returns amount of items removed, specific location of erasure is not possible to reconstruct.
        template <typename Func>
        [[rythe_always_inline]] constexpr size_type erase_shift(Func&& comparer) noexcept(move_construct_noexcept)
            requires invocable<Func, bool(ConstIter)> && can_resize;

        [[rythe_always_inline]] constexpr size_type replace(
                size_type pos,
                size_type count,
                const_view_type replacement
                ) noexcept(move_construct_noexcept && copy_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& at(size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator[](size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type* data() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type* data() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr view_type view() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_view_type view() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator view_type() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator const_view_type() const noexcept;

        // negative count will assume size() - abs(count)
        [[nodiscard]] [[rythe_always_inline]] constexpr view_type subview(size_type offset, diff_type count = npos) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_view_type subview(
                size_type offset,
                diff_type count = npos
                ) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& front() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& front() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& back() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& back() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

    protected:
        template <typename... Args>
        constexpr static bool construct_noexcept = is_nothrow_constructible_v<value_type, Args...>;

        [[rythe_always_inline]] constexpr bool maybe_shrink_to_static_storage() noexcept(move_construct_noexcept)
            requires(can_allocate);

        [[nodiscard]] [[rythe_always_inline]] constexpr bool maybe_grow() noexcept(move_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr bool resize_capacity_unsafe(
                size_type newCapacity
                ) noexcept(move_construct_noexcept)
            requires(can_allocate);

        [[rythe_always_inline]] constexpr void copy_assign_impl(
                const value_type* src,
                size_type srcSize,
                const void* allocOrFactory = nullptr
                )
            noexcept(copy_assign_noexcept && copy_construct_noexcept);

        [[rythe_always_inline]] constexpr void move_data_assign_impl(
                const value_type* src,
                size_type srcSize
                )
            noexcept(move_assign_noexcept && move_construct_noexcept);

        [[rythe_always_inline]] constexpr void split_reserve(
                size_type pos,
                diff_type offset
                )
            noexcept(move_construct_noexcept)
            requires(can_resize);

        [[rythe_always_inline]] constexpr void erase_swap_unsafe_impl(size_type pos) noexcept(move_construct_noexcept)
            requires(can_resize);

        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void copy_assign_from_unsafe_impl(
                size_type offset,
                size_type end,
                InputIt srcIter
                )
            noexcept(copy_assign_noexcept);

        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void copy_construct_from_unsafe_impl(
                size_type offset,
                size_type end,
                InputIt srcIter
                )
            noexcept(copy_construct_noexcept);

        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void move_assign_from_unsafe_impl(
                size_type offset,
                size_type end,
                InputIt srcIter
                )
            noexcept(move_assign_noexcept);

        template <input_iterator InputIt>
        [[rythe_always_inline]] constexpr void move_construct_from_unsafe_impl(
                size_type offset,
                size_type end,
                InputIt srcIter
                )
            noexcept(move_construct_noexcept);

        template <typename Type, typename... Types>
        void container_construct_items(const size_type offset, Type&& arg, Types&&... args) noexcept(noexcept_construct_from_all<Type, Types...>);

        template <typename... Args>
        [[rythe_always_inline]] constexpr void emplace_unsafe_impl(
                size_type offset,
                size_type end,
                Args&&... args
                )
            noexcept(construct_noexcept<Args...>);

        [[rythe_always_inline]] constexpr void reset_unsafe_impl(size_type offset = 0, size_type end = npos) noexcept;

        [[rythe_always_inline]] constexpr void move_shift_elements_unsafe(
                size_type offset,
                size_type end,
                diff_type shift
                )
            noexcept(move_construct_noexcept);

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type* get_ptr_at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type* get_ptr_at(size_type i) const noexcept;

        [[rythe_always_inline]] constexpr void shrink_to_postfix() noexcept;
        [[rythe_always_inline]] constexpr void construct_postfix() noexcept(construct_noexcept<>);
        [[rythe_always_inline]] constexpr void destroy_postfix() noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type calc_max_size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr static size_type calc_memory_size(size_type itemCount) noexcept;

        // TODO: m_size is not needed if `can_resize` is false
        //       m_capacity is not needed if `can_allocate` is false
        //       make a special case for array without resizing or allocations
        size_type m_size = internal::calculate_initial_size<can_resize, use_post_fix, static_capacity>();
        size_type m_memorySize = static_capacity;
    };

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    [[rythe_always_inline]] constexpr bool operator==(
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& lhs,
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& rhs
            ) noexcept;

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    [[rythe_always_inline]] constexpr bool operator!=(
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& lhs,
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& rhs
            ) noexcept { return !(lhs == rhs); }
} // namespace rsl

#include "contiguous_container_base.inl"
