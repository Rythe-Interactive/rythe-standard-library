#pragma once

#include "../util/common.hpp"
#include "../util/primitives.hpp"

#include "views.hpp"

namespace rsl
{
	template <size_type N>
	struct constexpr_string
	{
		char buffer[N];

		consteval constexpr_string() noexcept = default;

		consteval constexpr_string(const char (&arr)[N]) noexcept
		{
			for (size_type i = 0; i < N; ++i)
			{
				buffer[i] = arr[i];
			}
		}

		[[nodiscard]] static consteval size_type capacity() noexcept { return N; }
		// Uses strlen
		[[nodiscard]] constexpr size_type size() const noexcept;
		[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

		constexpr char& operator[](size_type i) noexcept { return buffer[i]; }
		constexpr const char& operator[](size_type i) const noexcept { return buffer[i]; }

		constexpr const char* data() const noexcept { return buffer; }
		constexpr operator const char*() const noexcept { return data(); }

		template <typename F>
		[[nodiscard]] consteval constexpr_string<N> filter_if(F f) const noexcept;
		template <size_type... Vals>
		[[nodiscard]] consteval constexpr_string<N> filter_range(index_sequence<Vals...>) const noexcept;
		[[nodiscard]] consteval constexpr_string<N> filter_range(size_type start, size_type end) const noexcept;
		template <size_type OtherN>
		[[nodiscard]] consteval constexpr_string<N> filter(const constexpr_string<OtherN>& str) const noexcept;
		[[nodiscard]] consteval constexpr_string<N> filter(char x) const noexcept;

		template <size_type N0, size_type N1>
		[[nodiscard]] consteval constexpr_string<N> replace(
			const constexpr_string<N0>& filter, const constexpr_string<N1>& replacement, size_type offset = 0,
			bool linear = false
		) const noexcept;

		template <size_type N0, size_type N1>
		[[nodiscard]] consteval constexpr_string<N> replace_first(
			const constexpr_string<N0>& filter, const constexpr_string<N1>& replacement, size_type offset = 0
		) const noexcept;

		template <size_type I>
		[[nodiscard]] consteval constexpr_string<I> refit() const noexcept;

		template <size_type OtherN>
		[[nodiscard]] consteval size_type
		find(const constexpr_string<OtherN>& str, size_type offset = 0) const noexcept;

		consteval void copy_from(string_view str) noexcept;

		[[nodiscard]] constexpr string_view view() const noexcept { return string_view::from_buffer(buffer, size()); }
		[[nodiscard]] explicit constexpr operator string_view() const noexcept { return view(); }
	};

	template <size_type N>
	[[nodiscard]] constexpr bool operator==(constexpr_string<N> lhs, const char* rhs)
	{
		return lhs.view() == string_view::from_string_length(rhs);
	}

	template <size_type A, size_type B>
	[[nodiscard]] consteval auto operator+(constexpr_string<A> lhs, constexpr_string<B> rhs) noexcept
	{
	    constexpr size_type retSize = A + B + 1;
		constexpr_string<retSize> retval;

        const size_type lhsSize = lhs.size();

		// copy up to first nil in lhs:
		for (size_type i = 0; i < lhsSize; ++i)
		{
			retval[i] = lhs[i];
		}
		// copy entire rhs buffer, including trailing nils:
		for (size_type i = 0; i < B; ++i)
		{
			retval[lhsSize + i] = rhs[i];
		}
		// zero out the leftovers, if any:
		for (size_type i = lhsSize + B; i < retSize; ++i)
		{
			retval[i] = '\0';
		}

		return retval;
	}

	template <size_type N>
	template <typename F>
	inline consteval constexpr_string<N> constexpr_string<N>::filter_if(F f) const noexcept
	{
		constexpr_string<N> ret{};
		size_type j = 0;
		for (size_type i = 0; i < N; ++i)
		{
		    if (f(i))
		    {
		        ret[j] = buffer[i];
		        ++j;
		    }
		}

		for (; j < N; ++j)
		{
			ret[j] = 0;
		}
		return ret;
	}

	template <size_type N>
	template <size_type... Vals>
	inline consteval constexpr_string<N> constexpr_string<N>::filter_range(index_sequence<Vals...>) const noexcept
	{
		constexpr_string<N> ret{};
		size_type j = 0;
		for (size_type i = 0; i < N; ++i)
		{
			if (index_sequence<Vals...>::template contains<i>)
			{
				ret[j] = buffer[i];
				++j;
			}
		}

		for (; j < N; ++j)
		{
			ret[j] = 0;
		}
		return ret;
	}

