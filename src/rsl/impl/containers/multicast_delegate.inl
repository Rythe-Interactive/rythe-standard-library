#pragma once
#include "multicast_delegate.hpp"

namespace rsl
{
    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>::multicast_delegate(
            const value_type& val
            ) noexcept : m_invocationList(invocation_container::create_in_place(1, val.m_invocation)) {}

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>::multicast_delegate(
            allocator_storage allocator
            ) noexcept(is_nothrow_constructible_v<invocation_container, allocator_storage>)
        : m_invocationList(allocator) {}

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::empty() const noexcept
    {
        return m_invocationList.empty();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr void multicast_delegate<ReturnType(ParamTypes...)>::clear() noexcept
    {
        m_invocationList.clear();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...)>::size() const noexcept
    {
        return m_invocationList.size();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr void multicast_delegate<ReturnType(ParamTypes...)>::reserve(
            size_type newCap
            ) noexcept
    {
        m_invocationList.reserve(newCap);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...)>::capacity() const noexcept
    {
        return m_invocationList.capacity();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr allocator_storage multicast_delegate<ReturnType(ParamTypes...)>::get_allocator() const noexcept
    {
        return m_invocationList.get_allocator();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::begin() noexcept
    {
        return m_invocationList.begin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::cbegin() const noexcept
    {
        return m_invocationList.cbegin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::begin() const noexcept
    {
        return cbegin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::rbegin() noexcept
    {
        return m_invocationList.rbegin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::crbegin() const noexcept
    {
        return m_invocationList.crbegin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::end() noexcept
    {
        return m_invocationList.end();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::cend() const noexcept
    {
        return m_invocationList.cend();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::end() const noexcept
    {
        return cend();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::rend() noexcept
    {
        return m_invocationList.rend();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::crend() const noexcept
    {
        return m_invocationList.crend();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::rend() const noexcept
    {
        return crend();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::iterator_type multicast_delegate<
        ReturnType(ParamTypes...)>::iterator_at(size_type i) noexcept
    {
        return m_invocationList.iterator_at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::const_iterator_type multicast_delegate<
        ReturnType(ParamTypes...)>::iterator_at(size_type i) const noexcept
    {
        return m_invocationList.iterator_at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::at(size_type i) noexcept
    {
        return m_invocationList.at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::at(size_type i) const noexcept
    {
        return m_invocationList.at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::operator[](const size_type i) noexcept
    {
        return at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::operator[](const size_type i) const noexcept
    {
        return at(i);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::view_type multicast_delegate<
        ReturnType(ParamTypes...)>::view() noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::const_view_type multicast_delegate<
        ReturnType(ParamTypes...)>::view() const noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>::operator view_type() noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>::operator const_view_type(
            ) const noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::front() noexcept
    {
        return m_invocationList.front();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::front() const noexcept
    {
        return m_invocationList.front();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::back() noexcept
    {
        return m_invocationList.back();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...)>::value_type& multicast_delegate<
        ReturnType(ParamTypes...)>::back() const noexcept
    {
        return m_invocationList.back();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::operator==(nullptr_type) const noexcept
    {
        return empty();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::operator!=(nullptr_type) const noexcept
    {
        return !empty();
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::operator==(const value_type& other) const noexcept
    {
        return size() == 1 && at(0) == other;
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::operator!=(const value_type& other) const noexcept
    {
        return !(*this == other);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(const value_type& e)
    {
        m_invocationList.push_back(e);
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(value_type&& e)
    {
        m_invocationList.push_back(move(e));
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(const T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back()
    {
        return push_back(base::template create_element<TMethod>(m_invocationList.get_allocator()));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator+=(const value_type& another)
    {
        return push_back(another);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator+=(value_type&& another)
    {
        return push_back(move(another));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator+=(T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator+=(const T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator+=(const Functor& instance)
    {
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...)>::erase(const size_type pos)
    {
        return m_invocationList.erase_swap(pos);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...)>::erase(
            const size_type pos,
            const size_type count
            )
    {
        return m_invocationList.erase_shift(pos, count);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr void multicast_delegate<ReturnType(ParamTypes...)>::pop_back()
    {
        erase(size() - 1);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove(const value_type& del)
    {
        return remove(del.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove(T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove(const T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove()
    {
        return remove(base::template create_id<TMethod>());
    }

    template <typename ReturnType, typename... ParamTypes>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return remove(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains(
            const value_type& del
            ) const noexcept
    {
        return contains(del.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains(
            T& instance
            ) const noexcept
    {
        return contains(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains(
            const T& instance
            ) const noexcept
    {
        return contains(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains() const noexcept
    {
        return contains(base::template create_id<TMethod>());
    }

    template <typename ReturnType, typename... ParamTypes>
    template <functor Functor>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains(const Functor& instance) const noexcept
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return contains(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator-=(const value_type& another)
    {
        return remove(another.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator-=(T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator-=(const T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator-=(const Functor& instance)
    {
        return remove(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator=(const value_type& del)
    {
        clear();
        return push_back(del);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator=(value_type&& del)
    {
        clear();
        return push_back(move(del));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator=(T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator=(const T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::operator=(const Functor& instance)
    {
        clear();
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(const value_type& del)
    {
        clear();
        return push_back(del);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(value_type&& del)
    {
        clear();
        return push_back(move(del));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(const T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        clear();
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator(), instance));
    }

    template <typename ReturnType, typename... ParamTypes>
    template <input_iterator InputIt>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::assign(InputIt first, InputIt last)
    {
        m_invocationList.assign(first, last);
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::operator()(
            ParamTypes... args
            ) const
    {
        return invoke(args...);
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...)>::invoke(
            ParamTypes... args
            ) const -> invocation_result_t<ReturnType>
    {
        if constexpr (same_as<ReturnType, void>)
        {
            for (auto& item : m_invocationList)
            {
                item.invoke(args...);
            }
            return;
        }
        else
        {
            invocation_result_t<ReturnType> result;
            result.reserve(size());
            for (auto& item : m_invocationList)
            {
                result.push_back(item.invoke(args...));
            }

            return result;
        }
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>::multicast_delegate(
            invocation_container&& e
            ) : m_invocationList(move(e)) {}

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::remove(id_type id)
    {
        m_invocationList.erase_swap([id](const value_type* elem) { return elem->m_invocation.id == id; });
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...)>::contains(
            id_type id
            ) const noexcept
    {
        for (auto& element : m_invocationList)
        {
            if (element.m_invocation.id == id)
            {
                return true;
            }
        }

        return false;
    }

    template <typename ReturnType, typename... ParamTypes>
    constexpr multicast_delegate<ReturnType(ParamTypes...)>& multicast_delegate<
        ReturnType(ParamTypes...)>::push_back(invocation_element&& elem)
    {
        return push_back(value_type(m_invocationList.get_allocator(), move(elem)));
    }
} // namespace rsl
