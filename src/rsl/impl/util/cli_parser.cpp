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

        static bool is_number([[maybe_unused]] const string_view arg)
        {
            // TODO(Glyn): from_chars implementation
            return false;
        }

        static bool is_option(const string_view arg)
        {
            rsl_assert_invalid_parameters(!arg.empty());
            if (is_number(arg))
            {
                return false;
            }
            return '-' == arg[0];
        }
    }

    void cli_parser::add_param(const string_view name)
    {
        m_registeredParams.insert(internal::trim_leading_dashes(name));
    }

    void cli_parser::parse(const int argc, const char* const argv[])
    {
        // clear out possible previous parsing remnants
        m_flags.clear();
        m_params.clear();
        m_posArgs.clear();

        // convert to strings
        m_args.resize(static_cast<size_type>(argc));
        for (size_type i = 0ull; i < m_args.size(); ++i)
        {
            m_args[i].assign(string_view::from_string_length(argv[i]));
        }

        // parse line
        for (size_type i = 0ull; i < m_args.size(); ++i)
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
                m_params.emplace(name, m_args[i + 1]);
                ++i; // skip next value, it is not a free parameter
                continue;
            }

            m_flags.insert(name);
        }
    }

    cli_parser::args_view cli_parser::params(const string_view name) const
    {
        auto trimmed_name = internal::trim_leading_dashes(name);
        auto* values = m_params.find(trimmed_name);
        if (values && !values->empty())
        {
            return *values;
        }
        return {};
    }

    bool cli_parser::has_flag(const string_view name) const
    {
        return m_flags.contains(internal::trim_leading_dashes(name));
    }

    bool cli_parser::has_flag(array_view<string_view> aliases) const
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

    string_view cli_parser::operator()(const string_view name) const
    {
        return operator()(name, {});
    }

    string_view cli_parser::operator()(const std::initializer_list<char const* const> init_list) const
    {
        for (auto& name : init_list)
        {
            const auto* values = m_params.find(internal::trim_leading_dashes(string_view::from_string_length(name)));
            if (values && !values->empty())
            {
                return values->at(0ull);
            }
        }
        return {};
    }

    string_view cli_parser::operator()(const string_view name, const string_view defaultValue) const
    {
        const auto* values = m_params.find(internal::trim_leading_dashes(name));
        if (values && !values->empty())
        {
            return values->at(0ull);
        }

        return defaultValue;
    }

    string_view cli_parser::operator()(const std::initializer_list<char const* const> init_list, const string_view defaultValue) const
    {
        for (auto& name : init_list)
        {
            const auto* values = m_params.find(internal::trim_leading_dashes(string_view::from_string_length(name)));
            if (values && !values->empty())
            {
                return values->at(0ull);
            }
        }

        return defaultValue;
    }

    bool cli_parser::is_param(const string_view name) const
    {
        return m_registeredParams.contains(name);
    }
} // namespace rsl
