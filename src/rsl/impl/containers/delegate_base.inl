#pragma once

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element::invocation_element(
		allocator_storage allocator, void* object, const stub_type stub, const id_type id, deleter_type deleter
	)
		noexcept(is_nothrow_constructible_v<
				 managed_resource<void*, Factory>, allocator_storage, deleter_type, void*>)
		: object(allocator, deleter ? deleter : default_deleter, object),
		  ownsData(deleter != nullptr),
		  stub(stub),
		  id(id)
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
    constexpr delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element::invocation_element(
		const delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element& other
	) noexcept(is_nothrow_copy_constructible_v<managed_resource<void*, Factory>>)
		: object(other.object),
		  ownsData(other.ownsData),
		  stub(other.stub),
		  id(other.id)
	{
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...)>
    ReturnType
	delegate_base<ReturnType(ParamTypes...), Factory>::method_stub(void* obj, ParamTypes... args)
	{
		T* p = force_cast<T*>(obj);
		return (p->*method)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...) const>
    ReturnType
	delegate_base<ReturnType(ParamTypes...), Factory>::const_method_stub(void* obj, ParamTypes... args)
	{
		const T* p = force_cast<const T*>(obj);
		return (p->*method)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...)>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::method_id(const T& obj)
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...) const>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::method_id(const T& obj)
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*func)(ParamTypes...)>
    ReturnType delegate_base<ReturnType(ParamTypes...), Factory>::function_stub(void*, ParamTypes... args)
	{
		return (func)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*func)(ParamTypes...)>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::function_id()
	{
		return force_cast<size_type>(func);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Func>
    ReturnType
	delegate_base<ReturnType(ParamTypes...), Factory>::function_ptr_stub(void* obj, ParamTypes... args)
		requires(!functor<Func>)
	{
		return (*bit_cast<Func*>(&obj))(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Func>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::function_ptr_id(Func obj)
		requires(!functor<Func>)
	{
		return force_cast<size_type>(obj);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
    ReturnType
	delegate_base<ReturnType(ParamTypes...), Factory>::functor_stub(void* obj, ParamTypes... args)
		requires invocable<Functor, ReturnType(ParamTypes...)>
	{
		Functor* p = force_cast<Functor*>(obj);
		return (p->operator())(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <functor Functor>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::functor_id(const Functor& obj)
		requires invocable<Functor, ReturnType(ParamTypes...)>
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(&Functor::operator()));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
    delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Factory>::create_element(
		allocator_storage allocator, T& instance
	)
	{
		return invocation_element(allocator, &instance, method_stub<T, TMethod>, method_id<T, TMethod>(instance));
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
    delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Factory>::create_element(
		allocator_storage allocator, const T& instance
	)
	{
		return invocation_element(
			allocator, force_cast<void*>(&instance), const_method_stub<T, TMethod>, method_id<T, TMethod>(instance)
		);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
    delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Factory>::create_element(allocator_storage allocator)
	{
		return invocation_element(allocator, nullptr, function_stub<TMethod>, function_id<TMethod>());
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
    delegate_base<ReturnType(ParamTypes...), Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Factory>::create_element(
		allocator_storage allocator, const Functor& instance
	)
	{
		if constexpr (!is_functor_v<Functor>)
		{
			return invocation_element(
				allocator, *bit_cast<void**>(&instance), function_ptr_stub<Functor>,
				function_ptr_id<Functor>(instance)
			);
		}
		else if constexpr (is_empty_v<Functor>)
		{
			return invocation_element(
				allocator, force_cast<void*>(&instance), functor_stub<Functor>, functor_id<Functor>(instance)
			);
		}
		else
		{
			return invocation_element(
				allocator, new Functor(instance), functor_stub<Functor>, functor_id<Functor>(instance),
				[](void* vptr)
			{
				Functor* ptr = static_cast<Functor*>(vptr);
				delete ptr;
			}
			);
		}
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::create_id(T& instance)
	{
		return method_id<T, TMethod>(instance);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::create_id(const T& instance)
	{
		return method_id<T, TMethod>(instance);
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::create_id()
	{
		return function_id<TMethod>();
	}

	template <typename ReturnType, typename... ParamTypes, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
    id_type delegate_base<ReturnType(ParamTypes...), Factory>::create_id(const Functor& instance)
	{
		if constexpr (!is_functor_v<Functor>)
		{
			return function_ptr_id<Functor>(instance);
		}
		else if constexpr (is_empty_v<Functor>)
		{
			return functor_id<Functor>(instance);
		}
		else
		{
			return functor_id<Functor>(instance);
		}
	}
} // namespace rsl
