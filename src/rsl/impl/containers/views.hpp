#pragma once

#include "../util/common.hpp"
#include "../util/container_util.hpp"
#include "../util/primitives.hpp"

#include "iterators.hpp"

namespace rsl
{
    namespace internal
    {
        template <typename T, typename Iter>
        using select_const_iter = conditional_t<is_const_v<T>, Iter, conditional_t<same_as<Iter, T*>, const T*, const_iterator<Iter>>>;
    }

    template <typename T, contiguous_iterator Iter = T*, contiguous_iterator ConstIter = internal::select_const_iter<T, Iter>>
    struct array_view
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = add_const_t<T>*;
        using reference = T&;
        using const_reference = add_const_t<T>&;
        using iterator_type = Iter;
        using const_iterator_type = ConstIter;
        using reverse_iterator_type = reverse_iterator<iterator_type>;
        using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

        using const_view_type = conditional_t<is_const_v<T>, array_view, array_view<const value_type, const_iterator_type>>;

        [[rythe_always_inline]] constexpr array_view() noexcept = default;
        [[rythe_always_inline]] constexpr array_view(const array_view& other) noexcept = default;

        template <size_type N>
        [[rythe_always_inline]] constexpr array_view(T (& arr)[N]) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator array_view<const value_type, const_iterator_type>() const noexcept
            requires(!is_const_v<T>);

