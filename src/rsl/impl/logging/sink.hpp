#pragma once
#include "formatter.hpp"
#include "severity.hpp"

namespace rsl::log
{
    struct message;

    class sink
    {
    public:
        constexpr sink() noexcept = default;
        constexpr sink(const sink&) noexcept = default;
        constexpr sink& operator=(const sink&) noexcept = default;

        virtual ~sink() = default;

        virtual void log(formatter& formatter, const message& msg) = 0;
        virtual void flush() = 0;

        [[rythe_always_inline]] void filter(severity s) noexcept;
        [[nodiscard]] [[rythe_always_inline]] severity filter_severity() const noexcept;
    private:
        severity m_severity = severity::default_severity;
    };
}

#include "sink.inl"
