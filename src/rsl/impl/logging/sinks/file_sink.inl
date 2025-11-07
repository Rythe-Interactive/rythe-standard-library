#pragma once

namespace rsl::log
{
    inline file_sink::file_sink(const string_view fileName, const size_type maxFileCount) noexcept
        : m_fileName(dynamic_string::from_view(fileName)), m_maxFileCount(maxFileCount) {}

    constexpr void file_sink::set_file_name(const string_view fileName) noexcept
    {
        m_fileName = dynamic_string::from_view(fileName);
    }

    constexpr void file_sink::set_max_file_count(const size_type maxFileCount) noexcept
    {
        m_maxFileCount = maxFileCount;
    }
}
