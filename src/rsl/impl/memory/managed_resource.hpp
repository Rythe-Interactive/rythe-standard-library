#pragma once
#include "../containers/optional.hpp"

#include "reference_counter.hpp"

namespace rsl
{
	template <typename T, untyped_factory_type Factory>
	class managed_resource;

	namespace internal
	{
		struct managed_payload_base : public manual_reference_counter
		{
			virtual void destroy(void*) noexcept { rsl_assert_unreachable(); }
		};

		template <typename Deleter, typename T>
		concept managed_deleter_type = requires(Deleter del, T& val) { del(val); };

		template <typename T, managed_deleter_type<T> Deleter>
		struct managed_payload final : public managed_payload_base
		{
			Deleter deleter;

			void destroy(void* value) noexcept override;
			virtual void on_reset() noexcept override { rsl_assert_invalid_object(!deleter); }
		};
	} // namespace internal

	template <typename T, untyped_factory_type Factory = type_erased_factory>
	class managed_resource : public basic_reference_counter<internal::managed_payload_base, Factory>
	{
	public:
		using ref_counter = basic_reference_counter<internal::managed_payload_base, Factory>;
		using mem_rsc = typename ref_counter::mem_rsc;

		using factory_storage_type = typename ref_counter::factory_storage_type;
		using factory_t = typename ref_counter::factory_t;

		[[rythe_always_inline]] constexpr managed_resource(nullptr_type)
			noexcept(is_nothrow_constructible_v<ref_counter>);

		[[rythe_always_inline]] explicit managed_resource(pointer<memory_allocator> allocator)
			noexcept(is_nothrow_constructible_v<ref_counter, pointer<memory_allocator>>);

		template <internal::managed_deleter_type<T> Deleter, typename... Args>
		[[rythe_always_inline]] constexpr explicit managed_resource(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<ref_counter> && is_nothrow_constructible_v<T, Args...>);

		template <internal::managed_deleter_type<T> Deleter, typename... Args>
		[[rythe_always_inline]] managed_resource(
			pointer<memory_allocator> allocator, Deleter deleter, Args&&... args
		) noexcept(is_nothrow_constructible_v<ref_counter, pointer<memory_allocator>> && is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr managed_resource() noexcept = default;

		[[rythe_always_inline]] constexpr ~managed_resource() noexcept;

		template <internal::managed_deleter_type<T> Deleter, typename... Args>
		[[rythe_always_inline]] constexpr void arm(Deleter deleter, Args&&... args)
			noexcept(is_nothrow_constructible_v<T, Args...>);

		[[rythe_always_inline]] constexpr T* get() noexcept { return &*m_value; }
		[[rythe_always_inline]] constexpr const T* get() const noexcept { return &*m_value; }

		[[rythe_always_inline]] constexpr T& operator*() noexcept { return *m_value; }
		[[rythe_always_inline]] constexpr const T& operator*() const noexcept { return *m_value; }
		[[rythe_always_inline]] constexpr T* operator->() noexcept { return &*m_value; }
		[[rythe_always_inline]] constexpr const T* operator->() const noexcept { return &*m_value; }

    protected:
		constexpr void disarm_impl() noexcept;
		virtual void on_disarm() noexcept;

	private:
		optional<T> m_value;
	};
} // namespace rsl

#include "managed_resource.inl"
