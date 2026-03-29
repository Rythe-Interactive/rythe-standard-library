#pragma once
#include "delegate.hpp"

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(
		const pointer<memory_allocator> allocator
	)
		: m_alloc(allocator),
		  m_invocation()
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(const Functor& instance)
		: m_alloc(),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(
		const pointer<memory_allocator> allocator, const Functor& instance
	)
		: m_alloc(allocator),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(const Functor& instance)
		: m_alloc(),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(
		const pointer<memory_allocator> allocator, const Functor& instance
	)
		: m_alloc(allocator),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(nullptr, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(pointer<memory_allocator> allocator, T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(allocator, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(const T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(nullptr, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(pointer<memory_allocator> allocator, const T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(allocator, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create()
	{
		return delegate(base::template create_element<TMethod>(nullptr));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(pointer<memory_allocator> allocator)
	{
		return delegate(base::template create_element<TMethod>(allocator));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(const Functor& instance)
	{
		return delegate(base::template create_element<Functor>(nullptr, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>
	delegate<ReturnType(ParamTypes...), Factory>::create(
		pointer<memory_allocator> allocator, const Functor& instance
	)
	{
		return delegate(base::template create_element<Functor>(allocator, instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr void
	delegate<ReturnType(ParamTypes...), Factory>::set_allocator(const pointer<memory_allocator> allocator)
		noexcept(is_nothrow_copy_assignable_v<pointer<memory_allocator>>)
	{
		m_alloc = allocator;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr memory_allocator&
	delegate<ReturnType(ParamTypes...), Factory>::get_allocator() noexcept
	{
		return *m_alloc;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr const memory_allocator&
	delegate<ReturnType(ParamTypes...), Factory>::get_allocator() const noexcept
	{
		return *m_alloc;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr bool delegate<ReturnType(ParamTypes...), Factory>::empty() const noexcept
	{
		return m_invocation.stub == nullptr;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr void delegate<ReturnType(ParamTypes...), Factory>::clear() noexcept
	{
		m_invocation = invocation_element();
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr bool delegate<ReturnType(ParamTypes...), Factory>::operator==(nullptr_type) const noexcept
	{
		return empty();
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr bool delegate<ReturnType(ParamTypes...), Factory>::operator!=(nullptr_type) const noexcept
	{
		return !empty();
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr bool delegate<ReturnType(ParamTypes...), Factory>::operator==(
		const multicast_delegate<ReturnType(ParamTypes...)>& other
	) const noexcept
	{
		return other == (*this);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr bool delegate<ReturnType(ParamTypes...), Factory>::operator!=(
		const multicast_delegate<ReturnType(ParamTypes...)>& other
	) const noexcept
	{
		return other != (*this);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>&
	delegate<ReturnType(ParamTypes...), Factory>::assign(T& instance)
	{
		m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
    constexpr delegate<ReturnType(ParamTypes...), Factory>&
	delegate<ReturnType(ParamTypes...), Factory>::assign(const T& instance)
	{
		m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
    constexpr delegate<ReturnType(ParamTypes...), Factory>&
	delegate<ReturnType(ParamTypes...), Factory>::assign()
	{
		m_invocation = base::template create_element<TMethod>(m_alloc);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
    constexpr delegate<ReturnType(ParamTypes...), Factory>&
	delegate<ReturnType(ParamTypes...), Factory>::operator=(const Functor& instance)
	{
		m_invocation = base::template create_element<Functor>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr ReturnType delegate<ReturnType(ParamTypes...), Factory>::operator()(ParamTypes... args
	) const
	{
		return invoke(args...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr ReturnType delegate<ReturnType(ParamTypes...), Factory>::invoke(ParamTypes... args) const
	{
		return (*m_invocation.stub)(*m_invocation.object, args...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr delegate<ReturnType(ParamTypes...), Factory>::delegate(
		const pointer<memory_allocator> allocator, invocation_element&& e
	)
		: m_alloc(allocator),
		  m_invocation(e)
	{
	}
} // namespace rsl
