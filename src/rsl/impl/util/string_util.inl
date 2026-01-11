#pragma once

namespace rsl
{
    constexpr bool is_white_space(const char c) noexcept
    {
        return c == white_space{};
    }

    template <weakly_equality_comparable_with<char> TokenType>
    constexpr dynamic_array<string_view> split_string_at(
            string_view str,
            const TokenType& token
            ) noexcept
    {
        dynamic_array<string_view> result;

        size_type start = 0;
        size_type end;
        while ((end = linear_search(str, token, start)) != npos)
        {
            result.push_back(str.subview(start, end - start));
            start = end + 1ull;
        }

        if (start < str.size())
        {
            result.push_back(str.subview(start, end - start));
        }

        return result;
    }

    template <char Token, char... Tokens>
    constexpr dynamic_array<string_view> split_string_at(const string_view str) noexcept
    {
        static constexpr char delimData[] = { Token, Tokens... };
        static constexpr string_view delimiters = delimData;

        dynamic_array<string_view> result;

        size_type start = 0;
        size_type end;
        while ((end = linear_search_collection(str, delimiters, start)) != npos)
        {
            result.push_back(str.subview(start, end - start));
            start = end + 1ull;
        }

        if (start < str.size())
        {
            result.push_back(str.subview(start, end - start));
        }

        return result;
    }

    template <contiguous_container_like Range>
    constexpr dynamic_string join_strings_with(const Range& elements, const string_view delimiters) noexcept
    {
        if (elements.empty())
        {
            return dynamic_string{};
        }

        size_type resultSize = elements.size() < 2ull ? 0ull : delimiters.size() * (elements.size() - 1ull);
        for (const auto& element : elements)
        {
            resultSize += view_from_stringish(element).size();
        }

        dynamic_string result = dynamic_string::create_reserved(resultSize);
        for (size_type i = 0; i < elements.size() - 1ull; i++)
        {
            result.append(view_from_stringish(elements[i]));
            result.append(delimiters);
        }
        result.append(elements.at(elements.size() - 1ull));

        return result;
    }

    template <const char* Delim, contiguous_container_like Range>
    constexpr dynamic_string join_strings_with(const Range& elements) noexcept
    {
        static constexpr string_view delimiters = string_view::from_string_length(Delim);
        return join_strings_with(elements, delimiters);
    }

    template <contiguous_container_like Range>
    constexpr dynamic_string join_strings_with(const Range& elements, const char delimiter) noexcept
    {
        return join_strings_with(elements, string_view::from_value(delimiter));
    }

    template <char Delimiter, contiguous_container_like Range>
    constexpr dynamic_string join_strings_with(const Range& elements) noexcept
    {
        return join_strings_with(elements, Delimiter);
    }

    constexpr string_view trim_left(const string_view str) noexcept
    {
        return trim_left(str, white_space{});
    }

    constexpr string_view trim_right(const string_view str) noexcept
    {
        return trim_right(str, white_space{});
    }

    constexpr string_view trim(const string_view str) noexcept
    {
        return trim_left(trim_right(str));
    }

    template <weakly_equality_comparable_with<char> C>
    constexpr string_view trim_left(const string_view str, const C& key) noexcept
    {
        const size_type offset = linear_search_not_eq(str, key);
        if (offset == npos)
        {
            return {};
        }

        return str.subview(offset);
    }

    template <weakly_equality_comparable_with<char> C>
    constexpr string_view trim_right(const string_view str, const C& key) noexcept
    {
        const size_type offset = reverse_linear_search_not_eq(str, key);
        if (offset == npos)
        {
            return {};
        }

        return str.subview(0ull, -static_cast<diff_type>(offset));
    }

    template <weakly_equality_comparable_with<char> C>
    constexpr string_view trim(const string_view str, const C& key) noexcept
    {
        return trim_left(trim_right(str, key), key);
    }

    template <invocable<bool(char)> Func>
    constexpr string_view trim_left(const string_view str, Func&& comparer) noexcept
    {
        const size_type offset = linear_search_custom(str, [&](const char c) { return !comparer(c); });
        if (offset == npos)
        {
            return {};
        }

        return str.subview(offset);
    }

    template <invocable<bool(char)> Func>
    constexpr string_view trim_right(const string_view str, Func&& comparer) noexcept
    {
        const size_type offset = reverse_linear_search_custom(str, [&](const char c) { return !comparer(c); });
        if (offset == npos)
        {
            return {};
        }

        return str.subview(0ull, -offset);
    }

    template <invocable<bool(char)> Func>
    constexpr string_view trim(const string_view str, Func&& comparer) noexcept
    {
        return trim_left(trim_right(str, comparer), comparer);
    }
}
