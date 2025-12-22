#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include "comparers.hpp" // used in .inl

namespace rsl
{
    template <typename T, typename Comparer>
    constexpr void insertion_sort(T* start, size_type count, const Comparer& comparer) noexcept;
    template <typename T, typename Comparer>
    [[rythe_always_inline]] constexpr void insertion_sort(T* start, T* end, const Comparer& comparer) noexcept;

    template <typename T>
    [[rythe_always_inline]] constexpr void insertion_sort(T* start, size_type count) noexcept;
    template <typename T>
    [[rythe_always_inline]] constexpr void insertion_sort(T* start, T* end) noexcept;

    template <typename T, typename Comparer>
    [[rythe_always_inline]] constexpr void quick_sort(T* start, size_type count, const Comparer& comparer) noexcept;
    template <typename T, typename Comparer>
    constexpr void quick_sort(T* start, T* end, const Comparer& comparer) noexcept;

    template <typename T>
    [[rythe_always_inline]] constexpr void quick_sort(T* start, size_type count) noexcept;
    template <typename T>
    [[rythe_always_inline]] constexpr void quick_sort(T* start, T* end) noexcept;

    template <typename T, typename Comparer, size_type InsertionSortCeiling>
    [[rythe_always_inline]] constexpr void hybrid_sort(T* start, size_type count, const Comparer& comparer) noexcept;
}

#include "sort.inl"
