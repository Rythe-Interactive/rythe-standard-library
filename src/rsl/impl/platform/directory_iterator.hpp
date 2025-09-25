#pragma once
#include "../util/error_handling.hpp"

#include "file.hpp"

namespace rsl
{
    // Exists purely for iterator semantics of directory_iterator, do not store!
    class directory_entry
    {
        friend class directory_iterator;

    public:
        [[nodiscard]] [[rythe_always_inline]] dynamic_string get_path() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_regular_file() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_directory() const;
        [[nodiscard]] [[rythe_always_inline]] bool is_writable() const;

        [[nodiscard]] [[rythe_always_inline]] result<file> open_file(
                file_access_mode mode,
                file_access_flags flags = file_access_flags::no_preference
                ) const;

    private:
        constexpr directory_entry(directory_iterator& iter) noexcept : m_directoryIterator(&iter) {}
        directory_iterator* m_directoryIterator;
    };

    DECLARE_NATIVE_API_TYPE(directory_iterator)

    class directory_iterator
    {
    public:
        using value_type = directory_entry;

        constexpr directory_iterator() noexcept = default;
        directory_iterator(directory_iterator&& other) noexcept;
        directory_iterator(const directory_iterator& other) noexcept;
        directory_iterator& operator=(directory_iterator&& other) noexcept;
        directory_iterator& operator=(const directory_iterator& other) noexcept;
        ~directory_iterator();

        [[nodiscard]] dynamic_string get_path() const;
        [[nodiscard]] bool is_regular_file() const;
        [[nodiscard]] bool is_directory() const;
        [[nodiscard]] bool is_writable() const;

        [[nodiscard]] result<file> open_file(file_access_mode mode, file_access_flags flags = file_access_flags::no_preference) const;

        bool operator==(const directory_iterator& other) const;
        bool operator!=(const directory_iterator& other) const { return !(*this == other); }

        const directory_entry& operator*() const;
        const directory_entry* operator->() const;

        directory_iterator operator++(int);
        directory_iterator& operator++();

    private:
        NATIVE_API_TYPE_PRIVATE_MEMBERS(directory_iterator)

        directory_entry m_entry = directory_entry(*this);
    };

    static inline const directory_iterator directory_end = {};

    using directory_view = iterator_view<directory_iterator>;
}
