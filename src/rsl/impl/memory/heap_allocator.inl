#pragma once

namespace rsl
{
    constexpr void* heap_allocator::allocate(const size_type size) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			return new byte[size];
		}
		else
		{
			return ::operator new(size, std::nothrow);
		}
	}

    constexpr void* heap_allocator::allocate(const size_type size, const size_type alignment) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			return new byte[size];
		}
		else
		{
			return ::operator new(size, std::align_val_t{alignment}, std::nothrow);
		}
	}

    constexpr void* heap_allocator::reallocate(void* ptr, const size_type oldSize, const size_type newSize) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = new byte[newSize];
				constexpr_memcpy(mem, ptr, oldSize < newSize ? oldSize : newSize);
			}

			delete[] bit_cast<byte*>(ptr);

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newSize != 0) [[likely]]
			{
				mem = ::operator new(newSize, std::nothrow);
				if (mem) [[likely]]
				{
					constexpr_memcpy(mem, ptr, min(oldSize, newSize));
				}
			}

			::operator delete(ptr, oldSize);

			return mem;
		}
	}

    constexpr void*
	heap_allocator::reallocate(void* ptr, const size_type oldSize, const size_type newSize, const size_type alignment) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			void* mem = nullptr;

			if (newSize != 0)
			{
				mem = new byte[newSize];
				constexpr_memcpy(mem, ptr, oldSize < newSize ? oldSize : newSize);
			}

			delete[] bit_cast<byte*>(ptr);

			return mem;
		}
		else
		{
			void* mem = nullptr;

			if (newSize != 0) [[likely]]
			{
				mem = ::operator new(newSize, std::align_val_t{alignment}, std::nothrow);
				if (mem) [[likely]]
				{
					memcpy(mem, ptr, min(oldSize, newSize));
				}
			}

			::operator delete(ptr, oldSize, std::align_val_t{alignment});

			return mem;
		}
	}

    constexpr void heap_allocator::deallocate(void* ptr, const size_type size) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			delete[] bit_cast<byte*>(ptr);
		}
		else
		{
			::operator delete(ptr, size);
		}
	}

    constexpr void heap_allocator::deallocate(void* ptr, const size_type size, const size_type alignment) noexcept
	{
		if (rsl::is_constant_evaluated())
		{
			delete[] bit_cast<byte*>(ptr);
		}
		else
		{
			::operator delete(ptr, size, std::align_val_t{alignment});
		}
	}
} // namespace rsl
