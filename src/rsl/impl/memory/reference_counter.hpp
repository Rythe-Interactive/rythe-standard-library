#pragma once

#include "../util/primitives.hpp"

#include "memory_resource_base.hpp"

namespace rsl
{
	template <typename T>
	concept reference_counted = requires(T& val) {
		{ val.borrow() };
		{ val.release() };
		{ val.count() } -> convertible_to<size_type>;
		{ val.is_occupied() } -> convertible_to<bool>;
		{ val.is_free() } -> convertible_to<bool>;
		{ val.reset() };
	};

	class manual_reference_counter
	{
	public:
		virtual ~manual_reference_counter() = default;
		[[rythe_always_inline]] constexpr size_type borrow() noexcept;
		[[rythe_always_inline]] constexpr void release() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type count() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_occupied() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_free() const noexcept;
		[[rythe_always_inline]] void reset() noexcept;

		virtual void on_reset() noexcept {};

	private:
		size_type m_count = 0;
	};

	template <typename T>
	struct reference_counted_payload final : public manual_reference_counter
	{
		T value;
	};

	struct arm_signal_type
	{
	};

	constexpr arm_signal_type arm_signal;

	template <
		reference_counted Counter = manual_reference_counter, allocator_type Alloc = default_allocator,
		factory_type Factory = default_factory<Counter>>
	class basic_reference_counter : public internal::select_memory_resource<Counter, Alloc, Factory>::type
	{
	protected:
		constexpr static bool untyped_memory_resource =
			internal::select_memory_resource<Counter, Alloc, Factory>::is_untyped;
		using mem_rsc = internal::select_memory_resource<Counter, Alloc, Factory>::type;
		using allocator_storage_type = mem_rsc::allocator_storage_type;
		using allocator_t = mem_rsc::allocator_t;
		using factory_storage_type = mem_rsc::factory_storage_type;
		using factory_t = mem_rsc::factory_t;

	public:
		[[rythe_always_inline]] constexpr basic_reference_counter()
			noexcept(is_nothrow_constructible_v<mem_rsc>) = default;
		[[rythe_always_inline]] constexpr basic_reference_counter(arm_signal_type) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(const basic_reference_counter& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter(basic_reference_counter&& other) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter& operator=(const basic_reference_counter& other
		) noexcept;
		[[rythe_always_inline]] constexpr basic_reference_counter& operator=(basic_reference_counter&& other) noexcept;
		[[rythe_always_inline]] constexpr ~basic_reference_counter() noexcept;

		[[rythe_always_inline]] explicit basic_reference_counter(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&>);

		[[rythe_always_inline]] explicit basic_reference_counter(const factory_storage_type& factoryStorage)
			noexcept(is_nothrow_constructible_v<mem_rsc, const factory_storage_type&>);

		[[rythe_always_inline]] basic_reference_counter(
			const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
		) noexcept(is_nothrow_constructible_v<mem_rsc, const allocator_storage_type&, const factory_storage_type&>);

		[[rythe_always_inline]] constexpr void arm() noexcept;
		[[rythe_always_inline]] void disarm() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_armed() const noexcept;

		[[rythe_always_inline]] constexpr size_type borrow() noexcept;
		[[rythe_always_inline]] constexpr void release() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type count() const noexcept;

		// These versions of occupied and free don't count the reference you call this on.
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_occupied() const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool is_free() const noexcept;

	protected:
		virtual void on_disarm() noexcept {};
		[[rythe_always_inline]] constexpr void arm(Counter* ptr) noexcept;
	};

	using reference_counter = basic_reference_counter<>;
} // namespace rsl

#include "reference_counter.inl"
