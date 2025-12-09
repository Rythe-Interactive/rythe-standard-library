#pragma once
#include "../sink.hpp"

#include "../../platform/platform.hpp"
#include "../../platform/platform_dependent_var.hpp"

namespace rsl::log
{
    class console_sink : public sink
    {
    public:
        void log(formatter& formatter, const message& msg) override;
        void flush() override;

    private:
        fmt::memory_buffer m_messageBuffer;
        file m_stdout = platform::open_file(platform_dependent_var(windows_var("CONOUT$"_sv), linux_var("stdout"_sv)).get(), file_access_mode::write).value();
    };
}
