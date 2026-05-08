#include "allocator_context.hpp"

namespace rsl
{
    heap_allocator allocator_context::defaultGlobalAllocator{};
    allocator_storage allocator_context::globalAllocator{&defaultGlobalAllocator};
    heap_allocator allocator_context::defaultThreadSpecificAllocator{};
    thread_local allocator_storage allocator_context::threadSpecificAllocator{&defaultThreadSpecificAllocator};
} // namespace rsl