	template <size_type N>
	inline consteval constexpr_string<N>
	constexpr_string<N>::filter_range(const size_type start, const size_type end) const noexcept
	{
		return filter_if([&](const size_type i) { return i < start || i > end; });
	}

	template <size_type N>
	template <size_type OtherN>
	inline consteval constexpr_string<N> constexpr_string<N>::filter(const constexpr_string<OtherN>& str) const noexcept
	{
	    if (empty())
	    {
			return *this;
	    }

		size_type start = find(str);
		if (start == npos)
		{
			return *this;
		}

		constexpr_string<N> result = filter_range(start, start + str.size() - 1);
		if (result.find(str) == npos)
		{
			return result;
		}
		else
		{
			return result.filter(str);
		}
	}

	template <size_type N>
	inline consteval constexpr_string<N> constexpr_string<N>::filter(char x) const noexcept
	{
		return filter_if([&](size_type i) { return x != buffer[i]; });
	}

	template <size_type N>
	template <size_type N0, size_type N1>
	inline consteval constexpr_string<N> constexpr_string<N>::replace_first(
		const constexpr_string<N0>& filter, const constexpr_string<N1>& replacement, size_type offset
	) const noexcept
	{
		size_type start = find(filter, offset);
		if (start == npos)
		{
			return *this;
		}

		size_type end = start + filter.size();

		constexpr_string<N> ret{};
		size_type j = 0;
		for (size_type i = 0; i < N; ++i)
		{
			if (i == start)
			{
				for (size_type k = 0; k < replacement.size(); k++)
				{
					ret[j] = replacement[k];
					j++;

					if (j == N - 1)
					{
						break;
					}
				}
				i = end;
			}

			if (j == N - 1)
			{
				break;
			}

			ret[j] = buffer[i];
			j++;
		}

		for (; j < N; ++j)
		{
			ret[j] = '\0';
		}

		return ret;
	}

	template <size_type N>
	template <size_type N0, size_type N1>
	inline consteval constexpr_string<N> constexpr_string<N>::replace(
		const constexpr_string<N0>& filter, const constexpr_string<N1>& replacement, size_type offset, const bool linear
	) const noexcept
	{
		constexpr_string<N> ret = replace_first(filter, replacement, offset);
		if (linear)
		{
			size_type i = find(filter, offset);
			while (i != npos)
			{
				i = ret.find(filter, i + replacement.size());
				ret = ret.replace_first(filter, replacement, i);
			}
		}
		else
		{
			while (ret.find(filter, offset) != npos)
			{
				ret = ret.replace_first(filter, replacement, offset);
			}
		}

		return ret;
	}

	template <size_type N>
	template <size_type I>
	inline consteval constexpr_string<I> constexpr_string<N>::refit() const noexcept
	{
		constexpr_string<I> ret{};

		constexpr size_type minSize = I < N ? I : N;
		for (size_type i = 0; i < minSize; ++i)
		{
			ret[i] = buffer[i];
		}
		for (size_type i = minSize; i < I; ++i)
		{
			ret[i] = '\0';
		}
		return ret;
	}

	template <size_type N>
	template <size_type OtherN>
	inline consteval size_type
	constexpr_string<N>::find(const constexpr_string<OtherN>& str, const size_type offset) const noexcept
	{
		if (str.size() > size())
		{
			return npos;
		}

		if (str.empty())
		{
			return 0;
		}

		size_type j = 0;
		for (size_type i = offset; i < N; ++i)
		{
			if (buffer[i] == str[j])
			{
				j++;
			}
			else
			{
				j = 0;
			}

			if (j == str.size())
			{
				return (i - str.size()) + 1;
			}
		}

		return npos;
	}

	template <size_type N>
	inline constexpr size_type constexpr_string<N>::size() const noexcept
	{
        if constexpr (N == 1ull)
        {
            return buffer[0] != '\0';
        }
	    else
	    {
	        size_type s = N;
	        while (buffer[s - 1ull] == '\0')
	        {
	            --s;
	            if (s == 0ull)
	            {
	                break;
	            }
	        }
	        return s;
	    }
	}

	template <size_type N>
	inline consteval void constexpr_string<N>::copy_from(string_view str) noexcept
	{
		size_type minSize = N < str.size() ? N : str.size();
		for (size_type i = 0; i < minSize; ++i)
		{
			buffer[i] = str[i];
		}
		for (size_type i = minSize; i < N; ++i)
		{
			buffer[i] = '\0';
		}
	}

	template <typename T>
	constexpr bool is_constexpr_string = false;

	template <size_type N>
	constexpr bool is_constexpr_string<constexpr_string<N>> = true;

	template <typename T>
	concept constexpr_string_type = is_constexpr_string<T>;

} // namespace rsl
