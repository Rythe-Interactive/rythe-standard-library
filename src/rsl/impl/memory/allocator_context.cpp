#include "allocator_context.hpp"

namespace rsl
{
	default_allocator allocator_context::defaultGlobalAllocator{};
	pointer<memory_allocator> allocator_context::globalAllocator{&defaultGlobalAllocator};
	default_allocator allocator_context::defaultThreadSpecificAllocator{};
	thread_local pointer<memory_allocator> allocator_context::threadSpecificAllocator{&defaultThreadSpecificAllocator};
} // namespace rsl
