#pragma once
#include "factory.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T>
		struct memset_zero : bool_constant<is_char_v<T>>
		{
		};

		template <typename T>
			requires requires{ T::memset_zero; }
		struct memset_zero<T> : bool_constant<T::memset_zero>
		{
		};

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr T default_construct_single_inline(Args&&... args)
			noexcept(noexcept(basic_factory<T>{}.construct_single_inline(forward<Args>(args)...)))
		{
			return basic_factory<T>{}.construct_single_inline(forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] constexpr void* default_construct(void* ptr, size_type count, Args&&... args)
			noexcept(noexcept(basic_factory<T>{}.construct(ptr, count, forward<Args>(args)...)))
		{
			return basic_factory<T>{}.construct(ptr, count, forward<Args>(args)...);
		}

		template <typename T>
		[[rythe_always_inline]] constexpr void* default_copy(void* dst, const void* src, size_type count)
			noexcept(noexcept(basic_factory<T>{}.copy(dst, static_cast<const T*>(src), count)))
		{
			return basic_factory<T>{}.copy(dst, static_cast<const T*>(src), count);
		}

		template <typename T>
		[[rythe_always_inline]] constexpr void* default_move(void* dst, void* src, size_type count)
			noexcept(noexcept(basic_factory<T>{}.move(dst, static_cast<T*>(src), count)))
		{
			return basic_factory<T>{}.move(dst, static_cast<T*>(src), count);
		}

		template <typename T>
		[[rythe_always_inline]] constexpr void default_destroy(void* ptr, size_type count)
			noexcept(noexcept(basic_factory<T>{}.destroy(static_cast<T*>(ptr), count)))
		{
			basic_factory<T>{}.destroy(static_cast<T*>(ptr), count);
		}
	} // namespace internal

	template <constructible_at_all T>
	template <typename ... Args>
	constexpr T basic_factory<T>::construct_single_inline(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		return T(rsl::forward<Args>(args)...);
	}

	template <constructible_at_all T>
	template <typename... Args>
	constexpr T* basic_factory<T>::construct(void* ptr, const size_type count, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		if constexpr ((is_trivially_default_constructible_v<T>) && sizeof...(Args) == 0)
		{
			if constexpr (internal::memset_zero<T>::value)
			{
				constexpr_memset(ptr, 0, count * sizeof(T));
			}

			return static_cast<T*>(ptr);
		}
		else
		{
			T* first = new (ptr) T(rsl::forward<Args>(args)...);

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(rsl::forward<Args>(args)...);
			}

			return first;
		}
	}

	template <constructible_at_all T>
	constexpr T* basic_factory<T>::copy(void* dst, const T* src, const size_type count) noexcept(is_nothrow_copy_constructible_v<T>)
	{
		if constexpr (is_trivially_copy_constructible_v<T>)
		{
			constexpr_memcpy(dst, src, count * sizeof(T));

			return static_cast<T*>(dst);
		}
		else
		{
			T* first = new (dst) T(src[0]);

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(src[i]);
			}

			return first;
		}
	}

	template <constructible_at_all T>
	constexpr T* basic_factory<T>::move(void* dst, T* src, const size_type count) noexcept(is_nothrow_move_constructible_v<T>)
	{
		if constexpr (is_trivially_copy_constructible_v<T>)
		{
			constexpr_memcpy(dst, src, count * sizeof(T));

			if constexpr (internal::memset_zero<T>::value)
			{
				constexpr_memset(src, 0, count * sizeof(T));
			}

			return static_cast<T*>(dst);
		}
		else
		{
			T* first = new (dst) T(rsl::move(src[0]));

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(rsl::move(src[i]));
			}

			return first;
		}
	}

	template <constructible_at_all T>
	constexpr void basic_factory<T>::destroy(T* ptr, const size_type count) noexcept
	{
		if constexpr (!is_trivially_destructible_v<T>)
		{
			for (size_type i = 0; i < count; i++)
			{
				(ptr + i)->~T();
			}
		}
		else if constexpr (internal::memset_zero<T>::value)
		{
			constexpr_memset(ptr, 0, count * sizeof(T));
		}
	}

	template <typename T>
	void* typed_polymorphic_factory<T>::construct(void* ptr, const size_type count) const
	{
		return internal::default_construct<T>(ptr, count);
	}

	template <typename T>
	void* typed_polymorphic_factory<T>::copy(void* dst, const void* src, const size_type count) const
	{
		return internal::default_copy<T>(dst, src, count);
	}

	template <typename T>
	void* typed_polymorphic_factory<T>::move(void* dst, void* src, const size_type count) const
	{
		return internal::default_move<T>(dst, src, count);
	}

	template <typename T>
	void typed_polymorphic_factory<T>::destroy(void* ptr, const size_type count) const noexcept
	{
		internal::default_destroy<T>(ptr, count);
	}

	template <typename T>
	size_type typed_polymorphic_factory<T>::type_size() const noexcept
	{
		return sizeof(T);
	}

	template <typename T>
	bool typed_polymorphic_factory<T>::trivial_copy() const noexcept
	{
		return is_trivially_copyable_v<T>;
	}

	template <typename T>
	id_type typed_polymorphic_factory<T>::type_id() const noexcept
	{
		return rsl::type_id<T>();
	}

	template <typename T>
	constexpr type_erased_factory::type_erased_factory(construct_type_signal_type<T>) noexcept
		: m_constructFunc(&internal::default_construct<T>),
		  m_copyFunc(&internal::default_copy<T>),
		  m_moveFunc(&internal::default_move<T>),
		  m_destroyFunc(&internal::default_destroy<T>),
		  m_typeSize(sizeof(T)),
		  m_triviallyCopyable(is_trivially_copyable_v<T>),
		  m_typeId(rsl::type_id<T>())
	{
	}

	template <untyped_factory_type Factory>
	bool can_trivially_copy(Factory& factory) noexcept
	{
		if constexpr (requires(Factory& f) {
						  { f.trivial_copy() } noexcept -> convertible_to<bool>;
					  })
		{
			return factory.trivial_copy();
		}
		else
		{
			return false;
		}
	}
} // namespace rsl
