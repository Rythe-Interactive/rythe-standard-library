#pragma once
#include "contiguous_container_base.hpp"

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
        size_type memorySize;
        if constexpr (use_post_fix)
        {
            memorySize = src.m_size + 1ull;
        }
        else
        {
            memorySize = src.m_size;
        }

        reserve(src.m_size);
        m_size = src.m_size;
        // will copy postfix from src if `use_post_fix`
        copy_construct_from_unsafe_impl(0ull, memorySize, src.get_ptr());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::contiguous_container_base(
            contiguous_container_base&& src
            ) noexcept(move_construct_container_noexcept)
        : mem_rsc(internal::alloc_and_factory_only_signal, rsl::move(src))
    {
        size_type memorySize;
        if constexpr (use_post_fix)
        {
            memorySize = src.m_size + 1ull;
        }
        else
        {
            memorySize = src.m_size;
        }

        if (src.is_static_memory())
        {
            mem_rsc::move(memorySize, 0ull, src.get_ptr());
            return;
        }

        mem_rsc::set_ptr(src.get_ptr());
        m_size = src.m_size;
        m_capacity = src.m_capacity;

        src.m_size = src.m_capacity = 0ull;
        src.set_ptr(nullptr);
        if constexpr (use_post_fix && !internal::is_dynamic_resource_v<mem_rsc>)
        {
            src.construct(1ull);
        }
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
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>) requires(can_allocate)
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
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>) requires(can_allocate)
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
            if constexpr (use_post_fix)
            {
                rsl_assert_invalid_operation((count + 1) <= static_capacity);
            }
            else
            {
                rsl_assert_invalid_operation(count <= static_capacity);
            }
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

        if constexpr (use_post_fix)
        {
            result.construct(1ull, result.m_size);
        }

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
            if constexpr (use_post_fix)
            {
                rsl_assert_invalid_operation((count + 1) <= static_capacity);
            }
            else
            {
                rsl_assert_invalid_operation(count <= static_capacity);
            }
        }

        if constexpr (can_resize)
        {
            result.m_size = count;
        }

        result.move_construct_from_unsafe_impl(0ull, result.m_size, ptr);

        if constexpr (use_post_fix)
        {
            result.construct(1ull, result.m_size);
        }

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
        template <size_type N, typename Container, typename Type, typename... Types>
        void container_construct_items(Container& container, Type&& arg, Types&&... args)
        {
            container.construct(1ull, N, std::forward<Type>(arg));
            if constexpr (sizeof...(Types) != 0)
            {
                container_construct_items<N + 1ull>(container, std::forward<Types>(args)...);
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <explicitly_convertible_to<T> ... ItemTypes>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_variadic_items(
            ItemTypes&&... items
            ) noexcept(noexcept_construct_from_all<ItemTypes...>)
    {
        if constexpr (can_allocate)
        {
            contiguous_container_base result;
            result.reserve(sizeof...(items));
            (result.push_back(items), ...);

            return result;
        }
        else
        {
            static_assert(sizeof...(items) <= static_capacity);
            contiguous_container_base result;

            if constexpr (can_resize)
            {
                result.m_size = sizeof...(items);
            }

            internal::container_construct_items<0>(result, std::forward<ItemTypes>(items)...);
            return result;
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo> contiguous_container_base<T, Alloc
        , Factory, Iter, ConstIter, ContiguousContainerInfo>::from_string_length(const T* str, T terminator) noexcept
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

        if constexpr (use_post_fix)
        {
            result.construct(1ull, result.m_size);
        }

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
        return m_capacity;
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
        m_capacity = src.m_capacity;

        src.m_size = src.m_capacity = 0ull;
        src.set_ptr(nullptr);
        if constexpr (use_post_fix && !internal::is_dynamic_resource_v<mem_rsc>)
        {
            src.construct(1ull);
        }
        return *this;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <size_type N>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& contiguous_container_base<T,
        Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator=(
            const value_type (& arr)[N]
            ) noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        copy_assign_impl(arr, N);
        return *this;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    template <size_type N>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& contiguous_container_base<T,
        Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator=(
            value_type (&& arr)[N]
            ) noexcept(move_assign_noexcept && move_construct_noexcept)
    {
        move_data_assign_impl(arr, N);
        return *this;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator ConstIter, typename
              ContiguousContainerInfo>
    constexpr contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>& contiguous_container_base<T,
        Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::operator=(
            view_type src
            ) noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        copy_assign_impl(src.data(), src.size());
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
        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        if constexpr (can_allocate)
        {
            reserve(newSize);
        }
        else
        {
            rsl_assert_invalid_operation(newSize <= m_capacity);
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

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::reserve(
            const size_type newCapacity
            )
        noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        if (newCapacity < m_capacity) [[unlikely]]
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
        reset_unsafe_impl();
        m_size = 0ull;
        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::construct(1ull);
            }
        }
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
                if constexpr (use_post_fix)
                {
                    mem_rsc::deallocate(m_capacity + 1);
                }
                else
                {
                    mem_rsc::deallocate(m_capacity);
                }
            }
            return;
        }

        if constexpr (internal::is_dynamic_resource_v<mem_rsc>)
        {
            rsl_ensure(resize_capacity_unsafe(m_size));
        }
        else if constexpr (internal::is_hybrid_resource_v<mem_rsc>)
        {
            maybe_shrink_to_static_storage();
            if (m_capacity != static_capacity)
            {
                rsl_ensure(resize_capacity_unsafe(m_size));
            }
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::push_back(
            const value_type& value
            )
        noexcept(copy_construct_noexcept)
        requires (can_resize)
    {
        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, value);
        ++m_size;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::push_back(
            value_type&& value
            )
        noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, move(value));
        ++m_size;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
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
        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        rsl_ensure(maybe_grow());
        emplace_unsafe_impl(m_size, m_size + 1, rsl::forward<Args>(args)...);
        ++m_size;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }

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

        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        reset_unsafe_impl(m_size - count);
        m_size -= count;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::clear()
        noexcept
        requires (can_resize)
    {
        reset_unsafe_impl();

        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
                mem_rsc::construct(1ull);
            }
        }

        m_size = 0;
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

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::insert(
            size_type pos,
            const value_type& value
            ) noexcept(move_construct_noexcept && copy_construct_noexcept)
        requires (can_resize)
    {
        split_reserve(
                pos,
                1,
                m_size == m_capacity ? m_capacity * 2 : m_size + 1
                );

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
        split_reserve(
                pos,
                1,
                m_size == m_capacity ? m_capacity * 2 : m_size + 1
                );

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
        split_reserve(pos, count, m_size + count);

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
        split_reserve(pos, count, m_size + count);

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
        split_reserve(pos, count, m_size + count);

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
        split_reserve(pos, N, m_size + N);

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
        split_reserve(pos, N, m_size + N);

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

        if constexpr (use_post_fix)
        {
            mem_rsc::destroy(1, m_size);
        }

        erase_swap_impl(pos);

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1, m_size);
        }

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
        return erase_swap(beginIndex, beginIndex + view.size());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap(size_type first, size_type last) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(first < m_size);
        rsl_assert_invalid_parameters(first < last);
        if (last > m_size) [[unlikely]]
        {
            last = m_size;
        }

        if constexpr (use_post_fix)
        {
            mem_rsc::destroy(1, m_size);
        }

        size_type count = last - first;
        mem_rsc::destroy(count, first);

        for (size_type i = 0; i < count; ++i)
        {
            mem_rsc::construct(1, i + first, move(*get_ptr_at(m_size - (i + 1))));
        }

        m_size -= count;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1, m_size);
        }

        return first;
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
        const size_type originalSize = m_size;

        size_type erasureCount = 0ull;
        for (size_type i = originalSize; i != 0; --i)
        {
            if (comparer(iterator_at(i - 1))) [[unlikely]]
            {
                erase_swap_impl(i - 1);
                erasureCount++;
            }
        }

        if constexpr (use_post_fix)
        {
            if (erasureCount != 0ull) [[likely]]
            {
                mem_rsc::destroy(1, originalSize);
                mem_rsc::construct(1, m_size);
            }
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

        mem_rsc::destroy(1, pos);
        move_shift_elements_unsafe(pos + 1, m_size, -1ll);
        --m_size;

        if constexpr (use_post_fix)
        {
            mem_rsc::destroy(1ull, m_size + 1ull);
            mem_rsc::construct(1ull, m_size);
        }

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
        return erase_shift(beginIndex, beginIndex + view.size());
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr size_type contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_shift(size_type first, size_type last) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        rsl_assert_out_of_range(first < m_size);
        rsl_assert_invalid_parameters(first < last);
        if (last > m_size) [[unlikely]]
        {
            last = m_size;
        }

        const size_type count = last - first;
        mem_rsc::destroy(count, first);
        move_shift_elements_unsafe(last, m_size, -static_cast<diff_type>(count));
        m_size -= count;

        if constexpr (use_post_fix)
        {
            if (count != 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size + count);
                mem_rsc::construct(1ull, m_size);
            }
        }

        return first;
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

        const size_type originalSize = m_size;

        size_type erasureCount = 0ull;
        for (size_type i = 0; i < originalSize; i++)
        {
            if (comparer(iterator_at(i))) [[unlikely]]
            {
                if (eraseLocation != npos) [[likely]]
                {
                    mem_rsc::destroy(1, eraseLocation);
                    move_shift_elements_unsafe(eraseLocation + 1, i, shift);
                    --m_size;
                    shift -= 1ll;
                }

                erasureCount++;
                eraseLocation = i;
            }
        }

        if (eraseLocation != npos) [[likely]]
        {
            mem_rsc::destroy(1, eraseLocation);
            move_shift_elements_unsafe(eraseLocation + 1, m_size, shift);
            --m_size;
        }

        if constexpr (use_post_fix)
        {
            if (erasureCount != 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, originalSize);
                mem_rsc::construct(1ull, m_size);
            }
        }

        return erasureCount;
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
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    maybe_shrink_to_static_storage() noexcept(move_construct_noexcept)
        requires (can_allocate)
    {
        if constexpr (internal::is_hybrid_resource_v<mem_rsc>)
        {
            size_type memorySize;
            size_type memoryCapacity;
            if constexpr (use_post_fix)
            {
                memorySize = m_size + 1ull;
                memoryCapacity = m_capacity + 1ull;
            }
            else
            {
                memorySize = m_size;
                memoryCapacity = m_capacity;
            }

            if (memorySize > static_capacity)
            {
                return;
            }

            if (mem_rsc::is_static_memory()) [[unlikely]]
            {
                return;
            }

            mem_rsc::move_to_static_memory_and_deallocate(memorySize, memoryCapacity);
            m_capacity = static_capacity;
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
                if (m_capacity == 0) [[unlikely]]
                {
                    return resize_capacity_unsafe(1);
                }
            }

            if (m_size == m_capacity) [[unlikely]]
            {
                return resize_capacity_unsafe(m_capacity * 2);
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
        size_type newMemorySize;
        size_type oldMemorySize;
        if constexpr (use_post_fix)
        {
            newMemorySize = newCapacity + 1ull;
            oldMemorySize = m_capacity + 1ull;
        }
        else
        {
            newMemorySize = newCapacity;
            oldMemorySize = m_capacity;
        }

        if constexpr (internal::is_dynamic_resource_v<mem_rsc>)
        {
            if (m_capacity == 0) [[unlikely]]
            {
                rsl_assert_invalid_object(!mem_rsc::get_ptr());

                mem_rsc::allocate(newMemorySize);

                if (mem_rsc::get_ptr() == nullptr) [[unlikely]]
                {
                    m_capacity = 0ull;
                    return false;
                }

                m_capacity = newCapacity;

                if constexpr (use_post_fix)
                {
                    mem_rsc::construct(1ull, 0);
                }

                return true;
            }
        }

        if constexpr (internal::is_hybrid_resource_v<mem_rsc>)
        {
            if (newMemorySize <= static_capacity) [[unlikely]]
            {
                maybe_shrink_to_static_storage();
                return true;
            }
        }

        if constexpr (is_trivially_copyable_v<T>)
        {
            mem_rsc::reallocate(oldMemorySize, newMemorySize);

            if (mem_rsc::get_ptr() == nullptr) [[unlikely]]
            {
                m_capacity = 0ull;
                return false;
            }
        }
        else
        {
            T* newMem = mem_rsc::m_alloc.allocate(newMemorySize);
            if (!newMem) [[unlikely]]
            {
                m_capacity = 0ull;
                mem_rsc::destroy(m_size);
                mem_rsc::set_ptr(nullptr);
                return false;
            }

            mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), m_size);
            mem_rsc::destroy(m_size);
            mem_rsc::deallocate(oldMemorySize);
            mem_rsc::set_ptr(newMem);

            if constexpr (use_post_fix)
            {
                mem_rsc::construct(1ull, m_size);
            }
        }

        m_capacity = newCapacity;
        return true;
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    copy_assign_impl(
            const value_type* src,
            size_type srcSize,
            const void* allocOrFactory
            ) noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        if constexpr (!can_resize)
        {
            rsl_assert_invalid_operation(srcSize != m_size);
        }

        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        if constexpr (can_allocate)
        {
            if (srcSize > m_capacity || allocOrFactory != nullptr)
            {
                if (mem_rsc::get_ptr()) [[likely]]
                {
                    reset_unsafe_impl();

                    if constexpr (use_post_fix)
                    {
                        mem_rsc::deallocate(m_capacity + 1ull);
                    }
                    else
                    {
                        mem_rsc::deallocate(m_capacity);
                    }

                    m_capacity = 0;
                    m_size = 0;
                }

                rsl_assert_frequent(m_size == 0);
                rsl_assert_frequent(m_capacity == 0);

                if (allocOrFactory)
                {
                    mem_rsc::m_alloc = *static_cast<const mem_rsc::typed_alloc_type*>(allocOrFactory);
                }

                reserve(srcSize);
            }
        }
        else
        {
            if (allocOrFactory)
            {
                mem_rsc::m_factory = *static_cast<mem_rsc::factory_storage_type*>(allocOrFactory);
            }

            if (srcSize > m_capacity)
            {
                srcSize = m_capacity;
            }
        }

        if (m_size > srcSize)
        {
            reset_unsafe_impl(srcSize);
            m_size = srcSize;
        }

        if (m_size > 0)
        {
            copy_assign_from_unsafe_impl(0, m_size, src);
        }

        if (m_size < srcSize)
        {
            copy_construct_from_unsafe_impl(
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

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
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

        if constexpr (use_post_fix)
        {
            if (m_capacity > 0ull) [[likely]]
            {
                mem_rsc::destroy(1ull, m_size);
            }
        }

        if constexpr (can_allocate)
        {
            if (srcSize > m_capacity)
            {
                if (mem_rsc::get_ptr()) [[likely]]
                {
                    reset_unsafe_impl();

                    if constexpr (use_post_fix)
                    {
                        mem_rsc::deallocate(m_capacity + 1ull);
                    }
                    else
                    {
                        mem_rsc::deallocate(m_capacity);
                    }

                    m_capacity = 0;
                    m_size = 0;
                }

                rsl_assert_frequent(m_size == 0);
                rsl_assert_frequent(m_capacity == 0);

                reserve(srcSize);
            }
        }
        else
        {
            if (srcSize > m_capacity)
            {
                srcSize = m_capacity;
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

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    split_reserve(size_type pos, const size_type count, const size_type newSize) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        if constexpr (can_allocate)
        {
            if (m_capacity == 0) [[unlikely]]
            {
                rsl_ensure(resize_capacity_unsafe(newSize));
            }
            else
            {
                size_type oldMemorySize;
                size_type newMemorySize;
                if constexpr (use_post_fix)
                {
                    newMemorySize = newSize + 1ull;
                    oldMemorySize = m_capacity + 1ull;
                }
                else
                {
                    newMemorySize = newSize;
                    oldMemorySize = m_capacity;
                }

                if constexpr (use_post_fix)
                {
                    mem_rsc::destroy(1ull, m_size);
                }

                if (newSize > m_capacity)
                {
                    if constexpr (is_trivially_copyable_v<T>)
                    {
                        mem_rsc::reallocate(oldMemorySize, newMemorySize);
                        move_shift_elements_unsafe(pos, m_size, count);
                    }
                    else
                    {
                        T* newMem = mem_rsc::m_alloc.allocate(newMemorySize);
                        if (newMem) [[likely]]
                        {
                            mem_rsc::m_alloc.move(newMem, mem_rsc::get_ptr(), pos);
                            mem_rsc::m_alloc.move(newMem + pos, get_ptr_at(pos), pos + count);
                        }

                        mem_rsc::deallocate(oldMemorySize);
                        mem_rsc::set_ptr(newMem);
                    }

                    m_capacity = newSize;
                }
                else
                {
                    move_shift_elements_unsafe(pos, m_size, count);
                }
            }
        }
        else
        {
            rsl_assert_invalid_operation(newSize <= m_capacity);
            if constexpr (use_post_fix)
            {
                mem_rsc::destroy(1ull, m_size);
            }
            move_shift_elements_unsafe(pos, m_size, count);
        }

        m_size = newSize;

        if constexpr (use_post_fix)
        {
            mem_rsc::construct(1ull, m_size);
        }
    }

    template <typename T, allocator_type Alloc, factory_type Factory, contiguous_iterator Iter, contiguous_iterator
              ConstIter, typename ContiguousContainerInfo>
    constexpr void contiguous_container_base<T, Alloc, Factory, Iter, ConstIter, ContiguousContainerInfo>::
    erase_swap_impl(size_type pos) noexcept(move_construct_noexcept)
        requires (can_resize)
    {
        --m_size;
        mem_rsc::destroy(1, pos);

        if (pos != m_size) [[likely]]
        {
            mem_rsc::construct(1, pos, move(*get_ptr_at(m_size)));
            mem_rsc::destroy(1, m_size);
        }

        if constexpr (use_post_fix)
        {
            mem_rsc::destroy(1, m_size + 2);
            mem_rsc::construct(1, m_size + 1);
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
            for (size_type i = offset; i != end; i++, ++srcIter)
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
            for (size_type i = offset; i != end; i++, ++srcIter)
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
        for (size_type i = offset; i != end; i++)
        {
            mem_rsc::construct(1, static_cast<size_type>(i + shift), move(*get_ptr_at(i)));
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
