#pragma once

namespace rsl
{
    template <typename T, typename Comparer>
    constexpr void insertion_sort(T* start, const size_type count, const Comparer& comparer) noexcept
    {
        for (size_type i = 1ull; i < count; ++i)
        {
            const T item = start[i];
            size_type j = i;
            while (comparer(item, start[j - 1ull]) && (j > 0ull))
            {
                start[j] = start[j - 1ull];
                --j;
            }
            start[j] = item;
        }
    }

    template <typename T, typename Comparer>
    constexpr void insertion_sort(T* start, T* end, const Comparer& comparer) noexcept
    {
        insertion_sort(start, static_cast<size_type>(end - start), comparer);
    }

    template <typename T>
    constexpr void insertion_sort(T* start, const size_type count) noexcept
    {
        insertion_sort(start, count, less<T>{});
    }

    template <typename T>
    constexpr void insertion_sort(T* start, T* end) noexcept
    {
        insertion_sort(start, static_cast<size_type>(end - start), less<T>{});
    }

    template <typename T, typename Comparer>
    constexpr void quick_sort(T* start, const size_type count, const Comparer& comparer) noexcept
    {
        quick_sort(start, start + count, comparer);
    }

    template <typename T, typename Comparer>
    constexpr void quick_sort(T* start, T* end, const Comparer& comparer) noexcept
    {
        const T pivot = *(start + (end - start) / 2ull);

        T* left = start;
        T* right = end - 1ull;

        while (left <= right)
        {
            while (comparer(*left, pivot))
            {
                ++left;
            }
            while (comparer(pivot, *right))
            {
                --right;
            }

            if (left <= right)
            {
                const T temp = rsl::move(*left);
                *left++ = rsl::move(*right);
                *right-- = rsl::move(temp);
            }
        }

        quick_sort(start, right + 1u, comparer);
        quick_sort(left, end, comparer);
    }

    template <typename T>
    constexpr void quick_sort(T* start, const size_type count) noexcept
    {
        quick_sort(start, start + count, less<T>{});
    }

    template <typename T>
    constexpr void quick_sort(T* start, T* end) noexcept
    {
        quick_sort(start, end, less<T>{});
    }

    template <size_type InsertionSortCeiling, typename T, typename Comparer>
    constexpr void hybrid_sort(T* start, const size_type count, const Comparer& comparer) noexcept
    {
        if (count <= InsertionSortCeiling)
        {
            insertion_sort(start, count, comparer);
            return;
        }

        quick_sort(start, start + count, comparer);
    }

    template <size_type InsertionSortCeiling, typename T, typename Comparer>
    constexpr void hybrid_sort(T* start, T* end, const Comparer& comparer) noexcept
    {
        hybrid_sort<InsertionSortCeiling>(start, end - start, comparer);
    }

    template <size_type InsertionSortCeiling, typename T>
    constexpr void hybrid_sort(T* start, const size_type count) noexcept
    {
        hybrid_sort<InsertionSortCeiling>(start, count, less<T>{});
    }

    template <size_type InsertionSortCeiling, typename T>
    constexpr void hybrid_sort(T* start, T* end) noexcept
    {
        hybrid_sort<InsertionSortCeiling>(start, end - start, less<T>{});
    }

    template <typename T, typename Comparer>
    constexpr void hybrid_sort(T* start, const size_type count, const Comparer& comparer) noexcept
    {
        hybrid_sort<default_insertion_sort_ceiling>(start, count, comparer);
    }

    template <typename T, typename Comparer>
    constexpr void hybrid_sort(T* start, T* end, const Comparer& comparer) noexcept
    {
        hybrid_sort<default_insertion_sort_ceiling>(start, end - start, comparer);
    }

    template <typename T>
    constexpr void hybrid_sort(T* start, const size_type count) noexcept
    {
        hybrid_sort<default_insertion_sort_ceiling>(start, count, less<T>{});
    }

    template <typename T>
    constexpr void hybrid_sort(T* start, T* end) noexcept
    {
        hybrid_sort<default_insertion_sort_ceiling>(start, end - start, less<T>{});
    }
}
