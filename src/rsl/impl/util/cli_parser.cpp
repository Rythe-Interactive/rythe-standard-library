#include "cli_parser.hpp"

namespace rsl
{
    namespace internal
    {
        static string_view trim_leading_dashes(const string_view name) noexcept
        {
            const size_type pos = linear_search_not_eq(name, '-');
            return pos != npos ? name.subview(pos) : name;
        }

        static bool is_number([[maybe_unused]] const string_view arg) noexcept
        {
            // TODO(Glyn): from_chars implementation
            return false;
        }

        static bool is_option(const string_view arg) noexcept
        {
            rsl_assert_invalid_parameters(!arg.is_empty());
            if (is_number(arg))
            {
                return false;
            }
            return '-' == arg[0];
        }
    } // namespace internal

    void cli_parser::set_command_usage(const string_view usage)
    {
        m_commandUsage = usage;
    }

    void cli_parser::add_param(const string_view name, const bool isFlag, const string_view usagePattern)
    {
        m_registeredParams.emplace(
                internal::trim_leading_dashes(name),
                param_info{
                        .isFlag = isFlag,
                        .usagePattern = usagePattern,
                });
    }

    void cli_parser::add_param(const array_view<const string_view> aliases, const bool isFlag, const string_view usagePattern)
    {
        for (const string_view name : aliases)
        {
            add_param(name, isFlag, usagePattern);
        }
    }

    void cli_parser::parse(const int argc, const char* const argv[])
    {
        m_flags.clear();
        m_params.clear();
        m_posArgs.clear();

        m_args.resize(static_cast<size_type>(argc));
        for (size_type i = 0ull; i < m_args.size(); ++i)
        {
            m_args[i] = string_view::from_string_length(argv[i]);
        }

        for (size_type i = 1ull; i < m_args.size(); ++i)
        {
            if (!internal::is_option(m_args[i]))
            {
                m_posArgs.emplace_back(m_args[i]);
                continue;
            }

            const string_view name = internal::trim_leading_dashes(m_args[i]);

            const size_type equalPos = linear_search(name, '=');
            if (equalPos != npos)
            {
                m_params.emplace(name.subview(0, narrowing_cast<diff_type>(equalPos)), name.subview(equalPos + 1));
                continue;
            }

            // any potential option will get as its value the next arg, unless that arg is an option too
            // in that case it will be determined a flag.
            if (i == m_args.size() - 1 || internal::is_option(m_args[i + 1]))
            {
                m_flags.insert(name);
                continue;
            }

            if (is_param(name))
            {
                // "--param value" instead of "--param=value"
                m_params.emplace(name, m_args[i + 1]);
                ++i;
                continue;
            }

            m_flags.insert(name);
        }
    }

    bool cli_parser::has_flag(const string_view name) const noexcept
    {
        return m_flags.contains(internal::trim_leading_dashes(name));
    }

    bool cli_parser::has_flag(const array_view<const string_view> aliases) const noexcept
    {
        for (const string_view alias : aliases)
        {
            if (has_flag(alias))
            {
                return true;
            }
        }
        return false;
    }

    string_view cli_parser::get_param(const string_view name) const noexcept
    {
        args_view view = get_params(name);
        return view.is_empty() ? string_view{} : view[0ull];
    }

    string_view cli_parser::get_param(const array_view<const string_view> aliases) const noexcept
    {
        args_view view = get_params(aliases);
        return view.is_empty() ? string_view{} : view[0ull];
    }

    cli_parser::args_view cli_parser::get_params(const string_view name) const noexcept
    {
        const args_container* values = m_params.find(internal::trim_leading_dashes(name));
        if (values && !values->is_empty())
        {
            return values->view();
        }

        return {};
    }

    cli_parser::args_view cli_parser::get_params(const array_view<const string_view> aliases) const noexcept
    {
        for (string_view name : aliases)
        {
            const args_container* values = m_params.find(internal::trim_leading_dashes(name));
            if (values && !values->is_empty())
            {
                return values->view();
            }
        }

        return {};
    }

    void cli_parser::print_current_command(rsl::log::severity severity, pointer<rsl::log::logger> logger) const
    {
        if (logger == nullptr)
        {
            logger = rsl::get_logging_context().undecoratedLogger;
        }

        for (size_type i = 0ull; i < m_args.size(); ++i)
        {
            if (i == m_args.size() - 1ull) [[unlikely]]
            {
                logger->logln(severity, "{}\n", m_args[i]);
            }
            else
            {
                logger->log(severity, "{} ", m_args[i]);
            }
        }
    }

    void cli_parser::print_usage(const rsl::log::severity severity, pointer<rsl::log::logger> logger) const
    {
        if (logger == nullptr)
        {
            logger = rsl::get_logging_context().undecoratedLogger;
        }

        logger->logln(severity, "{}", m_commandUsage);
        for (const auto& [param, info] : m_registeredParams)
        {
            logger->logln(severity, rsl::log::runtime_format(info.usagePattern), param);
        }
    }

    bool cli_parser::is_param(const string_view name) const noexcept
    {
        const param_info* info = m_registeredParams.find(name);
        return info && !info->isFlag;
    }
} // namespace rsl
