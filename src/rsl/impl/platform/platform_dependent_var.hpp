#pragma once
#include "../util/common.hpp"
#include "../util/error_handling.hpp"

namespace rsl
{
	template <size_type PlatformIndex>
	struct platform_tag
	{
		constexpr static size_type index = PlatformIndex;
	};

	using windows_tag = platform_tag<RYTHE_WINDOWS_INDEX>;
	using linux_tag = platform_tag<RYTHE_LINUX_INDEX>;

	template <typename Tag, typename T>
	struct platform_specific_var
	{
		using value_type = T;
		constexpr static size_type index = Tag::index;
		T value;

		template <typename V>
		constexpr explicit platform_specific_var(V&& v)
			: value(rsl::forward<V>(v))
		{
		}

		constexpr platform_specific_var() = default;
	};

	template <typename T>
	struct windows_var : platform_specific_var<windows_tag, T>
	{
		using platform_specific_var<windows_tag, T>::platform_specific_var;
	};

	template <typename T>
	windows_var(T) -> windows_var<T>;

	template <typename T>
	struct linux_var : platform_specific_var<linux_tag, T>
	{
		using platform_specific_var<linux_tag, T>::platform_specific_var;
	};

	template <typename T>
	linux_var(T) -> linux_var<T>;

	namespace internal
	{
		template <typename T, typename... Others>
		struct is_same_value_type
		{
			constexpr static bool value = (rsl::is_same_v<typename T::value_type, typename Others::value_type> && ...);
		};
	} // namespace internal

	template <typename T, size_type MaxPlatforms = RYTHE_MAX_PLATFORMS>
	struct platform_dependent_var
	{
		template <typename Var, typename... Vars>
			requires internal::is_same_value_type<Var, Vars...>::value
		constexpr platform_dependent_var(Var&& var, Vars&&... vars)
		{
			assign<Var>(var.index, var.value);
			(assign<Vars>(vars.index, vars.value), ...);
		}

		constexpr platform_dependent_var() = default;

		using value_type = T;
		constexpr const value_type& get() const { return m_values[RYTHE_PLATFORM_INDEX]; }
		constexpr value_type& get() { return m_values[RYTHE_PLATFORM_INDEX]; }
		constexpr operator const value_type&() const { return get(); }
		constexpr operator value_type&() { return get(); }

	private:
	    template<typename Var, typename Val>
		constexpr void assign(size_type index, Val&& value)
	    {
	        if constexpr (is_rvalue_reference_v<Var>)
	        {
	            m_values[index] = rsl::move(value);
	        }
	        else
	        {
	            m_values[index] = value;
	        }
	    }

		T m_values[MaxPlatforms];
	};

	template <typename Var, typename... Vars>
	platform_dependent_var(Var&&, Vars&&...) -> platform_dependent_var<typename Var::value_type>;
} // namespace rsl
