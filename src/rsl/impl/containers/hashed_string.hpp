#pragma once
#include "../memory/allocator.hpp"
#include "../util/hash.hpp"
#include "../util/primitives.hpp"

#include "string.hpp"
#include "views.hpp"

namespace rsl
{
    template <char_type CharType>
    struct basic_hashed_string_view
    {
        using string_view_type = rsl::array_view<CharType>;
        using value_type = string_view_type::value_type;
        using pointer = string_view_type::pointer;
        using const_pointer = string_view_type::const_pointer;
        using reference = string_view_type::reference;
        using iterator_type = string_view_type::iterator_type;
        using const_iterator_type = string_view_type::const_iterator_type;
        using reverse_iterator_type = string_view_type::reverse_iterator_type;
        using const_reverse_iterator_type = string_view_type::const_reverse_iterator_type;

        using const_view_type = basic_hashed_string_view<typename string_view_type::const_view_type::value_type>;

        string_view_type string;
        id_type hash = invalid_id;

        [[rythe_always_inline]] constexpr basic_hashed_string_view() noexcept = default;
        [[rythe_always_inline]] constexpr basic_hashed_string_view(const basic_hashed_string_view& other) noexcept = default;

        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string_view from_value(CharType& src) noexcept;
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string_view from_array(CharType (& arr)[N]) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string_view from_buffer(
                const value_type* ptr,
                size_type count
                ) noexcept;
        template <contiguous_iterator It>
        [[rythe_always_inline]] constexpr static basic_hashed_string_view from_iterator_pair(
                It first,
                It last
                ) noexcept(iter_noexcept_deref<It> && iter_noexcept_diff<It>)
            requires same_as<iter_pointer_t<It>, CharType*>;
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string_view from_view(string_view_type src) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string_view from_string_length(
                CharType* str,
                CharType terminator = CharType{}
                ) noexcept;

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
        [[nodiscard]] [[rythe_always_inline]] constexpr reference at(size_type index);
        [[nodiscard]] [[rythe_always_inline]] constexpr reference at(size_type index) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr reference operator[](size_type index);
        [[nodiscard]] [[rythe_always_inline]] constexpr reference operator[](size_type index) const;
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer data() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer data() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size_bytes() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr string_view_type view() const noexcept;
    };

    using hashed_string_view = basic_hashed_string_view<const char>;

    inline namespace literals
    {
        consteval hashed_string_view operator""_hsv(const cstring str, const size_type size) noexcept
        {
            return hashed_string_view::from_buffer(str, size);
        }
    }

    template <char_type CharType = char, allocator_type Alloc = default_allocator, size_type StaticCapacity = 0ull>
    class basic_hashed_string
    {
    public:
        using string_type = basic_dynamic_string<CharType, Alloc, StaticCapacity>;
        using value_type = typename string_type::value_type;
        using iterator_type = typename string_type::iterator_type;
        using const_iterator_type = typename string_type::const_iterator_type;
        using reverse_iterator_type = typename string_type::reverse_iterator_type;
        using const_reverse_iterator_type = typename string_type::const_reverse_iterator_type;
        using view_type = typename string_type::view_type;
        using hashed_view_type = basic_hashed_string_view<CharType>;
        using const_view_type = typename string_type::const_view_type;
        using const_hashed_view_type = basic_hashed_string_view<const CharType>;
        using allocator_storage_type = typename string_type::allocator_storage_type;
        using allocator_t = typename string_type::allocator_t;
        using factory_storage_type = typename string_type::factory_storage_type;
        using factory_t = typename string_type::factory_t;

        constexpr static bool use_post_fix = string_type::use_post_fix;
        constexpr static size_type static_capacity = string_type::static_capacity;
        constexpr static bool can_allocate = string_type::can_allocate;
        constexpr static bool can_resize = string_type::can_resize;

        string_type string;
        id_type hash = invalid_id;

        [[rythe_always_inline]] constexpr basic_hashed_string() noexcept(is_nothrow_constructible_v<string_type>) = default;

        [[rythe_always_inline]] explicit constexpr basic_hashed_string(
                const string_type& str
                ) noexcept(is_nothrow_copy_constructible_v<string_type>)
            : string(str),
              hash(hash_string(str)) {}

        [[rythe_always_inline]] explicit constexpr basic_hashed_string(
                string_type&& str
                ) noexcept(is_nothrow_move_constructible_v<string_type>)
            : string(rsl::move(str)),
              hash(hash_string(str)) {}

        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_value(value_type& src) noexcept(noexcept(string_type::from_value(src)));
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_array(
                const value_type (& arr)[N]
                )
        noexcept(noexcept(string_type::from_array(arr)));
        template <size_type N>
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_array(
                value_type (&& arr)[N]
                ) noexcept(noexcept(string_type::from_array(rsl::move(arr))));
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_buffer(
                const value_type* ptr,
                size_type count
                ) noexcept(noexcept(string_type::from_buffer(nullptr, size_type{ 0 })));
        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_view(
                const_view_type src
                ) noexcept(noexcept(string_type::from_view(const_view_type{})));

        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string from_string_length(
                const CharType* str,
                CharType terminator = CharType{}
                ) noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr static basic_hashed_string create_reserved(size_type capacity) noexcept
            requires(can_allocate);

        [[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr CharType& at(size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const CharType& at(size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator[](size_type i) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator[](size_type i) const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type* data() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type* data() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr hashed_view_type view() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const_hashed_view_type view() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator view_type() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr operator const_view_type() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& front() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& front() const noexcept;

        [[nodiscard]] [[rythe_always_inline]] constexpr value_type& back() noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr const value_type& back() const noexcept;

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
    };

    using hashed_string = basic_hashed_string<>;

    inline namespace literals
    {
        constexpr hashed_string operator""_hs(const cstring str, const size_type size) noexcept
        {
            return hashed_string::from_buffer(str, size);
        }
    }
} // namespace rsl

#include "hashed_string.inl"
