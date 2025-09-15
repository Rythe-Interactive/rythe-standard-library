#pragma once

#include "../defines.hpp"
#include "../util/concepts.hpp"
#include "../util/utilities.hpp"

#include "../memory/managed_resource.hpp"

namespace rsl
{

	template <typename T, allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class delegate_base;

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	class delegate_base<ReturnType(ParamTypes...), Alloc, Factory>
	{
	protected:
		using stub_type = ReturnType (*)(void*, ParamTypes...);
		using deleter_type = void (*)(void*);
		using allocator_storage_type =
			typename managed_resource<void*, Alloc, Factory>::mem_rsc::allocator_storage_type;
		using allocator_t = typename managed_resource<void*, Alloc, Factory>::mem_rsc::allocator_t;
		using factory_storage_type = typename managed_resource<void*, Alloc, Factory>::mem_rsc::factory_storage_type;
		using factory_t = typename managed_resource<void*, Alloc, Factory>::mem_rsc::factory_t;
		using typed_alloc_type = typename managed_resource<void*, Alloc, Factory>::mem_rsc::typed_alloc_type;

		constexpr static deleter_type default_deleter = []([[maybe_unused]] void*) {};

		struct invocation_element
		{
			using return_type = ReturnType;
			using param_types = type_sequence<ParamTypes...>;
			constexpr invocation_element() noexcept = default;

			constexpr invocation_element(
				const allocator_storage_type& allocStorage, void* object, stub_type stub, id_type id,
				deleter_type deleter = nullptr
			)
				noexcept(is_nothrow_constructible_v<
						 managed_resource<void*, Alloc, Factory>, const allocator_storage_type&, deleter_type, void*>);

			constexpr invocation_element(const invocation_element& other)
				noexcept(is_nothrow_copy_constructible_v<managed_resource<void*, Alloc, Factory>>);

			constexpr bool operator==(id_type otherId) const noexcept { return id == otherId; }
			constexpr bool operator!=(id_type otherId) const noexcept { return id != otherId; }

			constexpr bool operator==(const invocation_element& other) const noexcept { return id == other.id; }
			constexpr bool operator!=(const invocation_element& other) const noexcept { return id != other.id; }

			managed_resource<void*, Alloc, Factory> object = nullptr;
			bool ownsData = false;
			stub_type stub = nullptr;
			id_type id = invalid_id;
		};

		template <typename T, ReturnType (T::*method)(ParamTypes...)>
		static ReturnType method_stub(void* obj, ParamTypes... args);

		template <typename T, ReturnType (T::*method)(ParamTypes...) const>
		static ReturnType const_method_stub(void* obj, ParamTypes... args);

		template <typename T, ReturnType (T::*method)(ParamTypes...)>
		[[rythe_always_inline]] static id_type method_id(const T& obj);

		template <typename T, ReturnType (T::*method)(ParamTypes...) const>
		[[rythe_always_inline]] static id_type method_id(const T& obj);

		template <ReturnType (*func)(ParamTypes...)>
		static ReturnType function_stub(void*, ParamTypes... args);

		template <ReturnType (*func)(ParamTypes...)>
		[[rythe_always_inline]] static id_type function_id();

		template <invocable<ReturnType(ParamTypes...)> Func>
		static ReturnType function_ptr_stub(void* obj, ParamTypes... args)
			requires(!functor<Func>);

		template <invocable<ReturnType(ParamTypes...)> Func>
		[[rythe_always_inline]] static id_type function_ptr_id(Func obj)
			requires(!functor<Func>);

		template <functor Functor>
		static ReturnType functor_stub(void* obj, ParamTypes... args)
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		template <functor Functor>
		[[rythe_always_inline]] static id_type functor_id(const Functor& obj)
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static invocation_element
		create_element(const allocator_storage_type& allocStorage, T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] static invocation_element
		create_element(const allocator_storage_type& allocStorage, const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static invocation_element create_element(const allocator_storage_type& allocStorage);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] static invocation_element
		create_element(const allocator_storage_type& allocStorage, const Functor& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static id_type create_id(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] static id_type create_id(const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static id_type create_id();

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] static id_type create_id(const Functor& instance);
	};
} // namespace rsl

#include "delegate_base.inl"
