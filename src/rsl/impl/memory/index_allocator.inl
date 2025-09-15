#pragma once

namespace rsl
{
    inline void index_allocator::reserve_indices(const size_type capacity, const size_type offset)
    {
        for (index_type i = capacity; i != 0ull; --i)
        {
            m_freeIndices.push_back(i - 1ull + offset);
        }

        const index_type nextIndex = capacity + offset;
        if (nextIndex > m_nextIndex)
        {
            m_nextIndex = nextIndex;
        }
    }

    inline index_type index_allocator::allocate_index()
    {
        if (m_freeIndices.empty())
        {
            return m_nextIndex++;
        }

        const index_type result = m_freeIndices.back();
        m_freeIndices.pop_back();
        return result;
    }

    inline void index_allocator::free_index(const index_type index)
    {
        m_freeIndices.push_back(index);
    }
}