        [[nodiscard]] [[rythe_always_inline]] constexpr static array_view from_value(T& src) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr static array_view from_buffer(
                T* ptr,
                size_type count
                ) noexcept;
        template <not_same_as<T> Other>
        [[nodiscard]] [[rythe_always_inline]] constexpr static array_view from_buffer(
                Other* ptr,
                size_type count
                ) noexcept
            requires same_as<byte, remove_cvr_t<T>> && (is_const_v<T> || !is_const_v<Other>);
        template <contiguous_iterator It>
        [[rythe_always_inline]] constexpr static array_view from_iterator_pair(
                It first,
                It last
                ) noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>)
            requires same_as<iter_pointer_t<It>, T*>;
        [[nodiscard]] [[rythe_always_inline]] constexpr static array_view from_string_length(T* str, T terminator = T{}) noexcept
            requires char_type<T>;

        [[rythe_always_inline]] constexpr array_view& operator=(const array_view&) = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const array_view& rhs) const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const array_view& rhs) const noexcept;
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const value_type (& rhs)[N]) const noexcept;
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const value_type (& rhs)[N]) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reference front();
        [[nodiscard]] [[rythe_always_inline]] constexpr reference front() const;
        [[nodiscard]] [[rythe_always_inline]] constexpr reference back();
        [[nodiscard]] [[rythe_always_inline]] constexpr reference back() const;
        [[nodiscard]] [[rythe_always_inline]] constexpr reference at(size_type);
        [[nodiscard]] [[rythe_always_inline]] constexpr reference at(size_type) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr reference operator[](size_type);
        [[nodiscard]] [[rythe_always_inline]] constexpr reference operator[](size_type) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer data() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer data() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size_bytes() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;

        // negative count will assume size() - abs(count)
        [[nodiscard]] [[rythe_always_inline]] constexpr array_view subview(size_type offset, diff_type count = npos) const noexcept;

        [[rythe_always_inline]] constexpr void set_data(pointer data, size_type count) noexcept;
        [[rythe_always_inline]] constexpr void reset() noexcept;

    private:
        pointer m_src = nullptr;
        size_type m_count = 0; // the length of our view relative to m_position
    };

    template <typename It>
    array_view(It, size_type) -> array_view<iter_read_t<It>, It>;

    template <typename It>
    array_view(It, It) -> array_view<iter_read_t<It>, It>;

    template <typename T, size_type N>
    array_view(T (& )[N]) -> array_view<T>;

    template <typename T, contiguous_iterator Iter>
    array_view(array_view<T, Iter> other) -> array_view<const T, const_iterator<Iter>>;

    using string_view = rsl::array_view<const char>;
    using byte_view = rsl::array_view<const byte>;

    [[nodiscard]] [[rythe_always_inline]] consteval string_view operator""_sv(const cstring str, const size_type size) noexcept
    {
        return string_view::from_buffer(str, size);
    }

    template<typename StrType>
    constexpr string_view view_from_stringish(StrType&& str) noexcept
    {
        using string_type = remove_cvr_t<StrType>;
        if constexpr (is_same_v<string_type, string_view>)
        {
            return str;
        }
        else if constexpr (has_view_v<string_type, string_view()>)
        {
            return str.view();
        }
        else if constexpr (is_char_v<string_type>)
        {
            return string_view::from_value(str);
        }
        else
        {
            return string_view::from_string_length(str);
        }
    }

    // TODO(Glyn): Move these to their own header
    template <contiguous_container_like ArrayType, weakly_equality_comparable_with<container_value_type<ArrayType>> Comparable,
        convertible_to<container_value_type<ArrayType>> Replacement>
    [[rythe_always_inline]] constexpr size_type linear_search_and_replace(
            ArrayType& arr,
            const Comparable& key,
            const Replacement& replacement,
            size_type offset = 0,
            size_type endSearch = npos
            ) noexcept;

    // TODO(Rowan): The below functions check for any occurrence of any of the items in other in str, not for the sequence of other in
    // str. Is that intended?
    //				https://en.cppreference.com/w/cpp/string/basic_string_view/find_last_not_of.html see overload 1
    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_search_sequence(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0,
            size_type endSearch = npos
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search_sequence(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_search_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0,
            size_type endSearch = npos
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_search_outside_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search_outside_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type linear_search(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0,
            size_type endSearch = npos
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type linear_search_not_eq(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search_not_eq(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, typename Func>
    [[rythe_always_inline]] constexpr size_type linear_search_custom(
            array_view<T, Iter, ConstIter> arr,
            Func&& comparer,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, typename Func>
    [[rythe_always_inline]] constexpr size_type reverse_linear_search_custom(
            array_view<T, Iter, ConstIter> arr,
            Func&& comparer,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_count_sequence(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_count_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter,
        weakly_equality_comparable_with<T> C, contiguous_iterator CIter, contiguous_iterator CConstIter>
    [[rythe_always_inline]] constexpr size_type linear_count_outside_collection(
            array_view<T, Iter, ConstIter> arr,
            array_view<C, CIter, CConstIter> key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type linear_count(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0
            ) noexcept;

    template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter, weakly_equality_comparable_with<T> C>
    [[rythe_always_inline]] constexpr size_type linear_count_not_eq(
            array_view<T, Iter, ConstIter> arr,
            const C& key,
            size_type offset = 0
            ) noexcept;

    namespace internal
    {
        template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
        struct select_contiguous_view_impl
        {
            using contiguous_view = void;
        };


        template <typename T, contiguous_iterator Iter, contiguous_iterator ConstIter>
        struct select_contiguous_view_impl<T, Iter, ConstIter>
        {
            using contiguous_view = array_view<T, Iter, ConstIter>;
        };

        template <typename T, input_or_output_iterator<T> Iter, input_or_output_iterator<T> ConstIter>
        using select_contiguous_view = typename select_contiguous_view_impl<T, Iter, ConstIter>::contiguous_view;
    } // namespace internal

    template <
        typename T,
        input_or_output_iterator<T> Iter = T*,
        input_or_output_iterator<T> ConstIter = internal::select_const_iter<T, Iter>>
    struct iterator_view
    {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = add_const_t<T>*;
        using reference = T&;
        using const_reference = add_const_t<T>&;
        using iterator_type = Iter;
        using const_iterator_type = ConstIter;
        using reverse_iterator_type = reverse_iterator<iterator_type>;
        using const_reverse_iterator_type = reverse_iterator<const_iterator_type>;

        using const_view_type = conditional_t<is_const_v<T>, iterator_view, iterator_view<const value_type, const_iterator_type>>;
        using contiguous_view_type = internal::select_contiguous_view<T, iterator_type, const_iterator_type>;

        [[rythe_always_inline]] constexpr iterator_view() noexcept = default;
        [[rythe_always_inline]] constexpr iterator_view(iterator_type start, iterator_type end) noexcept;
        [[rythe_always_inline]] constexpr iterator_view(pointer ptr, size_type count) noexcept
            requires same_as<Iter, T*>;
        template <size_type N>
        [[rythe_always_inline]] constexpr iterator_view(value_type (& arr)[N]) noexcept
            requires same_as<Iter, T*>;
        [[rythe_always_inline]] constexpr iterator_view(const value_type& other) noexcept
            requires same_as<Iter, T*>;

        [[nodiscard]] [[rythe_always_inline]] constexpr operator iterator_view<const value_type, const_iterator_type>() noexcept
            requires(!is_const_v<T>);
        [[nodiscard]] [[rythe_always_inline]] constexpr operator contiguous_view_type() noexcept
            requires(contiguous_iterator<Iter> && contiguous_iterator<ConstIter>);

        [[nodiscard]] [[rythe_always_inline]] constexpr static iterator_view from_string_length(T* str, T terminator = T{}) noexcept
            requires(char_type<T> && same_as<Iter, T*>);

        [[rythe_always_inline]] constexpr iterator_view& operator=(const iterator_view&) = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const iterator_view& rhs);
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const iterator_view& rhs);

        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type begin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type begin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr iterator_type end() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type end() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_iterator_type cend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rbegin() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crbegin() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr reverse_iterator_type rend() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type rend() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_reverse_iterator_type crend() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr reference front();
        [[nodiscard]] [[rythe_always_inline]] constexpr reference front() const;
        [[nodiscard]] [[rythe_always_inline]] constexpr reference back();
        [[nodiscard]] [[rythe_always_inline]] constexpr reference back() const;

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;

    private:
        iterator_type m_start;
        iterator_type m_end;
    };

    template <has_view<any_type()> Container>
    [[nodiscard]] constexpr auto view(Container&& container) noexcept -> decltype(container.view())
    {
        return container.view();
    }

    template <typename Container>
        requires has_size<Container, size_type()> && has_data<Container, any_type()> && invert<has_view<Container, any_type()>>
    [[nodiscard]] constexpr auto view(Container&& container) noexcept
    {
        return array_view<container_value_type<Container>>::from_buffer(container.data(), container.size());
    }

    template <typename Container>
        requires has_begin<Container, any_type()> && has_end<Container, any_type()> && invert<has_data<Container, any_type()>>
    [[nodiscard]] constexpr auto view(Container&& container) noexcept
    {
        return iterator_view<container_value_type<Container>, container_iter_type<Container>>(container.begin(), container.end());
    }
} // namespace rsl

#include "views.inl"
