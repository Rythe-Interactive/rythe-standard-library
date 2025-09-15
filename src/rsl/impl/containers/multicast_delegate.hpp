#pragma once

#include "delegate.hpp"
#include "array.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T, allocator_type Alloc>
		struct multicast_delegate_invocation_result
		{
			using type = dynamic_array<T, Alloc>;
		};

		template <allocator_type Alloc>
		struct multicast_delegate_invocation_result<void, Alloc>
		{
			using type = void;
		};
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	class multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory> final :
		private delegate_base<ReturnType(ParamTypes...), Alloc, Factory>
	{
		using base = delegate_base<ReturnType(ParamTypes...), Alloc, Factory>;

	public:
		using return_type = ReturnType;
		using param_types = type_sequence<ParamTypes...>;
		using invocation_element = typename base::invocation_element;

		using value_type = delegate<ReturnType(ParamTypes...), Alloc, Factory>;

		using invocation_container = dynamic_array<value_type, Alloc>;

		using iterator_type = typename invocation_container::iterator_type;
		using const_iterator_type = typename invocation_container::const_iterator_type;
		using reverse_iterator_type = typename invocation_container::reverse_iterator_type;
		using const_reverse_iterator_type = typename invocation_container::const_reverse_iterator_type;
		using view_type = typename invocation_container::view_type;
		using const_view_type = typename invocation_container::const_view_type;
		using allocator_storage_type = typename invocation_container::allocator_storage_type;
		using allocator_t = typename invocation_container::allocator_t;
		using factory_storage_type = typename invocation_container::factory_storage_type;
		using factory_t = typename invocation_container::factory_t;

		[[rythe_always_inline]] constexpr multicast_delegate() noexcept = default;

		[[rythe_always_inline]] constexpr multicast_delegate(const value_type& val) noexcept;
		[[rythe_always_inline]] explicit constexpr multicast_delegate(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<invocation_container, const allocator_storage_type&>);
		[[rythe_always_inline]] explicit constexpr multicast_delegate(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<invocation_container, const factory_storage_type&>);
		[[rythe_always_inline]] constexpr multicast_delegate(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		)
			noexcept(is_nothrow_constructible_v<
					 invocation_container, const allocator_storage_type&, const factory_storage_type&>);

		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
		[[rythe_always_inline]] constexpr void clear() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
		[[rythe_always_inline]] constexpr void reserve(size_type newCap) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[rythe_always_inline]] constexpr void set_factory(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_copy_assignable_v<factory_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_t& get_factory() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_t& get_factory() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_storage_type& get_allocator_storage() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_storage_type&
		get_allocator_storage() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr factory_storage_type& get_factory_storage() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const factory_storage_type&
		get_factory_storage() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr auto begin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto cbegin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto begin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rbegin() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto crbegin() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rbegin() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr auto end() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto cend() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto end() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rend() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto crend() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rend() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr iterator_type iterator_at(size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type iterator_at(size_type i) const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& at(size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& at(size_type i) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator[](size_type i) noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator[](size_type i) const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr view_type view() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const_view_type view() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator view_type() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr operator const_view_type() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& front() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& front() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& back() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& back() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(nullptr_type) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(nullptr_type) const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator==(const multicast_delegate&) const noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator!=(const multicast_delegate&) const noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const value_type& other) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const value_type& other) const noexcept;

		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const value_type& e);
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(value_type&& e);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back();

		template <functor Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const Functor& instance)
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const value_type& another);
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(value_type&& another);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const T& instance);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const Functor& instance);

		[[rythe_always_inline]] constexpr size_type erase(size_type pos);
		[[rythe_always_inline]] constexpr size_type erase(size_type pos, size_type count);

		[[rythe_always_inline]] constexpr void pop_back();

		[[rythe_always_inline]] constexpr multicast_delegate& remove(const value_type& del);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& remove();

		template <functor Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(const Functor& instance)
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const value_type& del) const noexcept;

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(T& instance) const noexcept;

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const T& instance) const noexcept;

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains() const noexcept;

		template <functor Functor>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const Functor& instance) const noexcept
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const value_type& another);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const T& instance);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const Functor& instance);

		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const multicast_delegate&) = default;
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(multicast_delegate&&) = default;
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const value_type& del);
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(value_type&& del);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const T& instance);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const Functor& instance);

		[[rythe_always_inline]] constexpr multicast_delegate& assign(const value_type& del);
		[[rythe_always_inline]] constexpr multicast_delegate& assign(value_type&& del);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(const T& instance);

		template <functor Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(const Functor& instance)
			requires invocable<Functor, ReturnType(ParamTypes...)>;

		template <input_iterator InputIt>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(InputIt first, InputIt last);

	private:
		template <typename T>
		using invocation_result_t = typename internal::multicast_delegate_invocation_result<T, allocator_t>::type;

	public:
		[[rythe_always_inline]] constexpr auto operator()(ParamTypes... args) const;

		[[rythe_always_inline]] constexpr auto invoke(ParamTypes... args) const -> invocation_result_t<ReturnType>;

	private:
		[[rythe_always_inline]] constexpr multicast_delegate(invocation_container&& e);

		[[rythe_always_inline]] constexpr multicast_delegate& remove(id_type id);

		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(id_type id) const noexcept;

		[[rythe_always_inline]] constexpr multicast_delegate& push_back(invocation_element&& elem);

		invocation_container m_invocationList;
	};

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	multicast_delegate(const delegate<ReturnType(ParamTypes...), Alloc, Factory>&)
		-> multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>;
} // namespace rsl

#include "multicast_delegate.inl"
