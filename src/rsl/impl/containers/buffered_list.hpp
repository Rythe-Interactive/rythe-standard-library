#pragma once
#include "../rsl_core.hpp"

#include "views.hpp"

#include "iterators.hpp"

namespace rsl
{
    template <typename ValueType, size_type maxSize>
    class buffered_list
    {
    public:
        using value_type = ValueType;
        using iterator_type = value_type*;
        using const_iterator_type = const value_type*;
        using reverse_iterator_type = reverse_iterator<iterator_type>;
        using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;
        using view_type = array_view<value_type>;
        using const_view_type = array_view<const value_type>;

        [[rythe_always_inline]] constexpr buffered_list() noexcept = default;
        [[rythe_always_inline]] constexpr buffered_list(const buffered_list& src) noexcept(copy_construct_noexcept);
        [[rythe_always_inline]] constexpr buffered_list(buffered_list&& src) noexcept(move_construct_noexcept);
        template <size_type N>
        [[rythe_always_inline]] constexpr buffered_list(const value_type (&arr)[N]) noexcept(copy_construct_noexcept);
        template <size_type N>
        [[rythe_always_inline]] constexpr buffered_list(value_type (&&arr)[N]) noexcept(move_construct_noexcept);
        [[rythe_always_inline]] constexpr buffered_list(view_type src) noexcept(copy_construct_noexcept);
        template <typename... Args>
        [[rythe_always_inline]] constexpr static buffered_list create_in_place(size_type count, Args&&... args)
            noexcept(construct_noexcept<Args...>);

        [[rythe_always_inline]] constexpr buffered_list& operator=(const buffered_list& src)
            noexcept(copy_assign_noexcept && copy_construct_noexcept);
        template <size_type N>
        [[rythe_always_inline]] constexpr buffered_list& operator=(const value_type (&arr)[N])
            noexcept(copy_assign_noexcept && copy_construct_noexcept);
        [[rythe_always_inline]] constexpr buffered_list& operator=(view_type src)
            noexcept(copy_assign_noexcept && copy_construct_noexcept);

        template <typename... Args>
        [[rythe_always_inline]] constexpr void resize(size_type newSize, Args&&... args)
            noexcept(construct_noexcept<Args...>);

        [[rythe_always_inline]] constexpr void push_back(const value_type& value) noexcept(move_construct_noexcept);
        [[rythe_always_inline]] constexpr void push_back(value_type&& value) noexcept(move_construct_noexcept);

        template <typename... Args>
        [[rythe_always_inline]] constexpr value_type& emplace_back(Args&&... args)
            noexcept(construct_noexcept<Args...>);

        [[rythe_always_inline]] constexpr void pop_back(size_type count = 1ull) noexcept;

        [[rythe_always_inline]] constexpr void clear() noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& at(size_type i) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& raw_at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& raw_at(size_type i) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator[](size_type i);
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator[](size_type i) const;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr view_type view() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_view_type view() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr view_type raw_view() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_view_type raw_view() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator const value_type*() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator view_type() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator const_view_type() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;

    private:
        union value_container
        {
            value_type value;
            byte dummy;
        };

        value_container m_buffer[maxSize];
        size_type m_size;

        constexpr static size_type validate_size(size_type newSize) noexcept;

        constexpr static bool copy_assign_noexcept = is_nothrow_copy_assignable_v<value_type>;
        constexpr static bool copy_construct_noexcept = is_nothrow_copy_constructible_v<value_type>;
        constexpr static bool move_assign_noexcept = is_nothrow_move_assignable_v<value_type>;
        constexpr static bool move_construct_noexcept = is_nothrow_move_constructible_v<value_type>;

        template <typename... Args>
        constexpr static bool construct_noexcept = is_nothrow_constructible_v<value_type, Args...>;

        [[rythe_always_inline]] constexpr void
        copy_assign_from_unsafe_impl(size_type begin, size_type end, const value_type* src)
            noexcept(copy_assign_noexcept);

        [[rythe_always_inline]] constexpr void
        copy_construct_from_unsafe_impl(size_type begin, size_type end, const value_type* src)
            noexcept(copy_construct_noexcept);

        [[rythe_always_inline]] constexpr void
        move_assign_from_unsafe_impl(size_type begin, size_type end, const value_type* src)
            noexcept(move_assign_noexcept);

        [[rythe_always_inline]] constexpr void
        move_construct_from_unsafe_impl(size_type begin, size_type end, const value_type* src)
            noexcept(move_construct_noexcept);

        template <typename... Args>
        [[rythe_always_inline]] constexpr void emplace_unsafe_impl(size_type begin, size_type end, Args&&... args)
            noexcept(construct_noexcept<Args...>);

        [[rythe_always_inline]] constexpr void reset_unsafe_impl(size_type begin, size_type end) noexcept;

        [[rythe_always_inline]] constexpr void copy_assign_impl(const value_type* src, size_type srcSize)
            noexcept(copy_assign_noexcept && copy_construct_noexcept);
    };

    template <typename ValueType, size_type maxSize>
    buffered_list(const ValueType (&)[maxSize]) -> buffered_list<ValueType, maxSize>;

    template <typename ValueType>
    using b_list64 = buffered_list<ValueType, 64>;
    template <typename ValueType>
    using b_list128 = buffered_list<ValueType, 128>;
    template <typename ValueType>
    using b_list256 = buffered_list<ValueType, 256>;
    template <typename ValueType>
    using b_list512 = buffered_list<ValueType, 512>;
} // namespace rsl

#include "buffered_list.inl"
