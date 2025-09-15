#pragma once
#include "multicast_delegate.hpp"

namespace rsl
{
    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::multicast_delegate(
            const value_type& val
            ) noexcept : m_invocationList(invocation_container::create_in_place(1, val.m_invocation)) {}

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::multicast_delegate(
            const allocator_storage_type& allocStorage
            ) noexcept(is_nothrow_constructible_v<invocation_container, const allocator_storage_type&>)
        : m_invocationList(allocStorage) {}

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::multicast_delegate(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_constructible_v<invocation_container, const factory_storage_type&>)
        : m_invocationList(factoryStorage) {}

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::multicast_delegate(
            const allocator_storage_type& allocStorage,
            const factory_storage_type& factoryStorage
            )
        noexcept(is_nothrow_constructible_v<
            invocation_container, const allocator_storage_type&, const factory_storage_type&>)
        : m_invocationList(allocStorage, factoryStorage) {}

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::empty() const noexcept
    {
        return m_invocationList.empty();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr void multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::clear() noexcept
    {
        m_invocationList.clear();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::size() const noexcept
    {
        return m_invocationList.size();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr void multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::reserve(
            size_type newCap
            ) noexcept
    {
        m_invocationList.reserve(newCap);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::capacity() const noexcept
    {
        return m_invocationList.capacity();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_t& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_allocator() noexcept
    {
        return m_invocationList.get_allocator();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_t& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_allocator() const noexcept
    {
        return m_invocationList.get_allocator();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr void multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::set_factory(
            const factory_storage_type& factoryStorage
            ) noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
    {
        m_invocationList.set_factory(factoryStorage);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::factory_t& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_factory() noexcept
    {
        return m_invocationList.get_factory();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::factory_t& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_factory() const noexcept
    {
        return m_invocationList.get_factory();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_storage_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_allocator_storage() noexcept
    {
        return m_invocationList.get_allocator_storage();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_storage_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_allocator_storage() const noexcept
    {
        return m_invocationList.get_allocator_storage();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::factory_storage_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_factory_storage() noexcept
    {
        return m_invocationList.get_factory_storage();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::factory_storage_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::get_factory_storage() const noexcept
    {
        return m_invocationList.get_factory_storage();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::begin() noexcept
    {
        return m_invocationList.begin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::cbegin() const noexcept
    {
        return m_invocationList.cbegin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::begin() const noexcept
    {
        return cbegin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::rbegin() noexcept
    {
        return m_invocationList.rbegin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::crbegin() const noexcept
    {
        return m_invocationList.crbegin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::rbegin() const noexcept
    {
        return crbegin();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::end() noexcept
    {
        return m_invocationList.end();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::cend() const noexcept
    {
        return m_invocationList.cend();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::end() const noexcept
    {
        return cend();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::rend() noexcept
    {
        return m_invocationList.rend();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::crend() const noexcept
    {
        return m_invocationList.crend();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::rend() const noexcept
    {
        return crend();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::iterator_type multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::iterator_at(size_type i) noexcept
    {
        return m_invocationList.iterator_at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::const_iterator_type multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::iterator_at(size_type i) const noexcept
    {
        return m_invocationList.iterator_at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::at(size_type i) noexcept
    {
        return m_invocationList.at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::at(size_type i) const noexcept
    {
        return m_invocationList.at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator[](const size_type i) noexcept
    {
        return at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator[](const size_type i) const noexcept
    {
        return at(i);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::view_type multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::view() noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::const_view_type multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::view() const noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator view_type() noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator const_view_type(
            ) const noexcept
    {
        return m_invocationList.view();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::front() noexcept
    {
        return m_invocationList.front();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::front() const noexcept
    {
        return m_invocationList.front();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::back() noexcept
    {
        return m_invocationList.back();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr const typename multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::value_type& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::back() const noexcept
    {
        return m_invocationList.back();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator==(nullptr_type) const noexcept
    {
        return empty();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator!=(nullptr_type) const noexcept
    {
        return !empty();
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator==(const value_type& other) const noexcept
    {
        return size() == 1 && at(0) == other;
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator!=(const value_type& other) const noexcept
    {
        return !(*this == other);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(const value_type& e)
    {
        m_invocationList.push_back(e);
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(value_type&& e)
    {
        m_invocationList.push_back(move(e));
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(const T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back()
    {
        return push_back(base::template create_element<TMethod>(m_invocationList.get_allocator_storage()));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator+=(const value_type& another)
    {
        return push_back(another);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator+=(value_type&& another)
    {
        return push_back(move(another));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator+=(T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator+=(const T& instance)
    {
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator+=(const Functor& instance)
    {
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::erase(const size_type pos)
    {
        return m_invocationList.erase_swap(pos);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr size_type multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::erase(
            const size_type pos,
            const size_type count
            )
    {
        return m_invocationList.erase_shift(pos, count);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr void multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::pop_back()
    {
        erase(size() - 1);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove(const value_type& del)
    {
        return remove(del.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove(T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove(const T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove()
    {
        return remove(base::template create_id<TMethod>());
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return remove(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains(
            const value_type& del
            ) const noexcept
    {
        return contains(del.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains(
            T& instance
            ) const noexcept
    {
        return contains(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains(
            const T& instance
            ) const noexcept
    {
        return contains(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <ReturnType (* TMethod)(ParamTypes...)>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains() const noexcept
    {
        return contains(base::template create_id<TMethod>());
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <functor Functor>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains(const Functor& instance) const noexcept
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        return contains(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator-=(const value_type& another)
    {
        return remove(another.m_invocation.id);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator-=(T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator-=(const T& instance)
    {
        return remove(base::template create_id<T, TMethod>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator-=(const Functor& instance)
    {
        return remove(base::template create_id<Functor>(instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator=(const value_type& del)
    {
        clear();
        return push_back(del);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator=(value_type&& del)
    {
        clear();
        return push_back(move(del));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator=(T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator=(const T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::operator=(const Functor& instance)
    {
        clear();
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(const value_type& del)
    {
        clear();
        return push_back(del);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(value_type&& del)
    {
        clear();
        return push_back(move(del));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...)>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <typename T, ReturnType (T::* TMethod)(ParamTypes...) const>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(const T& instance)
    {
        clear();
        return push_back(base::template create_element<T, TMethod>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <functor Functor>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(const Functor& instance)
        requires invocable<Functor, ReturnType(ParamTypes...)>
    {
        clear();
        return push_back(base::template create_element<Functor>(m_invocationList.get_allocator_storage(), instance));
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    template <input_iterator InputIt>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::assign(InputIt first, InputIt last)
    {
        m_invocationList.assign(first, last);
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator()(
            ParamTypes... args
            ) const
    {
        return invoke(args...);
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr auto multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::invoke(
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

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::multicast_delegate(
            invocation_container&& e
            ) : m_invocationList(move(e)) {}

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::remove(id_type id)
    {
        m_invocationList.erase_swap([id](const value_type* elem) { return elem->m_invocation.id == id; });
        return *this;
    }

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr bool multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>::contains(
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

    template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
    constexpr multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>& multicast_delegate<
        ReturnType(ParamTypes...), Alloc, Factory>::push_back(invocation_element&& elem)
    {
        return push_back(value_type(m_invocationList.get_allocator_storage(), move(elem)));
    }
} // namespace rsl
