#pragma once
#include "array.hpp"

namespace rsl
{
    // TODO(Rowan): constexpr strings are still useful if the string never leaks from constant evaluation time to runtime. constexpr_string is needed if you want to be able to read the results of a string at runtime.
    template <char_type CharType = char, allocator_type Alloc = default_allocator, size_type StaticCapacity = 0ull>
    class basic_dynamic_string final
            : public contiguous_container_base<CharType, Alloc, default_factory<CharType>, CharType*, const CharType*,
                                               contiguous_container_info<
                                                   true, StaticCapacity, !is_same_v<Alloc, mock_allocator>, true>>
    {
    public:
        using container_base = contiguous_container_base<
            CharType, Alloc, default_factory<CharType>, CharType*, const CharType*, contiguous_container_info<
                true, StaticCapacity, !is_same_v<Alloc, mock_allocator>, true>>;
        using mem_rsc = typename container_base::mem_rsc;
        using value_type = CharType;
        using iterator_type = typename container_base::iterator_type;
        using const_iterator_type = typename container_base::const_iterator_type;
        using reverse_iterator_type = typename container_base::reverse_iterator_type;
        using const_reverse_iterator_type = typename container_base::const_reverse_iterator_type;
        using view_type = typename container_base::view_type;
        using const_view_type = typename container_base::const_view_type;
        using allocator_storage_type = allocator_storage<Alloc>;
        using allocator_t = Alloc;
        using factory_storage_type = factory_storage<default_factory<CharType>>;
        using factory_t = default_factory<CharType>;

        using contiguous_container_base<
            CharType, Alloc, default_factory<CharType>, CharType*, const CharType*, contiguous_container_info<
                true, StaticCapacity, !is_same_v<Alloc, mock_allocator>, true>>::contiguous_container_base;

        [[rythe_always_inline]] constexpr basic_dynamic_string(
                const container_base& src
                )
            noexcept(container_base::copy_construct_container_noexcept);
        [[rythe_always_inline]] constexpr basic_dynamic_string(
                container_base&& src
                ) noexcept(container_base::move_construct_container_noexcept);

        using container_base::operator view_type;
        using container_base::operator const_view_type;

        using container_base::operator[];
        using container_base::operator=;

        [[rythe_always_inline]] constexpr basic_dynamic_string& operator+=(const_view_type rhs);
        [[rythe_always_inline]] constexpr basic_dynamic_string& operator+=(CharType rhs);
    };

    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    [[nodiscard]] [[rythe_always_inline]] constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity> operator+(
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& lhs,
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& rhs
            );
    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    [[nodiscard]] [[rythe_always_inline]] constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity> operator+(
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& lhs,
            typename basic_dynamic_string<CharType, Alloc, StaticCapacity>::const_view_type rhs
            );
    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    [[nodiscard]] [[rythe_always_inline]] constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity> operator+(
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& lhs,
            CharType rhs
            );
    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    [[nodiscard]] [[rythe_always_inline]] constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity> operator+(
            typename basic_dynamic_string<CharType, Alloc, StaticCapacity>::const_view_type lhs,
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& rhs
            );
    template <char_type CharType, allocator_type Alloc, size_type StaticCapacity>
    [[nodiscard]] [[rythe_always_inline]] constexpr basic_dynamic_string<CharType, Alloc, StaticCapacity> operator+(
            CharType lhs,
            const basic_dynamic_string<CharType, Alloc, StaticCapacity>& rhs
            );

    using dynamic_string = basic_dynamic_string<>;

    template <size_type StaticCapacity, allocator_type Alloc = default_allocator>
    using hybrid_string = basic_dynamic_string<char, Alloc, StaticCapacity>;

    template <size_type StaticCapacity>
    using static_string = basic_dynamic_string<char, mock_allocator, StaticCapacity>;

    using dynamic_wstring = basic_dynamic_string<utf16>;

    template <size_type StaticCapacity, allocator_type Alloc = default_allocator>
    using hybrid_wstring = basic_dynamic_string<utf16, Alloc, StaticCapacity>;

    template <size_type StaticCapacity>
    using static_wstring = basic_dynamic_string<utf16, mock_allocator, StaticCapacity>;

    [[nodiscard]] dynamic_wstring to_utf16(dynamic_string::const_view_type str);

    // Returns empty string upon encoding error and succeeded will be set to false, otherwise true.
    [[nodiscard]] dynamic_string to_utf8(dynamic_wstring::const_view_type str, bool& succeeded);
    // Returns empty string upon encoding error.
    [[nodiscard]] dynamic_string to_utf8(dynamic_wstring::const_view_type str);

    [[nodiscard]] [[rythe_always_inline]] constexpr char to_upper(char ch);
    [[nodiscard]] [[rythe_always_inline]] constexpr char to_lower(char ch);
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string to_upper(dynamic_string::const_view_type str);
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string to_lower(dynamic_string::const_view_type str);

    using static_string64 = static_string<64>;
    using static_string128 = static_string<128>;
    using static_string256 = static_string<256>;
    using static_string512 = static_string<512>;

    struct white_space
    {
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const white_space) const noexcept { return true; }
        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const white_space) const noexcept { return false; }

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const char ch) const noexcept
        {
            return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v';
        }

        [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const char ch) const noexcept
        {
            return !(*this == ch);
        }
    };

    [[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const char ch, white_space) noexcept
    {
        return white_space{} == ch;
    }

    [[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const char ch, white_space) noexcept
    {
        return white_space{} != ch;
    }

    template <typename T>
    [[nodiscard]] [[rythe_always_inline]] dynamic_string to_string(const T& value);

    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string operator""_ds(const cstring str, const size_type size) noexcept
    {
        return dynamic_string::from_buffer(str, size);
    }
} // namespace rsl

#include "string.inl"
