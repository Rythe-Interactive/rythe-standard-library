#pragma once

namespace rsl::filesystem
{
    constexpr char separator() noexcept
    {
        if constexpr (RYTHE_PLATFORM_INDEX == RYTHE_WINDOWS_INDEX)
        {
            return '\\';
        }
        else
        {
            return '/';
        }
    }

    constexpr char anti_separator() noexcept
    {
        if constexpr (RYTHE_PLATFORM_INDEX == RYTHE_WINDOWS_INDEX)
        {
            return '/';
        }
        else
        {
            return '\\';
        }
    }

    constexpr dynamic_string domain(const string_view path)
    {
        const size_type idx = linear_search(path, ':');
        if (idx == npos)
        {
            return dynamic_string{};
        }

        return dynamic_string::from_variadic_items(path.subview(0, idx + 1), separator(), separator());
    }

    constexpr string_view parent(const string_view path) noexcept
    {
        return path.subview(0ull, reverse_linear_search(path, separator_char{}, path.size() - 2ull));
    }

    constexpr string_view filename(const string_view path) noexcept
    {
        return path.subview(reverse_linear_search(path, separator_char{}));
    }

    constexpr string_view extension(const string_view path, const bool fullExtension) noexcept
    {
        const string_view fullname = filename(path);
        if (fullExtension)
        {
            return fullname.subview(linear_search(fullname, '.'));
        }

        return fullname.subview(reverse_linear_search(fullname, '.'));
    }

    constexpr string_view stem(const string_view path) noexcept
    {
        const string_view fullname = filename(path);
        return fullname.subview(0ull, linear_search(fullname, '.'));
    }

    constexpr dynamic_string subdir(const string_view path, const string_view sub)
    {
        const string_view sanitizedPath = trim_right(path, separator_char{});
        dynamic_string result;
        result.reserve(sanitizedPath.size() + sub.size() + 1ull);
        result.append(sanitizedPath);
        result.append(separator());
        result.append(sub);
        return result;
    }

    constexpr dynamic_string sanitize(const string_view path, const bool failOnFsLeave)
    {
        dynamic_array<string_view> recreation;

        dynamic_array<string_view> tokens = split_string_at(path, separator_char{});

        dynamic_string filesystem;
        for (const string_view& token : tokens)
        {
            //skip filesystem decl
            if (linear_search(token, ':') != npos)
            {
                filesystem = dynamic_string::from_variadic_items(token, separator(), separator());
                continue;
            }

            if (token.empty())
            {
                continue;
            }

            //handle upwards
            if (trim_right(token) == "..")
            {
                if (!recreation.empty() && recreation.back() != "..")
                {
                    recreation.pop_back();
                }
                else if (failOnFsLeave)
                {
                    //someone is trying something fishy! terminate parsing
                    return {};
                }
                else
                {
                    recreation.emplace_back("..");
                }
            }

            //handle normal tokens and .
            else if (trim_right(token) != ".")
            {
                recreation.push_back(token);
            }
        }

        return filesystem + join_strings_with(recreation, separator());
    }

    constexpr dynamic_string localize(const string_view path)
    {
        dynamic_string result = dynamic_string::from_view(path);
        localize(in_place_signal, result);
        return result;
    }

    constexpr dynamic_string standardize(const string_view path)
    {
        dynamic_string result = dynamic_string::from_view(path);
        standardize(in_place_signal, result);
        return result;
    }

    template <string_like StringType>
    constexpr void localize(in_place_signal_type, StringType& path)
    {
        linear_search_and_replace(path, anti_separator(), separator());
    }

    template <string_like StringType>
    constexpr void standardize(in_place_signal_type, StringType& path)
    {
        linear_search_and_replace(path, '\\', '/');
    }

    constexpr string_view strip_domain(const string_view path) noexcept
    {
        const size_type idx = linear_search_sequence(path, "://"_sv, 0ull, 64ull);
        if (idx == npos)
        {
            return path;
        }

        return path.subview(idx + 3ull);
    }

    constexpr dynamic_string replace_domain(const string_view path, const string_view replacement) noexcept
    {
        dynamic_string result = dynamic_string::from_view(path);
        replace_domain(in_place_signal, result, replacement);
        return result;
    }

    template <string_like StringType>
    constexpr void replace_domain(in_place_signal_type, StringType& path, const string_view replacement) noexcept
    {
        const size_type idx = linear_search_sequence(view(path), "://"_sv, 0ull, 64ull);
        if (idx == npos)
        {
            return;
        }

        path.replace(0ull, idx + 3ull, replacement);
    }
}
