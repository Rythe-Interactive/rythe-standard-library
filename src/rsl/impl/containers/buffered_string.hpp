#pragma once

#include <string_view>

#include "../util/assert.hpp"
#include "../util/primitives.hpp"

#include "iterators.hpp"

namespace rsl
{
	template <size_type MaxSize, typename CharType = char>
	class buffered_string
	{
	public:
		using value_type = CharType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator_type = value_type*;
		using const_iterator_type = const value_type*;
		using reverse_iterator_type = reverse_iterator<iterator_type>;
		using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;
		using view_type = array_view<value_type>;

	private:
		value_type m_buffer[MaxSize + 1];
		size_type m_size;

		constexpr static size_type validate_size(const size_type newSize) noexcept
		{
			return newSize < MaxSize ? newSize : MaxSize;
		}

	public:
		constexpr buffered_string() noexcept = default;

		constexpr buffered_string(const buffered_string& src) noexcept
			: m_size(validate_size(src.m_size))
		{
			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = src.m_buffer[i];
			}

			m_buffer[m_size] = '\0';
		}

		template <size_type N>
		constexpr buffered_string(const value_type (&str)[N]) noexcept
			: m_size(validate_size(N))
		{
			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = str[i];
			}

			m_buffer[m_size] = '\0';
		}

		constexpr buffered_string(view_type src) noexcept
			: m_size(validate_size(src.size()))
		{
			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = src[i];
			}

			m_buffer[m_size] = '\0';
		}

		constexpr buffered_string& operator=(const buffered_string& src) noexcept
		{
			m_size = validate_size(src.m_size);

			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = src.m_buffer[i];
			}

			m_buffer[m_size] = '\0';

			return *this;
		}

		template <size_type N>
		constexpr buffered_string& operator=(const value_type (&str)[N]) noexcept
		{
			m_size = validate_size(N);

			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = str[i];
			}

			m_buffer[m_size] = '\0';

			return *this;
		}

		constexpr buffered_string& operator=(view_type src) noexcept
		{
			m_size = validate_size(src.size());

			for (size_type i = 0; i < m_size; i++)
			{
				m_buffer[i] = src[i];
			}

			m_buffer[m_size] = '\0';

			return *this;
		}

		constexpr reference at(const size_type i)
		{
			rsl_assert_out_of_range(i < m_size);
			return m_buffer[i];
		}

		constexpr const_reference at(const size_type i) const
		{
			rsl_assert_out_of_range(i < m_size);
			return m_buffer[i];
		}

		constexpr reference raw_at(const size_type i) { return m_buffer[i]; }

		constexpr const_reference raw_at(const size_type i) const { return m_buffer[i]; }

		constexpr reference operator[](const size_type i) { return at(i); }
		constexpr const_reference operator[](const size_type i) const { return at(i); }

		constexpr bool empty() const noexcept { return m_size == 0; }
		constexpr size_type size() const noexcept { return m_size; }
        static constexpr size_type max_size() noexcept { return MaxSize; }

		constexpr void resize(const size_type newSize) noexcept
		{
			size_type oldSize = m_size;
			m_size = validate_size(newSize);

			if (m_size > oldSize)
			{
				for (size_type i = oldSize; i < m_size; i++)
				{
					m_buffer[i] = ' ';
				}
			}

			m_buffer[m_size] = '\0';
		}

		constexpr view_type view() const noexcept { return view_type(m_buffer, m_size); }
		constexpr view_type raw_view() const noexcept { return view_type(m_buffer, MaxSize); }

		constexpr operator const value_type*() const noexcept { return m_buffer; }
		constexpr operator view_type() const noexcept { return view(); }

		constexpr iterator_type begin() noexcept { return m_buffer; }
		constexpr const_iterator_type begin() const noexcept { return m_buffer; }
		constexpr const_iterator_type cbegin() const noexcept { return m_buffer; }

		constexpr iterator_type end() noexcept { return &m_buffer[m_size]; }
		constexpr const_iterator_type end() const noexcept { return &m_buffer[m_size]; }
		constexpr const_iterator_type cend() const noexcept { return &m_buffer[m_size]; }

		constexpr reverse_iterator_type rend() noexcept { return reverse_iterator_type(begin()); }
		constexpr const_reverse_iterator_type rend() const noexcept { return crend(); }
		constexpr const_reverse_iterator_type crend() const noexcept { return const_reverse_iterator_type(cbegin()); }

		constexpr reverse_iterator_type rbegin() noexcept { return reverse_iterator_type(end()); }
		constexpr const_reverse_iterator_type rbegin() const noexcept { return crbegin(); }
		constexpr const_reverse_iterator_type crbegin() const noexcept { return const_reverse_iterator_type(cend()); }
	};

	template <size_type MaxSize, typename CharType>
	std::ostream& operator<<(std::ostream& os, const buffered_string<MaxSize, CharType>& str)
	{
		os << str.view();
		return os;
	}

	template <size_type MaxSize, typename CharType>
	buffered_string(const CharType (&)[MaxSize]) -> buffered_string<MaxSize, CharType>;

	using b_string64 = buffered_string<64>;
	using b_string128 = buffered_string<128>;
	using b_string256 = buffered_string<256>;
	using b_string512 = buffered_string<512>;
} // namespace rsl
