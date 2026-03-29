#pragma once

#include "typed_allocator.hpp"

namespace rsl
{
	struct allocator_context
	{
		static default_allocator defaultGlobalAllocator;
		static pointer<memory_allocator> globalAllocator;
		static default_allocator defaultThreadSpecificAllocator;
		static thread_local pointer<memory_allocator> threadSpecificAllocator;
	};

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	allocate(memory_allocator& alloc, size_type count = 1, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...>);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	allocate(memory_allocator& alloc, size_type count, size_type alignment, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...>);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T*
	reallocate(memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, Args&&... args)
		noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable);

	template <typename T, typename... Args>
	[[nodiscard]] [[rythe_allocating]] [[rythe_always_inline]] T* reallocate(
		memory_allocator& alloc, T* ptr, size_type oldCount, size_type newCount, size_type alignment, Args&&... args
	) noexcept(default_factory<T>::template noexcept_constructable<Args...> && default_factory<T>::noexcept_moveable);

	template <typename T>
	[[rythe_always_inline]] void deallocate(memory_allocator& alloc, T* ptr, size_type count = 1) noexcept;

	template <typename T>
	[[rythe_always_inline]] void
	deallocate(memory_allocator& alloc, T* ptr, size_type count, size_type alignment) noexcept;
} // namespace rsl

#include "allocator_context.inl"
