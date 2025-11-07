#pragma once
#include "../sink.hpp"

namespace rsl::log
{
    class file_sink : public sink
    {
    public:
        using sink::sink;
        [[rythe_always_inline]] file_sink(string_view fileName, size_type maxFileCount = npos) noexcept;

        [[rythe_always_inline]] constexpr void set_file_name(string_view fileName) noexcept;
        [[rythe_always_inline]] constexpr void set_max_file_count(size_type maxFileCount) noexcept;

        void log([[maybe_unused]] formatter& formatter, [[maybe_unused]] const message& msg) override{}
        void flush() override {}

    private:
        dynamic_string m_fileName = "logs/rythe.log"_ds;
        size_type m_maxFileCount = npos;
    };
}

#include "file_sink.inl"
