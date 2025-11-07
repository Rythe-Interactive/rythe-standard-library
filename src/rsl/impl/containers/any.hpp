#pragma once

#include "../util/assert.hpp"
#include "../util/common.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
	template <size_type MaxSize>
	class any
	{
	private:
		unsigned char m_data[MaxSize];

	public:
		constexpr any() noexcept
			: m_data{}
		{
		}

		any(const any& other) { memcpy(m_data, other.m_data, sizeof(other.m_data)); }

		any(any&& other) noexcept
			: m_data(move(other.m_data))
		{
		}

		template <class ValueType>
		explicit any(ValueType&& value)
			requires(conjunction_v<
					 negation<is_same<decay_t<ValueType>, any>>,
					 negation<rsl::is_specialization<decay_t<ValueType>, in_place_type_signal_type>>,
					 is_copy_constructible<decay_t<ValueType>>>)
		{
			emplace<decay_t<ValueType>>(forward<ValueType>(value));
		}

		template <class ValueType, class... Types>
		explicit any(in_place_type_signal_type<ValueType>, Types&&... args)
			requires(conjunction_v<
					 is_constructible<decay_t<ValueType>, Types...>, is_copy_constructible<decay_t<ValueType>>>)
		{
			emplace<decay_t<ValueType>>(forward<Types>(args)...);
		}

		~any() noexcept { reset(); }

		any& operator=(const any& other)
		{
			*this = any{other};
			return *this;
		}

		any& operator=(any&& other) noexcept
		{
			reset();
			m_data = move(other.m_data);
			return *this;
		}

		template <class ValueType>
		any& operator=(ValueType&& value)
			requires(conjunction_v<
					 negation<is_same<decay_t<ValueType>, any>>, is_copy_constructible<decay_t<ValueType>>>)
		{
			*this = any{forward<ValueType>(value)};
			return *this;
		}

		template <class ValueType, class... Types>
		decay_t<ValueType>& emplace(Types&&... args)
			requires(conjunction_v<
					 is_constructible<decay_t<ValueType>, Types...>, is_copy_constructible<decay_t<ValueType>>>)
		{
			reset();
			return emplace<decay_t<ValueType>>(forward<Types>(args)...);
		}

		void reset() noexcept
		{
			// delete m_data;
		}

        void swap(any& other) noexcept
        {
            any tmp = static_cast<any&&>(*this);
            *this = static_cast<any&&>(other);
            other = tmp;
        }

		[[nodiscard]] bool has_value() const noexcept { return m_data != nullptr; }

		template <class Decayed>
		[[nodiscard]] const Decayed* cast() const noexcept
		{
			return reinterpret_cast<const Decayed*>(&m_data);
		}

		template <class Decayed>
		[[nodiscard]] Decayed* cast() noexcept
		{
			return const_cast<Decayed*>(static_cast<const any*>(this)->cast<Decayed>());
		}

	private:
		template <class Decayed, class... Types>
		Decayed& emplace(Types&&... args)
		{
			auto& obj = reinterpret_cast<Decayed&>(m_data);
			construct_at(obj, forward<Types>(args)...);
			return obj;
		}
	};

	template <typename ValueType>
    void swap(rsl::any<sizeof(ValueType)>& left, rsl::any<sizeof(ValueType)>& right) noexcept
	{
		left.swap(right);
	}

	template <class ValueType, class... Types>
	[[nodiscard]] any<sizeof(ValueType)> make_any(Types&&... args)
		requires(is_constructible_v<any<sizeof(ValueType)>, in_place_type_signal_type<ValueType>, Types...>)
	{
		return any<sizeof(ValueType)>{in_place_type_signal<ValueType>, forward<Types>(args)...};
	}

	template <class ValueType>
	[[nodiscard]] const ValueType* any_cast(const rsl::any<sizeof(ValueType)>* const any) noexcept
	{
		// retrieve a pointer to the ValueType contained in any, or null
		static_assert(!is_void_v<ValueType>, "rsl::any cannot contain void.");

		if constexpr (is_function_v<ValueType> || is_array_v<ValueType>)
		{
			return nullptr;
		}
		else
		{
			if (!any)
			{
				return nullptr;
			}

			return any->template cast<remove_cvr_t<ValueType>>();
		}
	}

	template <class ValueType>
	[[nodiscard]] ValueType* any_cast(rsl::any<sizeof(ValueType)>* const any) noexcept
	{
		// retrieve a pointer to the ValueType contained in any, or null
		static_assert(!is_void_v<ValueType>, "rsl::any cannot contain void.");

		if constexpr (is_function_v<ValueType> || is_array_v<ValueType>)
		{
			return nullptr;
		}
		else
		{
			if (!any)
			{
				return nullptr;
			}

			return any->template cast<remove_cvr_t<ValueType>>();
		}
	}

	template <typename T>
	[[nodiscard]] remove_cv_t<T> any_cast(const rsl::any<sizeof(T)>& any)
	{
		static_assert(
			is_constructible_v<remove_cv_t<T>, const remove_cvr_t<T>&>,
			"any_cast<T>(const any&) requires remove_cv_t<T> to be constructible from "
			"const remove_cv_t<remove_reference_t<T>>&"
		);

		const T* ptr = rsl::any_cast<remove_cvr_t<T>>(&any);
		rsl_hard_assert(ptr);

		return static_cast<remove_cv_t<T>>(*ptr);
	}

	template <typename T>
	[[nodiscard]] remove_cv_t<T> any_cast(rsl::any<sizeof(T)>& any)
	{
		static_assert(
			is_constructible_v<remove_cv_t<T>, remove_cvr_t<T>&>,
			"any_cast<T>(any&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>&"
		);

		const T* ptr = rsl::any_cast<remove_cvr_t<T>>(&any);
		rsl_hard_assert(ptr);

		return static_cast<remove_cv_t<T>>(*ptr);
	}

	template <typename T>
	[[nodiscard]] remove_cv_t<T> any_cast(rsl::any<sizeof(T)>&& any) // NOLINT(cppcoreguidelines*)
	{
		static_assert(
			is_constructible_v<remove_cv_t<T>, remove_cvr_t<T>>,
			"any_cast<T>(any&&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>"
		);

		const T* ptr = rsl::any_cast<remove_cvr_t<T>>(&any);
		rsl_hard_assert(ptr);

		return static_cast<remove_cv_t<T>>(move(*ptr));
	}
} // namespace rsl
