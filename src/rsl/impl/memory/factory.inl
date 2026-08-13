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
            noexcept(noexcept(factory<T>{}.construct_single_inline(forward<Args>(args)...)))
        {
            return factory<T>{}.construct_single_inline(forward<Args>(args)...);
        }

        template <typename T, typename... Args>
        [[rythe_always_inline]] constexpr void* default_construct(void* ptr, size_type count, Args&&... args)
            noexcept(noexcept(factory<T>{}.construct(ptr, count, forward<Args>(args)...)))
        {
            return factory<T>{}.construct(ptr, count, forward<Args>(args)...);
        }

        template <typename T>
        [[rythe_always_inline]] constexpr void* default_copy(void* dst, const void* src, size_type count)
            noexcept(noexcept(factory<T>{}.copy(dst, static_cast<const T*>(src), count)))
        {
            return factory<T>{}.copy(dst, static_cast<const T*>(src), count);
        }

        template <typename T>
        [[rythe_always_inline]] constexpr void* default_move(void* dst, void* src, size_type count)
            noexcept(noexcept(factory<T>{}.move(dst, static_cast<T*>(src), count)))
        {
            return factory<T>{}.move(dst, static_cast<T*>(src), count);
        }

        template <typename T>
        [[rythe_always_inline]] constexpr void default_destroy(void* ptr, size_type count)
            noexcept(noexcept(factory<T>{}.destroy(static_cast<T*>(ptr), count)))
        {
            factory<T>{}.destroy(static_cast<T*>(ptr), count);
        }

        template <typename...>
        struct can_construct_from_view : false_type
        {};

        template<typename T, typename Arg>
        struct can_construct_from_view<T, Arg>
        {
            static constexpr bool value = has_static_from_view_v<T, T(Arg)>;
        };
    } // namespace internal

    template <constructible_at_all T>
    template <typename ... Args>
    constexpr T factory<T>::construct_single_inline(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
    {
        if constexpr (internal::can_construct_from_view<T, Args...>::value)
        {
            return T::from_view(args...);
        }
        else
        {
            return T(rsl::forward<Args>(args)...);
        }
    }

    template <constructible_at_all T>
    template <typename... Args>
    constexpr T* factory<T>::construct(void* ptr, const size_type count, Args&&... args)
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
            if constexpr (internal::can_construct_from_view<T, Args...>::value)
            {
                T* first = new (ptr) T(T::from_view(args...));

                for (size_type i = 1; i < count; i++) { new (first + i) T(T::from_view(args...)); }

                return first;
            }
            else
            {
                T* first = new (ptr) T(rsl::forward<Args>(args)...);

                for (size_type i = 1; i < count; i++) { new (first + i) T(rsl::forward<Args>(args)...); }

                return first;
            }
        }
    }

    template <constructible_at_all T>
    constexpr T* factory<T>::copy(void* dst, const T* src, const size_type count) noexcept(is_nothrow_copy_constructible_v<T>)
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
    constexpr T* factory<T>::move(void* dst, T* src, const size_type count) noexcept(is_nothrow_move_constructible_v<T>)
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
    constexpr void factory<T>::destroy(T* ptr, const size_type count) noexcept
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
} // namespace rsl
