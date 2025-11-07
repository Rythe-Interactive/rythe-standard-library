#pragma once
#include "../sink.hpp"

#include "../../platform/platform.hpp"

namespace rsl::log
{
    class console_sink : public sink
    {
    public:
        void log(formatter& formatter, const message& msg) override;
        void flush() override;

    private:
        file m_stdout = platform::open_file("stdout", file_access_mode::append).value();
    };
}
