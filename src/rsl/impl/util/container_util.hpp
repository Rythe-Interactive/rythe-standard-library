#pragma once

#include "common.hpp"

#include "../containers/iterators.hpp"

namespace rsl
{
    template <char_type T>
    [[nodiscard]] constexpr size_type string_length(add_const_t<T>* str, T terminator = T{}) noexcept
    {
        size_type length = 0;
        while (*str != terminator)
        {
            ++length;
            ++str;
        }

        return length;
    }

    RYTHE_HAS_FUNC(begin)
    RYTHE_HAS_FUNC(end)
    RYTHE_HAS_FUNC(cbegin)
    RYTHE_HAS_FUNC(cend)
    RYTHE_HAS_FUNC(rbegin)
    RYTHE_HAS_FUNC(rend)
    RYTHE_HAS_FUNC(crbegin)
    RYTHE_HAS_FUNC(crend)
    RYTHE_HAS_FUNC(size)
    RYTHE_HAS_FUNC(empty)
    RYTHE_HAS_FUNC(data)
    RYTHE_HAS_FUNC(at)
    RYTHE_HAS_FUNC(view)
    RYTHE_HAS_FUNC(append)
    RYTHE_HAS_FUNC(insert)
    RYTHE_HAS_FUNC(front)
    RYTHE_HAS_FUNC(back)

    namespace internal
    {
        template <typename, typename T>
        struct has_index_operator_impl
        {
            static_assert(
                    ::rsl::integral_constant<bool_t<T>, false>::value,
                    "Second template param needs to be of function type."
                    );
        };

        template <typename C, typename Ret, typename... Args>
        struct has_index_operator_impl<C, Ret(Args...)>
        {
            static constexpr bool value = requires(C& val, Args&&... args)
                {
                    { val.operator[](::rsl::forward<Args>(args)...) } -> ::rsl::convertible_to<Ret>;
                };
        };
    }

    template <typename C, typename F>
    constexpr bool has_index_operator_v =
            internal::has_index_operator_impl<C, F>::value;

    template <typename C, typename F>
    concept has_index_operator = has_index_operator_v<C, F>;

    template<typename T>
    using container_iter_type = decltype(declval<T>().begin());

    template<typename T>
    using container_value_type = iter_value_t<container_iter_type<T>>;

    template <typename T>
    concept container_like = has_begin<T, any_type()> && has_end<T, any_type()> && has_size<T, size_type()> && has_empty<T, bool()>;

    template <typename T>
    concept contiguous_container_like = container_like<T> && has_data<T, any_type()> && has_at<T, any_type(size_type)> && has_index_operator<T, any_type(size_type)>;

    template <typename T>
    concept string_like = contiguous_container_like<T> && is_char_v<container_value_type<T>>;

    template <has_begin<any_type()> Container>
    [[nodiscard]] constexpr auto begin(Container& container) noexcept(noexcept(container.begin())) -> decltype(container.begin())
    {
        return container.begin();
    }

    template <typename Container>
        requires has_begin<const Container, any_type()>
    [[nodiscard]] constexpr auto begin(const Container& container) noexcept(noexcept(container.begin())) -> decltype(container.begin())
    {
        return container.begin();
    }

    template <has_end<any_type()> Container>
    [[nodiscard]] constexpr auto end(Container& container) noexcept(noexcept(container.end())) -> decltype(container.end())
    {
        return container.end();
    }

