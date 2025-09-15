#pragma once
#include "../containers/array.hpp"

// TODO(Glyn): static and hybrid

namespace rsl
{
    class index_allocator
    {
    public:
        [[rythe_always_inline]] void reserve_indices(size_type capacity, size_type offset = 0ull);

        [[nodiscard]] [[rythe_always_inline]] index_type allocate_index();
        [[rythe_always_inline]] void free_index(index_type index);

    private:
        dynamic_array<index_type> m_freeIndices;
        index_type m_nextIndex = 0ull;
    };
}

#include "index_allocator.inl"
