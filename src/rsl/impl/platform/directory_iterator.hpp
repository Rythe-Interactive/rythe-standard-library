#pragma once
#include "../util/error_handling.hpp"

namespace rsl
{
    #if !defined(RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL)
    #define RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL void*
    #endif

    #if !defined(RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE)
    #define RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE nullptr
    #endif

    class directory_entry
    {
        friend class directory_iterator;
    public:
        [[rythe_always_inline]] result<dynamic_string> get_path() const;

    private:
        constexpr directory_entry(directory_iterator& iter) noexcept : m_directoryIterator(&iter) {}
        directory_iterator* m_directoryIterator;
    };

    class directory_iterator
    {
        using platform_specific_handle = RYTHE_DIRECTORY_ITERATOR_HANDLE_IMPL;

        platform_specific_handle m_handle = RYTHE_DIRECTORY_ITERATOR_HANDLE_DEFAULT_VALUE;

        friend class platform;
    public:
        using value_type = directory_entry;

        constexpr directory_iterator() noexcept = default;
        constexpr directory_iterator(directory_iterator&& other) noexcept;
        constexpr directory_iterator(const directory_iterator& other) noexcept;
        constexpr directory_iterator& operator=(directory_iterator&& other) noexcept;
        constexpr directory_iterator& operator=(const directory_iterator& other) noexcept;
        constexpr ~directory_iterator();

        result<dynamic_string> get_path() const;

        bool operator==(const directory_iterator& other) const;
        bool operator!=(const directory_iterator& other) const { return !(*this == other); }

        const directory_entry& operator*() const;
        const directory_entry* operator->() const;

        directory_iterator operator++(int);
        directory_iterator& operator++();

    private:
        directory_entry m_entry = directory_entry(*this);
    };

    static inline const directory_iterator directory_end = {};

    using directory_view = iterator_view<directory_iterator>;
}
