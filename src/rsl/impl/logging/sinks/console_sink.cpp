#include "console_sink.hpp"

namespace rsl
{

    void log::console_sink::log(formatter& formatter, const message& msg)
    {
        fmt::memory_buffer messageBuffer;
        formatter.format(msg, messageBuffer);

        m_stdout.write(byte_view::from_buffer(messageBuffer.data(), messageBuffer.size())).report_errors_and_resolve();
    }

    void log::console_sink::flush()
    {
        m_stdout.flush().report_errors_and_resolve();
    }
}