    template <typename Container>
        requires has_end<const Container, any_type()>
    [[nodiscard]] constexpr auto end(const Container& container) noexcept(noexcept(container.end())) -> decltype(container.end())
    {
        return container.end();
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr T* begin(T (& arr)[Size]) noexcept
    {
        return arr;
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr T* end(T (& arr)[Size]) noexcept
    {
        return arr + Size;
    }

    namespace internal
    {
        template <typename T>
        concept rsl_begin_valid = requires(T& v) { ::rsl::begin(v); };

        template <typename T>
        concept rsl_begin_invalid = !rsl_begin_valid<T>;

        template <typename T>
        concept rsl_end_valid = requires(T& v) { ::rsl::end(v); };

        template <typename T>
        concept rsl_end_invalid = !rsl_end_valid<T>;
    } // namespace internal

    template <typename Container>
        requires internal::rsl_begin_valid<const Container>
    [[nodiscard]] constexpr auto cbegin(
            const Container& container
            ) noexcept(noexcept(::rsl::begin(container))) -> decltype(::rsl::begin(container))
    {
        return ::rsl::begin(container);
    }

    // Fallback.
    template <typename Container>
        requires has_cbegin<const Container, any_type()> && internal::rsl_begin_invalid<const Container>
    [[nodiscard]] constexpr auto cbegin(
            const Container& container
            ) noexcept(noexcept(container.cbegin())) -> decltype(container.cbegin())
    {
        return container.cbegin();
    }

    template <typename Container>
        requires internal::rsl_end_valid<const Container>
    [[nodiscard]] constexpr auto cend(
            const Container& container
            ) noexcept(noexcept(::rsl::end(container))) -> decltype(::rsl::end(container))
    {
        return ::rsl::end(container);
    }

    // Fallback.
    template <typename Container>
        requires has_cend<const Container, any_type()> && internal::rsl_end_invalid<const Container>
    [[nodiscard]] constexpr auto cend(const Container& container) noexcept(noexcept(container.cend())) -> decltype(container.cend())
    {
        return container.cend();
    }

    template <has_rbegin<any_type()> Container>
    [[nodiscard]] constexpr auto rbegin(Container& container) noexcept(noexcept(container.rbegin())) -> decltype(container.rbegin())
    {
        return container.rbegin();
    }

    template <typename Container>
        requires has_rbegin<const Container, any_type()>
    [[nodiscard]] constexpr auto rbegin(
            const Container& container
            ) noexcept(noexcept(container.rbegin())) -> decltype(container.rbegin())
    {
        return container.rbegin();
    }

    template <has_rend<any_type()> Container>
    [[nodiscard]] constexpr auto rend(Container& container) noexcept(noexcept(container.rend())) -> decltype(container.rend())
    {
        return container.rend();
    }

    template <typename Container>
        requires has_rend<const Container, any_type()>
    [[nodiscard]] constexpr auto rend(const Container& container) noexcept(noexcept(container.rend())) -> decltype(container.rend())
    {
        return container.rend();
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr reverse_iterator<T*> rbegin(T (& arr)[Size]) noexcept
    {
        return reverse_iterator<T*>(arr + Size);
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr reverse_iterator<T*> rend(T (& arr)[Size]) noexcept
    {
        return reverse_iterator<T*>(arr);
    }

    namespace internal
    {
        template <typename T>
        concept rsl_rbegin_valid = requires(T& v) { ::rsl::rbegin(v); };

        template <typename T>
        concept rsl_rbegin_invalid = !rsl_rbegin_valid<T>;

        template <typename T>
        concept rsl_rend_valid = requires(T& v) { ::rsl::rend(v); };

        template <typename T>
        concept rsl_rend_invalid = !rsl_rend_valid<T>;
    } // namespace internal

    template <typename Container>
        requires internal::rsl_rbegin_valid<const Container>
    [[nodiscard]] constexpr auto crbegin(
            const Container& container
            ) noexcept(noexcept(::rsl::rbegin(container))) -> decltype(::rsl::rbegin(container))
    {
        return ::rsl::rbegin(container);
    }

    // Fallback.
    template <typename Container>
        requires has_crbegin<const Container, any_type()> && internal::rsl_rbegin_invalid<const Container>
    [[nodiscard]] constexpr auto crbegin(
            const Container& container
            ) noexcept(noexcept(container.crbegin())) -> decltype(container.crbegin())
    {
        return container.crbegin();
    }

    template <typename Container>
        requires internal::rsl_rend_valid<const Container>
    [[nodiscard]] constexpr auto crend(
            const Container& container
            ) noexcept(noexcept(::rsl::rend(container))) -> decltype(::rsl::rend(container))
    {
        return ::rsl::rend(container);
    }

    // Fallback.
    template <typename Container>
        requires has_crend<const Container, any_type()> && internal::rsl_rend_invalid<const Container>
    [[nodiscard]] constexpr auto crend(const Container& container) noexcept(noexcept(container.crend())) -> decltype(container.crend())
    {
        return container.crend();
    }

    template <typename Container>
        requires has_size<const Container, any_type()>
    [[nodiscard]] constexpr auto size(const Container& container) noexcept(noexcept(container.size())) -> decltype(container.size())
    {
        return container.size();
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr size_type size(const T (& )[Size]) noexcept
    {
        return Size;
    }

    template <typename Container>
        requires has_empty<const Container, any_type()>
    [[nodiscard]] constexpr auto empty(const Container& container) noexcept(noexcept(container.empty())) -> decltype(container.empty())
    {
        return container.empty();
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr bool empty(const T (& )[Size]) noexcept
    {
        return false;
    }

    template <has_data<any_type()> Container>
    [[nodiscard]] constexpr auto data(Container& container) noexcept(noexcept(container.data())) -> decltype(container.data())
    {
        return container.data();
    }

    template <typename Container>
        requires has_data<const Container, any_type()>
    [[nodiscard]] constexpr auto data(const Container& container) noexcept(noexcept(container.data())) -> decltype(container.data())
    {
        return container.data();
    }

    template <typename T, size_type Size>
    [[nodiscard]] constexpr T* data(T (& arr)[Size]) noexcept
    {
        return arr;
    }
} // namespace rsl
