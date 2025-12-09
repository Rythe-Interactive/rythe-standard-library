#pragma once
#include "unique_resource.hpp"

namespace rsl
{
	template <typename T, internal::unique_deleter_type<T> Deleter>
	void internal::unique_payload<T, Deleter>::destroy(void* value) noexcept
	{
		if (deleter)
		{
			deleter(*static_cast<T*>(value));
		}
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc>)
		: mem_rsc(), m_value(factoryStorage) {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(const allocator_storage_type& allocStorage,
	                                                              const factory_storage_type& factoryStorage)
		noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>)
		: mem_rsc(allocStorage), m_value(factoryStorage) {}

    template <typename T, allocator_type Alloc, typed_factory_type Factory>
    template <typename OtherT, allocator_type OtherAlloc, typed_factory_type OtherFactory>
    constexpr unique_resource<T, Alloc, Factory>::unique_resource(
            internal::alloc_and_factory_only_signal_type,
            const unique_resource<OtherT, OtherAlloc, OtherFactory>& other
            ) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>)
		: mem_rsc(internal::alloc_and_factory_only_signal, other), m_value(other.get_factory_storage()) {}

    template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <internal::unique_deleter_type<T> Deleter, typename... Args>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(const allocator_storage_type& allocStorage, Deleter deleter,
	                                                    Args&&... args)
		noexcept( is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&> && is_nothrow_constructible_v<
			          T, Args...>)
		: mem_rsc(allocStorage)
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(nullptr_type)
		noexcept(is_nothrow_constructible_v<mem_rsc>)
		: mem_rsc() {}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <internal::unique_deleter_type<T> Deleter, typename... Args>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<mem_rsc> && is_nothrow_constructible_v<T, Args...>)
		: mem_rsc()
	{
		arm(deleter, forward<Args>(args)...);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::unique_resource(unique_resource&& other) noexcept
		: mem_rsc(other.mem_rsc::get_allocator_storage(), other.mem_rsc::get_factory_storage()), m_value(move(other.m_value))
	{
		mem_rsc::set_ptr(other.mem_rsc::get_ptr());
		other.mem_rsc::set_ptr(nullptr);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>::~unique_resource() noexcept
	{
		disarm();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr unique_resource<T, Alloc, Factory>& unique_resource<T, Alloc, Factory>::operator=(unique_resource&& other) noexcept
	{
		disarm();

		mem_rsc::set_allocator(other.mem_rsc::get_allocator_storage());
		mem_rsc::set_factory(other.mem_rsc::get_factory_storage());
		m_value = move(other.m_value);

		mem_rsc::set_ptr(other.mem_rsc::get_ptr());
		other.mem_rsc::set_ptr(nullptr);

		return *this;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr void unique_resource<T, Alloc, Factory>::set_factory(const factory_storage_type& factoryStorage)
	noexcept(is_nothrow_copy_assignable_v<factory_storage_type>)
	{
		m_value.set_factory(factoryStorage);
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr Factory& unique_resource<T, Alloc, Factory>::get_factory() noexcept { return m_value.get_factory(); }

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr const Factory& unique_resource<T, Alloc, Factory>::get_factory() const noexcept
	{
		return m_value.get_factory();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr factory_storage<Factory>& unique_resource<T, Alloc, Factory>::get_factory_storage() noexcept
	{
		return m_value.get_factory_storage();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr const factory_storage<Factory>& unique_resource<T, Alloc, Factory>::get_factory_storage() const noexcept
	{
		return m_value.get_factory_storage();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	template <internal::unique_deleter_type<T> Deleter, typename... Args>
	constexpr void unique_resource<T, Alloc, Factory>::arm(Deleter deleter, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		m_value.emplace(rsl::forward<Args>(args)...);

		mem_rsc::set_factory(type_erased_factory(construct_type_signal<internal::unique_payload<T, Deleter>>));
	    static_assert(sizeof(internal::unique_payload<T, Deleter>) == 16);
		mem_rsc::allocate_and_construct(1);
		bit_cast<internal::unique_payload<T, Deleter>*>(mem_rsc::get_ptr())->deleter = deleter;
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr bool unique_resource<T, Alloc, Factory>::is_armed() const noexcept
	{
		return mem_rsc::get_ptr();
	}

	template <typename T, allocator_type Alloc, typed_factory_type Factory>
	constexpr void unique_resource<T, Alloc, Factory>::disarm() noexcept
	{
		if (!is_armed())
		{
			return;
		}

        mem_rsc::get_ptr()->destroy(get());

		m_value.reset();
		mem_rsc::destroy_and_deallocate();
		mem_rsc::set_ptr(nullptr);
	}
}
