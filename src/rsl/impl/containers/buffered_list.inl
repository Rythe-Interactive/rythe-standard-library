#pragma once

#include "buffered_list.hpp"

namespace rsl
{
    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::buffered_list(const buffered_list& src)
        noexcept(copy_construct_noexcept)
        : m_size(validate_size(src.m_size))
    {
        copy_construct_from_unsafe_impl(0, m_size, src.m_buffer);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::buffered_list(buffered_list&& src)
        noexcept(move_construct_noexcept)
        : m_size(validate_size(src.m_size))
    {
        move_construct_from_unsafe_impl(0, m_size, src.m_buffer);
    }

    template <typename ValueType, size_type MaxSize>
    template <size_type N>
    constexpr buffered_list<ValueType, MaxSize>::buffered_list(const value_type (&arr)[N])
        noexcept(copy_construct_noexcept)
        : m_size(validate_size(N))
    {
        copy_construct_from_unsafe_impl(0, m_size, arr);
    }

    template <typename ValueType, size_type MaxSize>
    template <size_type N>
    constexpr buffered_list<ValueType, MaxSize>::buffered_list(value_type (&&arr)[N])
        noexcept(move_construct_noexcept)
        : m_size(validate_size(N))
    {
        move_construct_from_unsafe_impl(0, m_size, arr);
    }

    template <typename ValueType, size_type MaxSize>
    template <typename... Args>
    constexpr buffered_list<ValueType, MaxSize> buffered_list<ValueType, MaxSize>::create_in_place(
        const size_type count, Args&&... args
    ) noexcept(construct_noexcept<Args...>)
    {
        buffered_list ret;
        ret.m_size = validate_size(count);
        ret.emplace_unsafe_impl(0, ret.m_size, forward<Args>(args)...);
        return ret;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::buffered_list(view_type src) noexcept(copy_construct_noexcept)
        : m_size(validate_size(src.size()))
    {
        copy_construct_from_unsafe_impl(0, m_size, src.data());
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>&
    buffered_list<ValueType, MaxSize>::operator=(const buffered_list& src)
        noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        rsl_assert_invalid_operation(this != &src);
        copy_assign_impl(src.m_buffer, src.m_size);
        return *this;
    }

    template <typename ValueType, size_type MaxSize>
    template <size_type N>
    constexpr buffered_list<ValueType, MaxSize>&
    buffered_list<ValueType, MaxSize>::operator=(const value_type (&arr)[N])
        noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        copy_assign_impl(arr, N);
        return *this;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>& buffered_list<ValueType, MaxSize>::operator=(view_type src)
        noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        copy_assign_impl(src.data(), src.size());
        return *this;
    }

    template <typename ValueType, size_type MaxSize>
    template <typename... Args>
    constexpr void buffered_list<ValueType, MaxSize>::resize(size_type newSize, Args&&... args)
        noexcept(construct_noexcept<Args...>)
    {
        size_type oldSize = m_size;
        m_size = validate_size(newSize);

        if (m_size >= oldSize)
        {
            emplace_unsafe_impl(oldSize, m_size, rsl::forward<Args>(args)...);
        }
        else
        {
            reset_unsafe_impl(m_size, oldSize);
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::push_back(const value_type& value)
        noexcept(move_construct_noexcept)
    {
        size_type oldSize = m_size++;
        rsl_assert_out_of_range(m_size <= MaxSize);
        copy_construct_from_unsafe_impl(oldSize, m_size, &value);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::push_back(value_type&& value)
        noexcept(move_construct_noexcept)
    {
        size_type oldSize = m_size++;
        rsl_assert_out_of_range(m_size <= MaxSize);
        move_construct_from_unsafe_impl(oldSize, m_size, &value);
    }

    template <typename ValueType, size_type MaxSize>
    template <typename... Args>
    constexpr ValueType& buffered_list<ValueType, MaxSize>::emplace_back(Args&&... args)
        noexcept(construct_noexcept<Args...>)
    {
        size_type oldSize = m_size++;
        rsl_assert_out_of_range(m_size <= MaxSize);
        emplace_unsafe_impl(oldSize, m_size, rsl::forward<Args>(args)...);
        return m_buffer[m_size - 1].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::pop_back(size_type count) noexcept
    {
        size_type oldSize = m_size;
        m_size = count > m_size ? 0 : (m_size - count);
        reset_unsafe_impl(m_size, oldSize);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::clear() noexcept
    {
        pop_back(MaxSize);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr ValueType& buffered_list<ValueType, MaxSize>::at(size_type i) noexcept
    {
        rsl_assert_out_of_range(i < m_size);
        return m_buffer[i].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr const ValueType& buffered_list<ValueType, MaxSize>::at(size_type i) const noexcept
    {
        rsl_assert_out_of_range(i < m_size);
        return m_buffer[i].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr ValueType& buffered_list<ValueType, MaxSize>::raw_at(size_type i) noexcept
    {
        return m_buffer[i].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr const ValueType& buffered_list<ValueType, MaxSize>::raw_at(size_type i) const noexcept
    {
        return m_buffer[i].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr ValueType& buffered_list<ValueType, MaxSize>::operator[](size_type i)
    {
        return at(i);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr const ValueType& buffered_list<ValueType, MaxSize>::operator[](size_type i) const
    {
        return at(i);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr bool buffered_list<ValueType, MaxSize>::empty() const noexcept
    {
        return m_size == 0;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr size_type buffered_list<ValueType, MaxSize>::size() const noexcept
    {
        return m_size;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr size_type buffered_list<ValueType, MaxSize>::capacity() const noexcept
    {
        return MaxSize;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::view_type buffered_list<ValueType, MaxSize>::view() noexcept
    {
        return view_type(&m_buffer->value, m_size);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_view_type
    buffered_list<ValueType, MaxSize>::view() const noexcept
    {
        return const_view_type(&m_buffer->value, m_size);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::view_type buffered_list<ValueType, MaxSize>::raw_view() noexcept
    {
        return view_type(&m_buffer->value, MaxSize);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_view_type
    buffered_list<ValueType, MaxSize>::raw_view() const noexcept
    {
        return const_view_type(&m_buffer->value, MaxSize);
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::operator const value_type*() const noexcept
    {
        return &m_buffer->value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::operator view_type() noexcept
    {
        return view();
    }

    template <typename ValueType, size_type MaxSize>
    constexpr buffered_list<ValueType, MaxSize>::operator const_view_type() const noexcept
    {
        return view();
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::iterator_type buffered_list<ValueType, MaxSize>::begin() noexcept
    {
        return &m_buffer->value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_iterator_type
    buffered_list<ValueType, MaxSize>::begin() const noexcept
    {
        return &m_buffer->value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_iterator_type
    buffered_list<ValueType, MaxSize>::cbegin() const noexcept
    {
        return &m_buffer->value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::iterator_type buffered_list<ValueType, MaxSize>::end() noexcept
    {
        return &m_buffer[m_size].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_iterator_type
    buffered_list<ValueType, MaxSize>::end() const noexcept
    {
        return &m_buffer[m_size].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_iterator_type
    buffered_list<ValueType, MaxSize>::cend() const noexcept
    {
        return &m_buffer[m_size].value;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::reverse_iterator_type
    buffered_list<ValueType, MaxSize>::rend() noexcept
    {
        return reverse_iterator_type(begin());
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_reverse_iterator_type
    buffered_list<ValueType, MaxSize>::rend() const noexcept
    {
        return crend();
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_reverse_iterator_type
    buffered_list<ValueType, MaxSize>::crend() const noexcept
    {
        return const_reverse_iterator_type(cbegin());
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::reverse_iterator_type
    buffered_list<ValueType, MaxSize>::rbegin() noexcept
    {
        return reverse_iterator_type(end());
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_reverse_iterator_type
    buffered_list<ValueType, MaxSize>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename ValueType, size_type MaxSize>
    constexpr typename buffered_list<ValueType, MaxSize>::const_reverse_iterator_type
    buffered_list<ValueType, MaxSize>::crbegin() const noexcept
    {
        return const_reverse_iterator_type(cend());
    }

    template <typename ValueType, size_type MaxSize>
    constexpr size_type buffered_list<ValueType, MaxSize>::validate_size(size_type newSize) noexcept
    {
        return newSize < MaxSize ? newSize : MaxSize;
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::copy_assign_from_unsafe_impl(
        size_type begin, size_type end, const value_type* src
    ) noexcept(copy_assign_noexcept)
    {
        for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
        {
            to->value = *src;
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::copy_construct_from_unsafe_impl(
        size_type begin, size_type end, const value_type* src
    ) noexcept(copy_construct_noexcept)
    {
        for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
        {
            new (&to->value) value_type(*src);
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::move_assign_from_unsafe_impl(
        size_type begin, size_type end, const value_type* src
    ) noexcept(move_assign_noexcept)
    {
        for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
        {
            to->value = rsl::move(*src);
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::move_construct_from_unsafe_impl(
        size_type begin, size_type end, const value_type* src
    ) noexcept(move_construct_noexcept)
    {
        for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
        {
            new (&to->value) value_type(rsl::move(*src));
        }
    }

    template <typename ValueType, size_type MaxSize>
    template <typename... Args>
    constexpr void
    buffered_list<ValueType, MaxSize>::emplace_unsafe_impl(size_type begin, size_type end, Args&&... args)
        noexcept(construct_noexcept<Args...>)
    {
        for (auto* to = m_buffer + begin; to != m_buffer + end; to++)
        {
            new (&to->value) value_type(rsl::forward<Args>(args)...);
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::reset_unsafe_impl(size_type begin, size_type end) noexcept
    {
        for (size_type i = begin; i < end; i++)
        {
            m_buffer[i].value.~value_type();
        }
    }

    template <typename ValueType, size_type MaxSize>
    constexpr void buffered_list<ValueType, MaxSize>::copy_assign_impl(const value_type* src, size_type srcSize)
        noexcept(copy_assign_noexcept && copy_construct_noexcept)
    {
        if (m_size >= srcSize)
        {
            reset_unsafe_impl(srcSize, m_size);
            m_size = srcSize;
            copy_assign_from_unsafe_impl(0, m_size, src);
        }
        else
        {
            size_type effectiveSize = validate_size(srcSize);
            copy_assign_from_unsafe_impl(0, m_size, src);
            copy_construct_from_unsafe_impl(m_size, effectiveSize, src);
            m_size = effectiveSize;
        }
    }
} // namespace rsl
