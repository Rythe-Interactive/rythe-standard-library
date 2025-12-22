#pragma once

#include "../util/concepts.hpp"

#include "heap_allocator.hpp"

//TODO(Glyn): Having these be template parameters to containers is not worth the saving of the pointer sometimes.

namespace rsl
{
	template <typename T>
	concept noexcept_allocator_type = requires(T alloc, void* ptr, size_type n) {
		{ alloc.allocate(n) } noexcept -> convertible_to<void*>;
		{ alloc.allocate(n, n) } noexcept -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n) } noexcept -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n, n) } noexcept -> convertible_to<void*>;
		{ alloc.deallocate(ptr, n) } noexcept;
		{ alloc.deallocate(ptr, n, n) } noexcept;
		{ alloc.is_valid() } noexcept -> convertible_to<bool>;
	};

	template <typename T>
	concept allocator_type = requires(T alloc, void* ptr, size_type n) {
		{ alloc.allocate(n) } -> convertible_to<void*>;
		{ alloc.allocate(n, n) } -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n) } -> convertible_to<void*>;
		{ alloc.reallocate(ptr, n, n, n) } -> convertible_to<void*>;
		{ alloc.deallocate(ptr, n) };
		{ alloc.deallocate(ptr, n, n) };
		{ alloc.is_valid() } noexcept -> convertible_to<bool>;
	};

#if !defined(RSL_DEFAULT_ALLOCATOR_OVERRIDE)
	using default_allocator = heap_allocator;
#else
	using default_allocator = RSL_DEFAULT_ALLOCATOR_OVERRIDE;
#endif

	class polymorphic_allocator
	{
	public:
		virtual bool is_valid() const noexcept { return true; } //NOLINT

		virtual ~polymorphic_allocator() = default;
		[[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] virtual void* allocate(size_type size, size_type alignment) noexcept = 0;

		[[nodiscard]] [[rythe_allocating]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept = 0;
		[[nodiscard]] [[rythe_allocating]] virtual void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept = 0;

		virtual void deallocate(void* ptr, size_type size) noexcept = 0;
		virtual void deallocate(void* ptr, size_type size, size_type alignment) noexcept = 0;

		template<not_same_as<void> T>
		[[nodiscard]] [[rythe_allocating]] T* allocate() noexcept { return static_cast<T*>(allocate(sizeof(T))); }
		template<not_same_as<void> T>
		[[nodiscard]] [[rythe_allocating]] T* allocate(const size_type alignment) noexcept { return static_cast<T*>(allocate(sizeof(T), alignment)); }

		template<not_same_as<void> T>
		void deallocate(T* ptr) noexcept { deallocate(static_cast<void*>(ptr), sizeof(T)); }
		template<not_same_as<void> T>
		void deallocate(T* ptr, const size_type alignment) noexcept { deallocate(ptr, sizeof(T), alignment); }
	};

	using pmu_allocator = polymorphic_allocator;

	template <allocator_type Alloc = default_allocator>
	class polymorphic_allocator_mixin final : public polymorphic_allocator
	{
	public:
		using value_type = void;

		Alloc impl;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void* allocate(size_type size) noexcept override;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		allocate(size_type size, size_type alignment) noexcept override;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept override;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept override;

		[[rythe_always_inline]] void deallocate(void* ptr, size_type size) noexcept override;
		[[rythe_always_inline]] void deallocate(void* ptr, size_type size, size_type alignment) noexcept override;
	};

	using default_polymorphic_allocator = polymorphic_allocator_mixin<default_allocator>;
	using default_pmu_allocator = default_polymorphic_allocator;

	class polymorphic_allocator_ptr_wrapper
	{
	public:
		using value_type = void;

		pmu_allocator* allocatorPtr;

		constexpr polymorphic_allocator_ptr_wrapper() noexcept = default;
		constexpr polymorphic_allocator_ptr_wrapper(pmu_allocator* ptr) noexcept
			: allocatorPtr(ptr)
		{
		}

		[[rythe_always_inline]] constexpr bool is_valid() const noexcept { return allocatorPtr; }

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void* allocate(size_type size) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		allocate(size_type size, size_type alignment) noexcept;

		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldSize, size_type newSize) noexcept;
		[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] constexpr void*
		reallocate(void* ptr, size_type oldSize, size_type newSize, size_type alignment) noexcept;

		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type size) noexcept;
		[[rythe_always_inline]] constexpr void deallocate(void* ptr, size_type size, size_type alignment) noexcept;
	};

	using pmu_alloc_ptr_wrapper = polymorphic_allocator_ptr_wrapper;
} // namespace rsl

#include "allocator.inl"
