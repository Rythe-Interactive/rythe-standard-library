#include "formatter.hpp"

#include "../platform/platform.hpp"
#include "../time/stopwatch.hpp"

#include "message.hpp"

#include <fmt/chrono.h>

namespace rsl::log
{
    namespace
    {
        class forwarding_formatter final : public flag_formatter
        {
        public:
            forwarding_formatter(const string_view section) noexcept : patternSection(section) {}

            string_view patternSection;

            void format(
                    [[maybe_unused]] const message& msg,
                    [[maybe_unused]] const tm::point32 time,
                    fmt::memory_buffer& dest
                    ) override
            {
                dest.append(patternSection.data(), patternSection.data() + patternSection.size());
            }
        };

        void append_string(fmt::memory_buffer& dest, const string_view str)
        {
            dest.append(str.data(), str.data() + str.size());
        }
    }

    void undecorated_formatter::format(const message& msg, fmt::memory_buffer& dest)
    {
        fmt::vformat_to(fmt::appender(dest), fmt::string_view(msg.msg.data(), msg.msg.size()), msg.formatArgs);
    }

    void pattern_formatter::format(const message& msg, fmt::memory_buffer& dest)
    {
        const tm::point32 time = tm::main_clock.current_point();

        for (auto& formatter : m_formatters)
        {
            formatter->format(msg, time, dest);
        }
    }

    void pattern_formatter::compile_pattern(array_view<temporary_object<flag_formatter>> flagFormatters)
    {
        m_formatters.clear();

        size_type formatterIndex = 0ull;

        string_view forwardingSection;
        char const* const end = m_pattern.end();
        for (const char* character = m_pattern.begin(); character != end; ++character)
        {
            if (*character == '{')
            {
                if (!forwardingSection.empty())
                {
                    m_formatters.emplace_back(
                            unique_object<forwarding_formatter>::create_in_place(forwardingSection)
                            );
                    forwardingSection.reset();
                }

                if (character != end)
                {
                    // TODO(Glyn): handle indexed patterns: "{1} {0}: my message"
                    unique_object<flag_formatter>& formatter = m_formatters.emplace_back(rsl::move(flagFormatters[formatterIndex++]));
                    ++character;

                    if (*character != '}')
                    {
                        const char* start = character - 1;
                        while (*character != '}')
                        {
                            ++character;
                        }
                        formatter->set_flag_options(string_view::from_buffer(start, static_cast<size_type>(character - start) + 1ull));
                    }
                }
                else
                {
                    break;
                }
            }
            else // chars not following the { sign should be displayed as is
            {
                if (forwardingSection.empty())
                {
                    forwardingSection.set_data(character, 1ull);
                }
                else
                {
                    forwardingSection.set_data(forwardingSection.data(), forwardingSection.size() + 1ull);
                }
            }
        }

        if (!forwardingSection.empty())
        {
            m_formatters.emplace_back(
                    unique_object<forwarding_formatter>::create_in_place(forwardingSection)
                    );
            forwardingSection.reset();
        }
    }

    void message_flag_formatter::format(const message& msg, [[maybe_unused]] const tm::point32 time, fmt::memory_buffer& dest)
    {
        fmt::vformat_to(fmt::appender(dest), fmt::string_view(msg.msg.data(), msg.msg.size()), msg.formatArgs);
    }

    void severity_flag_formatter::format(const message& msg, [[maybe_unused]] const tm::point32 time, fmt::memory_buffer& dest)
    {
        // TODO(Glyn): severity colors
        switch (m_mode)
        {
            case 'l':
                append_string(dest, severity_name(msg.severity));
                return;
            case 'L':
                append_string(dest, to_upper(severity_name(msg.severity)));
                return;
            case 's':
                append_string(dest, severity_short_name(msg.severity));
                return;
            case 'S':
                append_string(dest, to_upper(severity_short_name(msg.severity)));
                return;
            default:
                break;
        }
        rsl_assert_unreachable();
    }

    void severity_flag_formatter::set_flag_options(const string_view options)
    {
        if (options.size() == 2ull)
        {
            return;
        }

        rsl_assert_invalid_pattern(linear_count_not_eq(options.subview(1, -1), white_space{}) == 1ull);
        m_mode = options[1];
    }

    void genesis_flag_formatter::format([[maybe_unused]] const message& msg, const tm::point32 time, fmt::memory_buffer& dest)
    {
        size_type seconds = static_cast<size_type>((time - tm::genesis).seconds()); // TODO(Glyn): Underlying time should really not be floating point!

        ::tm t{};
        t.tm_sec = static_cast<int>(seconds % 60ull);
        t.tm_min = static_cast<int>((seconds / 60ull) % 60ull);
        t.tm_hour = static_cast<int>(seconds / 3600ull);

        fmt::format_to(
                fmt::appender(dest),
                fmt::runtime(fmt::string_view(m_options.data(), m_options.size())),
                t
            );
    }

    void genesis_flag_formatter::set_flag_options(const string_view options)
    {
        m_options = options;
    }

    void logger_name_flag_formatter::format(const message& msg, [[maybe_unused]] const tm::point32 time, fmt::memory_buffer& dest)
    {
        append_string(dest, msg.loggerName);
    }

    void thread_name_flag_formatter::format(const message& msg, [[maybe_unused]] const tm::point32 time, fmt::memory_buffer& dest)
    {
        append_string(dest, platform::get_thread_name(msg.threadId));
    }
} // namespace rsl
