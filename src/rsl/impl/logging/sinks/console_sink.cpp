#include "console_sink.hpp"

namespace rsl
{

    void log::console_sink::log(formatter& formatter, const message& msg)
    {
        formatter.format(msg, m_messageBuffer);
    }

    void log::console_sink::flush()
    {
        platform::append_file(m_stdout, byte_view::from_buffer(reinterpret_cast<byte*>(m_messageBuffer.data()), m_messageBuffer.size())).report_errors_and_resolve();
        m_messageBuffer.clear();
    }
}
