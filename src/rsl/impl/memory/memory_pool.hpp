#pragma once

#include "allocator_storage.hpp"

namespace rsl
{
    template <typename T, allocator_type Alloc = default_allocator, size_type MinBlockSize = 4, size_type MaxBlockSize = 16384>
        requires(MinBlockSize >= 1 && MaxBlockSize >= MinBlockSize)
    class memory_pool
    {
    public:
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;

        memory_pool() noexcept = default;

        memory_pool(const memory_pool&) noexcept
            : m_head(nullptr),
              m_freeList(nullptr) {}

        memory_pool(memory_pool&& other) noexcept
            : m_head(other.m_head),
              m_freeList(other.m_freeList)
        {
            other.m_freeList = nullptr;
            other.m_head = nullptr;
        }

        memory_pool& operator=(memory_pool&& other) noexcept
        {
            reset();
            m_head = other.m_head;
            m_freeList = other.m_freeList;
            other.m_freeList = nullptr;
            other.m_head = nullptr;
            return *this;
        }

        memory_pool& operator=(const memory_pool&) noexcept { return *this; }

        ~memory_pool() noexcept { reset(); }

        void reset() noexcept
        {
            while (m_freeList)
            {
                memory_block* block = m_freeList;
                m_freeList = block->next;
                m_alloc->deallocate(block, block->blockSize, elementAlignment);
            }
            m_head = nullptr;
        }

        [[nodiscard]] T* allocate()
        {
            element_node* node = m_head;
            if (!node)
            {
                node = allocate_block();
            }

            m_head = node->next;
            return bit_cast<T*>(node);
        }

        void deallocate(T* ptr) noexcept
        {
            rsl_assert_frequent(ptr);
            element_node* node = bit_cast<element_node*>(ptr);
            node->next = m_head;
            m_head = node;
        }

        // Adds an already allocated block of memory to the pool.
        // Make sure memory block is compatible with the allocator used by this pool, and the alignment is correct!
        [[nodiscard]] bool add_block_to_pool(void* ptr, const size_type numBytes) noexcept
        {
            if (numBytes < minimumExternalBlockSize)
            {
                return false;
            }

            add_block_unsafe(ptr, numBytes);
            return true;
        }

        // Very slow, use for debugging purposes only. Do not use in hot paths.
        [[nodiscard]] size_type capacity()
        {
            memory_block* block = m_freeList;
            size_type capacity = 0;

            while (block)
            {
                capacity += get_element_count(block->blockSize);
                block = block->next;
            }

            return capacity;
        }

        // Very slow, use for debugging purposes only. Do not use in hot paths.
        [[nodiscard]] size_type size()
        {
            size_type usedCapacity = capacity();

            element_node* node = m_head;
            while (node)
            {
                node = node->next;
                --usedCapacity;
            }

            return usedCapacity;
        }

        void reserve(const size_type newCapacity)
        {
            size_type currentCapacity = capacity();

            size_type blockSize = next_block_size();
            while (currentCapacity < newCapacity)
            {
                allocate_block();
                currentCapacity += blockSize;
                blockSize = next_block_size();
            }
        }

        static constexpr size_type elementAlignment =
                alignment_of<T>::value > alignment_of<T*>::value ? alignment_of<T>::value : alignment_of<T*>::value;
        static constexpr size_type elementSize = (((sizeof(T) - 1) / elementAlignment) + 1) * elementAlignment;

    private:
        struct alignas(elementAlignment) element_node
        {
            element_node* next;
        };

        struct alignas(elementAlignment) memory_block
        {
            memory_block* next;
            size_type blockSize;
        };

    public:
        [[nodiscard]] static constexpr size_type get_element_count(const size_type blockSize) noexcept
        {
            return (blockSize - blockHeaderSize) / elementSize;
        }

        [[nodiscard]] static constexpr size_type get_block_size(const size_type elementCount) noexcept
        {
            return blockHeaderSize + elementSize * elementCount;
        }

        static constexpr size_type blockHeaderSize =
                (((sizeof(memory_block) - 1) / elementAlignment) + 1) * elementAlignment;

        static constexpr size_type minimumExternalBlockSize = get_block_size(1);
        static constexpr size_type minimumBlockSize = get_block_size(MinBlockSize);
        static constexpr size_type maximumBlockSize = get_block_size(MaxBlockSize);

    private:
        [[nodiscard]] size_type next_block_size() const noexcept
        {
            memory_block* block = m_freeList;
            size_type elementCount = MinBlockSize;

            while (elementCount * 2 <= MaxBlockSize && block)
            {
                block = block->next;
                elementCount *= 2;
            }

            return get_block_size(elementCount);
        }

        void add_block_unsafe(void* ptr, size_type blockSize) noexcept
        {
            const size_type elementCount = get_element_count(blockSize);

            memory_block* block = bit_cast<memory_block*>(ptr);
            block->next = m_freeList;
            block->blockSize = blockSize;
            m_freeList = block;

            // create linked list for newly allocated data
            element_node* head = bit_cast<element_node*>(block + 1);

            for (size_type i = 0; i < elementCount; ++i)
            {
                (head + i)->next = head + i + 1;
            }

            (head + (elementCount - 1))->next = m_head;
            m_head = head;
        }

        [[rythe_never_inline]] element_node* allocate_block()
        {
            const size_type blockSize = next_block_size();

            add_block_unsafe(m_alloc->allocate(blockSize, elementAlignment), blockSize);
            return m_head;
        }

        allocator_storage_type m_alloc;
        element_node* m_head = nullptr;
        memory_block* m_freeList = nullptr;
    };
} // namespace rsl

#include "memory_pool.inl"
