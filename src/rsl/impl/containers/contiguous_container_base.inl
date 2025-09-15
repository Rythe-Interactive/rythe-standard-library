#pragma once

namespace rsl
{
    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    contiguous_container_base() noexcept(is_nothrow_constructible_v<mem_rsc>)
    {
        if constexpr (!can_resize)
        {
            mem_rsc::construct(m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            const contiguous_container_base& src
            ) noexcept(copy_construct_container_noexcept)
        : mem_rsc(internal::alloc_and_factory_only_signal, src)
    {
        reserve(src.m_size);
        m_size = src.m_size;
        // will copy postfix from src if `use_post_fix`
        copy_construct_from_unsafe_impl(0ull, calc_memory_size(m_size), src.get_ptr());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            contiguous_container_base&& src
            ) noexcept(move_construct_container_noexcept)
        : mem_rsc(internal::alloc_and_factory_only_signal, rsl::move(src))
    {
        if (src.is_static_memory())
        {
            mem_rsc::move(calc_memory_size(src.m_size), 0ull, src.get_ptr());
            return;
        }

        mem_rsc::set_ptr(src.get_ptr());
        m_size = src.m_size;
        m_memorySize = src.m_memorySize;

        src.m_size = internal::calculate_initial_size<can_resize, use_post_fix, static_capacity>();
        src.m_memorySize = static_capacity;
        src.set_ptr(nullptr);
        src.construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::~contiguous_container_base()
    {
        if constexpr (!can_resize)
        {
            mem_rsc::destroy(m_size);
        }
        else
        {
            if (!empty()) [[unlikely]]
            {
                reset();
            }
        }

        if constexpr (can_allocate)
        {
            shrink_to_fit();
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            const allocator_storage_type& allocStorage
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
        requires(can_allocate)
        : mem_rsc(allocStorage)
    {
        if constexpr (!can_resize)
        {
            mem_rsc::construct(m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>)
        : mem_rsc(factoryStorage)
    {
        if constexpr (!can_resize)
        {
            mem_rsc::construct(m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
        requires(can_allocate)
        : mem_rsc(allocStorage, factoryStorage)
    {
        if constexpr (!can_resize)
        {
            mem_rsc::construct(m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_value(value_type& src) noexcept
    {
        return from_buffer(&src, 1ull);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <size_type N>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_array(const value_type (& arr)[N]) noexcept(copy_construct_noexcept)
    {
        return from_buffer(arr, N);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <size_type N>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_array(value_type (&& arr)[N]) noexcept(move_construct_noexcept)
    {
        return move_from_buffer(arr, N);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_buffer(
            const value_type* ptr,
            const size_type count
            ) noexcept(copy_construct_noexcept)
    {
        contiguous_container_base result;

        if constexpr (can_allocate)
        {
            result.reserve(count);
        }
        else
        {
            rsl_assert_invalid_operation(contiguous_container_base::calc_memory_size(count) <= static_capacity);
        }

        if constexpr (can_resize)
        {
            result.m_size = count;
        }

        result.copy_construct_from_unsafe_impl(0ull, result.m_size, ptr);

        if constexpr (char_type<value_type>)
        {
            if (result.m_size > 0)
            {
                if (*result.get_ptr_at(result.m_size - 1) == static_cast<value_type>('\0'))
                {
                    --result.m_size;
                }
            }
        }

        result.construct_postfix();
        return result;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::move_from_buffer(
            const value_type* ptr,
            size_type count
            ) noexcept(move_construct_noexcept)
    {
        contiguous_container_base result;

        if constexpr (can_allocate)
        {
            result.reserve(count);
        }
        else
        {
            rsl_assert_invalid_operation(contiguous_container_base::calc_memory_size(count) <= static_capacity);
        }

        if constexpr (can_resize)
        {
            result.m_size = count;
        }

        result.move_construct_from_unsafe_impl(0ull, result.m_size, ptr);
        result.construct_postfix();
        return result;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_view(const_view_type src) noexcept(copy_construct_noexcept)
    {
        return from_buffer(src.data(), src.size());
    }

    namespace internal
    {
        template <typename T, variadic_item_type<T> ItemType>
        constexpr size_type variadic_item_size(ItemType&& item) noexcept
        {
            if constexpr (container_like<ItemType> || is_array_v<ItemType>)
            {
                return size(item);
            }
            else
            {
                return 1ull;
            }
        }

        template <typename Container, typename Type, typename... Types>
        void container_construct_items(Container& container, const size_type offset, Type&& arg, Types&&... args)
        {
            if constexpr (container_like<Type> || is_array_v<Type>)
            {
                size_type i = 0;
                for (auto& item : arg)
                {
                    if constexpr (is_rvalue_reference_v<Type>)
                    {
                        container.construct(1ull, offset + i, rsl::move(item));
                    }
                    else
                    {
                        container.construct(1ull, offset + i, item);
                    }
                    ++i;
                }
            }
            else
            {
                container.construct(1ull, offset, rsl::forward<Type>(arg));
            }

            if constexpr (sizeof...(Types) != 0)
            {
                container_construct_items(
                        container,
                        offset + variadic_item_size<typename Container::value_type>(arg),
                        rsl::forward<Types>(args)...
                        );
            }
        }

        template <typename T, variadic_item_type<T> ... ItemTypes>
        constexpr size_type calc_total_variadic_item_size(ItemTypes&&... items) noexcept
        {
            return (variadic_item_size<T>(rsl::forward<ItemTypes>(items)) + ...);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <variadic_item_type<T> ... ItemTypes>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_variadic_items(
            ItemTypes&&... items
            ) noexcept(noexcept_construct_from_all<ItemTypes...>)
    {
        contiguous_container_base result;
        if constexpr (can_allocate)
        {
            result.reserve(internal::calc_total_variadic_item_size<T>(rsl::forward<ItemTypes>(items)...));
        }
        else
        {
            rsl_assert_invalid_operation(
                    contiguous_container_base::calc_memory_size(internal::calc_total_variadic_item_size<T>(rsl::forward<ItemTypes>(
                        items)...)) <= static_capacity
                    );
        }

        if constexpr (can_resize)
        {
            result.m_size = internal::calc_total_variadic_item_size<T>(rsl::forward<ItemTypes>(items)...);
        }

        internal::container_construct_items(result, 0ull, rsl::forward<ItemTypes>(items)...);
        result.construct_postfix();
        return result;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_string_length(T* str, T terminator) noexcept
        requires char_type<T>
    {
        return from_buffer(str, string_length(str, terminator));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::create_reserved(const size_type capacity) noexcept
        requires (can_allocate)
    {
        contiguous_container_base result;
        result.reserve(capacity);

        return result;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <typename... Args>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::create_in_place(
            const size_type count,
            Args&&... args
            ) noexcept(construct_noexcept<Args...>)
    {
        contiguous_container_base result;
        result.reserve(count);
        result.m_size = count;
        result.construct(count, 0ull, rsl::forward<Args>(args)...);
        result.construct_postfix();

        return result;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::size() const noexcept
    {
        return m_size;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::empty() const noexcept
    {
        return m_size == 0;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                  ContiguousContainerInfo>::capacity() const noexcept
    {
        return m_memorySize;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& contiguous_container_base<T,
        Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator=(
            const contiguous_container_base& src
            ) noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        if constexpr (internal::is_static_resource_v<mem_rsc>)
        {
            copy_assign_impl(src.get_ptr(), src.m_size, mem_rsc::m_factory == src.m_factory ? nullptr : &src.m_factory);
        }
        else
        {
            copy_assign_impl(src.get_ptr(), src.m_size, mem_rsc::m_alloc == src.m_alloc ? nullptr : &src.m_alloc);
        }
        return *this;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& contiguous_container_base<T,
        Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator=(contiguous_container_base&& src) noexcept
    {
        internal::move_alloc_and_factory<mem_rsc>(*this, rsl::move(src));
        mem_rsc::set_ptr(src.get_ptr());
        m_size = src.m_size;
        m_memorySize = src.m_memorySize;

        src.m_size = internal::calculate_initial_size<can_resize, use_post_fix, static_capacity>();
        src.m_memorySize = static_capacity;
        src.set_ptr(nullptr);
        src.construct_postfix();
        return *this;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename... Args>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::resize(
            size_type newSize,
            Args&&... args
            )
        noexcept(construct_noexcept<Args...> && move_construct_noexcept)
        requires (can_resize)
    {
        destroy_postfix();

        if constexpr (can_allocate)
        {
            reserve(newSize);
        }
        else
        {
            rsl_assert_invalid_operation(newSize <= m_memorySize);
        }

        if (newSize > m_size)
        {
            emplace_unsafe_impl(m_size, newSize, forward<Args>(args)...);
        }
        else if (newSize < m_size)
        {
            reset_unsafe_impl(newSize);
        }

        m_size = newSize;

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reserve(
            size_type newCapacity
            )
        noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        newCapacity = calc_memory_size(newCapacity);

        if (newCapacity < m_memorySize) [[unlikely]]
        {
            return;
        }

        rsl_ensure(resize_capacity_unsafe(newCapacity));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reset() noexcept
        requires (can_resize)
    {
        destroy_postfix();
        reset_unsafe_impl();
        m_size = 0ull;
        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::shrink_to_fit() noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        if (m_size == 0ull)
        {
            if constexpr (!internal::is_static_resource_v<mem_rsc>)
            {
                mem_rsc::deallocate(m_memorySize);
            }
            return;
        }

        if (maybe_shrink_to_static_storage())
        {
            return;
        }

        rsl_ensure(resize_capacity_unsafe(calc_memory_size(m_size)));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::push_back(
            const value_type& value
            )
        noexcept(copy_construct_noexcept)
        requires (can_resize)
    {
        destroy_postfix();

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, value);
        ++m_size;

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::push_back(
            value_type&& value
            )
        noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        destroy_postfix();

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, move(value));
        ++m_size;

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename... Args>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::emplace_back(
            Args&&... args
            )
        noexcept(construct_noexcept<Args...> && move_construct_noexcept)
        requires (can_resize)
    {
        destroy_postfix();

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, rsl::forward<Args>(args)...);
        ++m_size;

        construct_postfix();

        return back();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::pop_back()
        noexcept
        requires (can_resize)
    {
        reduce(1);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reduce(
            size_type count
            )
        noexcept
        requires (can_resize)
    {
        if (count > m_size) [[unlikely]]
        {
            count = m_size;
        }

        destroy_postfix();

        reset_unsafe_impl(m_size - count);
        m_size -= count;

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::clear()
        noexcept
        requires (can_resize)
    {
        destroy_postfix();
        reset_unsafe_impl();
        m_size = 0;
        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::assign(
            const size_type count,
            const value_type& value
            )
        requires (can_resize)
    {
        clear();
        insert(0, count, value);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    assign(InputIt first, InputIt last)
        requires (can_resize)
    {
        clear();
        insert(0, first, last);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::assign(
            const value_type* ptr,
            const size_type count
            )
        requires (can_resize)
    {
        clear();
        insert(0, ptr, count);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    assign(const value_type (& src)[N])
        requires (can_resize || (N == static_capacity))
    {
        clear();
        insert<N>(0, src);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    assign(value_type (&& src)[N])
        requires (can_resize || (N == static_capacity))
    {
        clear();
        insert<N>(0, rsl::move(src));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    iterator_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::iterator_at(
            size_type i
            )
        noexcept
    {
        return begin() + i;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::iterator_at(size_type i) const noexcept
    {
        return begin() + i;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            array_view<const value_type> other
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, other);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            move_signal,
            array_view<value_type> other
            ) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, move_signal{}, other);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            const value_type& value
            )
        noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, value);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            value_type&& value
            )
        noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, rsl::move(value));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            const size_type count,
            const value_type& value
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, count, value);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            InputIt first,
            InputIt last
            ) noexcept(move_construct_noexcept && construct_noexcept<iter_value_t<InputIt>>)
        requires (
            can_resize)
    {
        return insert(m_size, first, last);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            const value_type* ptr,
            const size_type count
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, ptr, count);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            const value_type (& src)[N]
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize
        )
    {
        return insert(m_size, src);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::append(
            value_type (&& src)[N]
            ) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        return insert(m_size, rsl::move(src));
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            array_view<const value_type> other
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        const size_type count = other.size();
        split_reserve(pos, count);

        copy_construct_from_unsafe_impl(pos, pos + count, other.begin());

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            move_signal,
            array_view<value_type> other
            ) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        const size_type count = other.size();
        split_reserve(pos, count);

        move_construct_from_unsafe_impl(pos, pos + count, other.begin());

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            const value_type& value
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(pos, 1);
        mem_rsc::construct(1, pos, value);
        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            value_type&& value
            ) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(pos, 1);
        mem_rsc::construct(1, pos, move(value));
        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            size_type count,
            const value_type& value
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(pos, count);
        mem_rsc::construct(count, pos, value);
        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            InputIt first,
            InputIt last
            ) noexcept(move_construct_noexcept && construct_noexcept<iter_value_t<InputIt>>)
        requires (
            can_resize)
    {
        size_type count = iterator_diff(first, last);
        split_reserve(pos, count);

        copy_construct_from_unsafe_impl(pos, pos + count, first);

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            const value_type* ptr,
            const size_type count
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(pos, count);

        copy_construct_from_unsafe_impl(pos, pos + count, ptr);

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            const value_type (& src)[N]
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize
        )
    {
        split_reserve(pos, N);

        copy_construct_from_unsafe_impl(pos, pos + N, src);

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <size_type N>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            value_type (&& src)[N]
            ) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(pos, N);

        move_construct_from_unsafe_impl(pos, pos + N, src);

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(const size_type pos) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(pos < m_size);

        destroy_postfix();

        erase_swap_unsafe_impl(pos);
        --m_size;

        construct_postfix();

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(const_view_type view) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_ptr_out_of_range(view.data(), mem_rsc::get_ptr(), mem_rsc::get_ptr() + m_size);
        rsl_assert_ptr_out_of_range(
                view.data() + view.size(),
                mem_rsc::get_ptr(),
                mem_rsc::get_ptr() + m_size
                );

        size_type beginIndex = view.data() - mem_rsc::get_ptr();
        return erase_swap(beginIndex, view.size());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(const size_type pos, size_type count) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(pos < m_size);
        if (pos + count > m_size) [[unlikely]]
        {
            count = m_size - pos;
        }

        destroy_postfix();

        mem_rsc::destroy(count, pos);

        for (size_type i = 0ull; i < count; ++i)
        {
            mem_rsc::construct(1ull, i + pos, move(*get_ptr_at(m_size - (i + 1ull))));
        }

        m_size -= count;

        construct_postfix();

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename Comp>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(const Comp& comparable) noexcept(move_construct_noexcept)
        requires equality_comparable_with<Comp, T> &&
        can_resize
    {
        return erase_swap([&](const_iterator_type iter) { return (*iter) == comparable; });
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename Func>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(Func&& comparer) noexcept(move_construct_noexcept)
        requires invocable<Func, bool(ConstIter)> &&
        can_resize
    {
        size_type newSize = m_size;
        for (size_type i = m_size; i != 0ull; --i)
        {
            if (comparer(iterator_at(i - 1ull))) [[unlikely]]
            {
                erase_swap_unsafe_impl(i - 1ull);
                --newSize;
            }
        }

        const size_type erasureCount = m_size - newSize;

        if (newSize != m_size)
        {
            destroy_postfix();
            m_size = newSize;
            construct_postfix();
        }

        return erasureCount;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(size_type pos) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(pos < m_size);

        mem_rsc::destroy(1ull, pos);
        destroy_postfix();
        move_shift_elements_unsafe(pos + 1ull, m_size, -1ll);
        --m_size;

        construct_postfix();

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(const_view_type view) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_ptr_out_of_range(view.data(), mem_rsc::get_ptr(), mem_rsc::get_ptr() + m_size);
        rsl_assert_ptr_out_of_range(
                view.data() + view.size(),
                mem_rsc::get_ptr(),
                mem_rsc::get_ptr() + m_size
                );

        size_type beginIndex = view.data() - mem_rsc::get_ptr();
        return erase_shift(beginIndex, view.size());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(const size_type pos, size_type count) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(pos < m_size);
        size_type last = pos + count;
        if (last > m_size) [[unlikely]]
        {
            last = m_size;
            count = last - pos;
        }

        mem_rsc::destroy(count, pos);
        destroy_postfix();

        move_shift_elements_unsafe(last, m_size, -static_cast<diff_type>(count));
        m_size -= count;

        construct_postfix();
        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename Comp>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(const Comp& comparable) noexcept(move_construct_noexcept)
        requires equality_comparable_with<Comp, T> &&
        can_resize
    {
        return erase_shift([&](const_iterator_type iter) { return (*iter) == comparable; });
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    template <typename Func>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(Func&& comparer) noexcept(move_construct_noexcept)
        requires invocable<Func, bool(ConstIter)> &&
        can_resize
    {
        size_type eraseLocation = npos;
        diff_type shift = -1ll;

        size_type newSize = m_size;
        for (size_type i = 0ull; i < m_size; ++i)
        {
            if (comparer(iterator_at(i))) [[unlikely]]
            {
                if (eraseLocation != npos) [[likely]]
                {
                    mem_rsc::destroy(1ull, eraseLocation);
                    move_shift_elements_unsafe(eraseLocation + 1ull, i, shift);
                    --newSize;
                    shift -= 1ll;
                }

                eraseLocation = i;
            }
        }

        if (eraseLocation != npos) [[likely]]
        {
            mem_rsc::destroy(1ull, eraseLocation);
            move_shift_elements_unsafe(eraseLocation + 1ull, newSize, shift);
            --newSize;
        }

        const size_type erasureCount = m_size - newSize;
        if (newSize != m_size) [[likely]]
        {
            destroy_postfix();
            m_size = newSize;
            construct_postfix();
        }

        return erasureCount;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::replace(
            const size_type pos,
            size_type count,
            const_view_type replacement
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
    {
        rsl_assert_out_of_range(pos < m_size);
        size_type last = pos + count;
        if (last > m_size) [[unlikely]]
        {
            last = m_size;
            count = last - pos;
        }

        destroy_postfix();

        mem_rsc::destroy(count, pos);
        const diff_type shift = static_cast<diff_type>(replacement.size()) - count;
        split_reserve(last, shift);
        m_size += shift;
        mem_rsc::copy(replacement.size(), pos, replacement.data());

        construct_postfix();

        return pos;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::at(const size_type i) noexcept
    {
        rsl_assert_out_of_range(i < m_size);
        return *get_ptr_at(i);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::at(const size_type i) const noexcept
    {
        rsl_assert_out_of_range(i < m_size);
        return *get_ptr_at(i);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator[](const size_type i) noexcept
    {
        return at(i);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator[](
            const size_type i
            ) const noexcept
    {
        return at(i);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::data() noexcept
    {
        return mem_rsc::get_ptr();
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::data() const noexcept
    {
        return mem_rsc::get_ptr();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::view_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::view() noexcept
    {
        return view_type(mem_rsc::get_ptr(), m_size);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_view_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::view() const noexcept
    {
        return const_view_type::from_buffer(mem_rsc::get_ptr(), m_size);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator view_type() noexcept
    {
        return view();
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator const_view_type(
            ) const noexcept
    {
        return view();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::view_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::subview(
                size_type offset,
                diff_type count
                ) noexcept
    {
        return view().subview(offset, count);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_view_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::subview(
                size_type offset,
                diff_type count
                ) const noexcept
    {
        return view().subview(offset, count);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::front() noexcept
    {
        return at(0);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::front() const noexcept
    {
        return at(0);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::back() noexcept
    {
        return at(m_size - 1);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr const typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::value_type&
    contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::back() const noexcept
    {
        return at(m_size - 1);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::begin() noexcept
    {
        return contiguous_container_base::iterator_type(mem_rsc::m_ptr);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::begin() const noexcept
    {
        return cbegin();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::cbegin() const noexcept
    {
        return const_iterator_type(mem_rsc::get_ptr());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::end() noexcept
    {
        return iterator_type(mem_rsc::get_ptr() + m_size);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::end() const noexcept
    {
        return cend();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::const_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::cend() const noexcept
    {
        return const_iterator_type(mem_rsc::get_ptr() + m_size);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reverse_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::rbegin() noexcept
    {
        return reverse_iterator_type(end());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                 ContiguousContainerInfo>::const_reverse_iterator_type contiguous_container_base<
        T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                 ContiguousContainerInfo>::const_reverse_iterator_type contiguous_container_base<
        T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::crbegin() const noexcept
    {
        return const_reverse_iterator_type(cend());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reverse_iterator_type
        contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::rend() noexcept
    {
        return reverse_iterator_type(begin());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                 ContiguousContainerInfo>::const_reverse_iterator_type contiguous_container_base<
        T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::rend() const noexcept
    {
        return crend();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr typename contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                 ContiguousContainerInfo>::const_reverse_iterator_type contiguous_container_base<
        T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::crend() const noexcept
    {
        return const_reverse_iterator_type(cbegin());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    maybe_shrink_to_static_storage() noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        if constexpr (internal::is_hybrid_resource_v<mem_rsc>)
        {
            const size_type footprint = calc_memory_size(m_size);

            if (footprint > static_capacity)
            {
                return false;
            }

            if (mem_rsc::is_static_memory()) [[unlikely]]
            {
                return true;
            }

            mem_rsc::move_to_static_memory_and_deallocate(footprint, m_memorySize);
            m_memorySize = static_capacity;

            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::maybe_grow()
        noexcept(move_construct_noexcept)
    {
        if constexpr (can_allocate)
        {
            if constexpr (internal::is_dynamic_resource_v<mem_rsc>)
            {
                if (m_memorySize == 0) [[unlikely]]
                {
                    return resize_capacity_unsafe(1);
                }
            }

            if (m_size == m_memorySize) [[unlikely]]
            {
                return resize_capacity_unsafe(m_memorySize * 2);
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr bool contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    resize_capacity_unsafe(const size_type newCapacity) noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        const size_type oldCapacity = m_memorySize;
        if constexpr (internal::is_dynamic_resource_v<mem_rsc>)
        {
            if (m_memorySize == 0) [[unlikely]]
            {
                rsl_assert_invalid_object(!mem_rsc::get_ptr());
                rsl_assert_invalid_object(m_size == 0ull);

                mem_rsc::allocate(newCapacity);

                if (mem_rsc::get_ptr() == nullptr) [[unlikely]]
                {
                    m_memorySize = 0ull;
                    return false;
                }

                m_memorySize = newCapacity;
                construct_postfix();
                return true;
            }
        }

        if constexpr (internal::is_hybrid_resource_v<mem_rsc>)
        {
            if (newCapacity <= static_capacity) [[unlikely]]
            {
                maybe_shrink_to_static_storage();
                return true;
            }
        }

        if constexpr (is_trivially_copyable_v<T>)
        {
            mem_rsc::reallocate(oldCapacity, newCapacity);

            if (mem_rsc::get_ptr() == nullptr) [[unlikely]]
            {
                m_memorySize = 0ull;
                return false;
            }
        }
        else
        {
            T* newMem = mem_rsc::m_alloc.allocate(newCapacity);
            if (!newMem) [[unlikely]]
            {
                m_memorySize = 0ull;
                mem_rsc::destroy(m_size);
                mem_rsc::set_ptr(nullptr);
                return false;
            }

            const size_type footprint = calc_memory_size(m_size);
            mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), footprint);
            mem_rsc::destroy(footprint);
            mem_rsc::deallocate(oldCapacity);
            mem_rsc::set_ptr(newMem);
        }

        m_memorySize = newCapacity;
        return true;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    copy_assign_impl(
            const value_type* src,
            size_type srcSize,
            void* allocOrFactory
            ) noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        if constexpr (!can_resize)
        {
            rsl_assert_invalid_operation(srcSize != m_size);
        }

        destroy_postfix();

        if constexpr (can_allocate)
        {
            const size_type srcFootprint = calc_memory_size(srcSize);
            if (srcFootprint > m_memorySize || allocOrFactory != nullptr)
            {
                if (mem_rsc::get_ptr()) [[likely]]
                {
                    reset_unsafe_impl();
                    mem_rsc::deallocate(m_memorySize);

                    m_memorySize = 0;
                    m_size = 0;
                }

                rsl_assert_frequent(m_size == 0);
                rsl_assert_frequent(m_memorySize == 0);

                if (allocOrFactory)
                {
                    mem_rsc::m_alloc = *static_cast<mem_rsc::typed_alloc_type*>(allocOrFactory);
                }

                rsl_ensure(resize_capacity_unsafe(srcFootprint));
            }
        }
        else
        {
            if (allocOrFactory)
            {
                mem_rsc::m_factory = *static_cast<mem_rsc::factory_storage_type*>(allocOrFactory);
            }

            const size_type maxSize = calc_max_size();
            if (srcSize > maxSize)
            {
                srcSize = maxSize;
            }
        }

        if (m_size > srcSize)
        {
            reset_unsafe_impl(srcSize);
        }

        copy_construct_from_unsafe_impl(0ull, srcSize, src);
        m_size = srcSize;

        shrink_to_postfix();
        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    move_data_assign_impl(
            const value_type* src,
            size_type srcSize
            ) noexcept(move_assign_noexcept && move_construct_noexcept)
    {
        if constexpr (!can_resize)
        {
            rsl_assert_invalid_operation(srcSize != m_size);
        }

        destroy_postfix();

        if constexpr (can_allocate)
        {
            const size_type srcFootprint = calc_memory_size(srcSize);
            if (srcFootprint > m_memorySize)
            {
                if (mem_rsc::get_ptr()) [[likely]]
                {
                    reset_unsafe_impl();
                    mem_rsc::deallocate(m_memorySize);

                    m_memorySize = 0;
                    m_size = 0;
                }

                rsl_assert_frequent(m_size == 0);
                rsl_assert_frequent(m_memorySize == 0);

                rsl_ensure(resize_capacity_unsafe(srcFootprint));
            }
        }
        else
        {
            const size_type maxSize = calc_max_size();
            if (srcSize > maxSize)
            {
                srcSize = maxSize;
            }
        }

        if (m_size > srcSize)
        {
            reset_unsafe_impl(srcSize);
            m_size = srcSize;
        }

        if (m_size > 0)
        {
            move_assign_from_unsafe_impl(0, m_size, src);
        }

        if (m_size < srcSize)
        {
            move_construct_from_unsafe_impl(
                    m_size,
                    srcSize,
                    src + m_size
                    );
            m_size = srcSize;
        }

        if constexpr (char_type<value_type>)
        {
            if (m_size > 0)
            {
                if (*get_ptr_at(m_size - 1) == static_cast<value_type>('\0'))
                {
                    --m_size;
                }
            }
        }

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    split_reserve(size_type pos, diff_type offset) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(pos < m_size);
        rsl_assert_invalid_operation_frequent(static_cast<diff_type>(pos) + offset >= 0ull);

        const size_type newSize = m_size + offset;
        const size_type newFootprint = calc_memory_size(newSize);

        if constexpr (can_allocate)
        {
            if (m_memorySize == 0) [[unlikely]]
            {
                rsl_ensure(resize_capacity_unsafe(newSize));
            }
            else
            {
                destroy_postfix();

                if (newFootprint > m_memorySize)
                {
                    // offset is always positive here.

                    if constexpr (is_trivially_copyable_v<T>)
                    {
                        mem_rsc::reallocate(m_memorySize, newFootprint);
                        move_shift_elements_unsafe(pos, m_size, offset);
                    }
                    else
                    {
                        T* newMem = mem_rsc::m_alloc.allocate(newFootprint);
                        if (newMem) [[likely]]
                        {
                            mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), pos);
                            mem_rsc::m_alloc.move(newMem + pos + offset, get_ptr_at(pos), m_size - pos);
                        }

                        mem_rsc::deallocate(m_memorySize);
                        mem_rsc::set_ptr(newMem);
                    }

                    m_memorySize = newFootprint;
                }
                else
                {
                    move_shift_elements_unsafe(pos, m_size, offset);
                }
            }
        }
        else
        {
            rsl_assert_invalid_operation(newFootprint <= m_memorySize);
            destroy_postfix();
            move_shift_elements_unsafe(pos, m_size, offset);
        }

        m_size = newSize;

        construct_postfix();
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap_unsafe_impl(size_type pos) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        mem_rsc::destroy(1, pos);

        const size_type lastIndex = m_size - 1ull;
        if (pos != lastIndex) [[likely]]
        {
            mem_rsc::construct(1, pos, move(*get_ptr_at(lastIndex)));
            mem_rsc::destroy(1, lastIndex);
        }
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::copy_assign_from_unsafe_impl(
            const size_type offset,
            const size_type end,
            InputIt srcIter
            ) noexcept(copy_assign_noexcept)
    {
        for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; ++to, ++srcIter)
        {
            *to = *srcIter;
        }
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::copy_construct_from_unsafe_impl(
            const size_type offset,
            const size_type end,
            InputIt srcIter
            ) noexcept(copy_construct_noexcept)
    {
        if constexpr (is_pointer_v<InputIt>)
        {
            mem_rsc::copy(end - offset, offset, srcIter);
        }
        else
        {
            for (size_type i = offset; i != end; ++i, ++srcIter)
            {
                mem_rsc::construct(1, i, *srcIter);
            }
        }
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::move_assign_from_unsafe_impl(
            const size_type offset,
            const size_type end,
            InputIt srcIter
            ) noexcept(move_assign_noexcept)
    {
        for (auto to = mem_rsc::get_ptr() + offset; to != mem_rsc::get_ptr() + end; ++to, ++srcIter)
        {
            *to = move(*srcIter);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <input_iterator InputIt>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::move_construct_from_unsafe_impl(
            const size_type offset,
            const size_type end,
            InputIt srcIter
            ) noexcept(move_construct_noexcept)
    {
        if constexpr (is_pointer_v<InputIt>)
        {
            mem_rsc::move(end - offset, offset, srcIter);
        }
        else
        {
            for (size_type i = offset; i != end; ++i, ++srcIter)
            {
                mem_rsc::construct(1, i, move(*srcIter));
            }
        }
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    template <typename... Args>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::emplace_unsafe_impl(
            const size_type offset,
            const size_type end,
            Args&&... args
            ) noexcept(construct_noexcept<Args...>)
    {
        mem_rsc::construct(end - offset, offset, rsl::forward<Args>(args)...);
    }

    template <
        typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
        ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reset_unsafe_impl(
            const size_type offset,
            size_type end
            ) noexcept
    {
        if (end > m_size) [[unlikely]]
        {
            end = m_size;
        }

        mem_rsc::destroy(end - offset, offset);
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::move_shift_elements_unsafe(
            const size_type offset,
            const size_type end,
            const diff_type shift
            ) noexcept(move_construct_noexcept)
    {
        if (shift > 0ull)
        {
            for (size_type i = end; i != offset; --i)
            {
                size_type index = i - 1ull;
                mem_rsc::construct(1ull, static_cast<size_type>(index + shift), move(*get_ptr_at(index)));
                mem_rsc::destroy(1ull, index);
            }
        }
        else
        {
            for (size_type i = offset; i != end; ++i)
            {
                mem_rsc::construct(1ull, static_cast<size_type>(i + shift), move(*get_ptr_at(i)));
                mem_rsc::destroy(1ull, i);
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::get_ptr_at(
            const size_type i
            ) noexcept
    {
        return mem_rsc::get_ptr() + i;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr const T* contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::get_ptr_at(
            const size_type i
            ) const noexcept
    {
        return mem_rsc::get_ptr() + i;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::shrink_to_postfix() noexcept
    {
        if constexpr (use_post_fix)
        {
            if (m_size > 0) [[likely]]
            {
                const size_type oldSize = m_size;
                while (*get_ptr_at(m_size - 1) == value_type{})
                {
                    --m_size;
                    if (m_size == 0) [[unlikely]]
                    {
                        break;
                    }
                }

                if (oldSize != m_size)
                {
                    mem_rsc::destroy(oldSize - m_size, m_size);
                }
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::construct_postfix() noexcept(construct_noexcept<>)
    {
        if constexpr (use_post_fix)
        {
            if (m_memorySize > 0ull) [[likely]]
            {
                mem_rsc::construct(1ull, m_size);
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                             ContiguousContainerInfo>::destroy_postfix() noexcept
    {
        if constexpr (use_post_fix)
        {
            if (m_memorySize > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter,
                                                  ContiguousContainerInfo>::calc_max_size() const noexcept
    {
        if constexpr (use_post_fix)
        {
            return m_memorySize == 0ull ? 0ull : m_memorySize - 1ull;
        }
        else
        {
            return m_memorySize;
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::calc_memory_size(
            const size_type itemCount
            ) noexcept
    {
        if constexpr (use_post_fix)
        {
            return itemCount + 1ull;
        }
        else
        {
            return itemCount;
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr bool operator==(
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& lhs,
            const contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& rhs
            ) noexcept
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        for (size_type i = 0; i < lhs.size(); ++i)
        {
            if (lhs.at(i) != rhs.at(i))
            {
                return false;
            }
        }

        return true;
    }
} // namespace rsl
