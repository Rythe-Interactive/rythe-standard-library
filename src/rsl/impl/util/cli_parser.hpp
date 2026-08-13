#pragma once
#include "../containers/string.hpp"
#include "../containers/map/dynamic_map.hpp"
#include "../containers/map/dynamic_set.hpp"


namespace rsl
{
    // Based on Argh: https://github.com/adishavit/argh
    // Copyright (c) 2016, Adi Shavit
    // All rights reserved.
    
   class cli_parser
    {
    public:
        using flags_container = dynamic_set<dynamic_string>;
        using params_container = dynamic_multi_map<dynamic_string, dynamic_string>;
        using args_container = dynamic_array<dynamic_string>;
        using args_iterator = args_container::const_iterator_type;

        cli_parser() = default;

        void add_param(string_view name);
        template<string_like... Names>
        void add_params(Names&&... names);

        void parse(int argc, const char* const argv[]);

        flags_container const& flags() const { return m_flags; }
        params_container const& params() const { return m_params; }
        array_view<const dynamic_string> params(string_view name) const;
        array_view<const dynamic_string> pos_args() const { return m_posArgs; }

        args_iterator begin() const { return m_posArgs.cbegin(); }
        args_iterator end() const { return m_posArgs.cend(); }
        size_type size() const { return m_posArgs.size(); }

        //////////////////////////////////////////////////////////////////////////
        // Accessors

        // flag (boolean) accessors: return true if the flag appeared, otherwise false.
        bool operator[](string_view name) const;

        // multiple flag (boolean) accessors: return true if at least one of the flag appeared, otherwise false.
        bool operator[](std::initializer_list<char const* const> init_list) const;

        // returns positional arg string by order. Like argv[] but without the options
        string_view operator[](size_type index) const;

        // returns a std::istream that can be used to convert a positional arg to a typed value.
        string_view operator()(size_type index) const;

        // same as above, but with a default value in case the arg is missing (index out of range).
        string_view operator()(size_type index, string_view defaultValue) const;

        // parameter accessors, give a name get an std::istream that can be used to convert to a typed value.
        // call .str() on result to get as string
        string_view operator()(string_view name) const;

        // accessor for a parameter with multiple names, give a list of names, get an std::istream that can be used to convert to a
        // typed value. call .str() on result to get as string returns the first value in the list to be found.
        string_view operator()(std::initializer_list<char const* const> init_list) const;

        // same as above, but with a default value in case the param was missing.
        // Non-string defaultValue types must have an operator<<() (output stream operator)
        // If T only has an input stream operator, pass the string version of the type as in "3" instead of 3.
        string_view operator()(string_view name, string_view defaultValue) const;

        // same as above but for a list of names. returns the first value to be found.
        string_view operator()(std::initializer_list<char const* const> init_list, string_view defaultValue) const;

    private:
        string_view trim_leading_dashes(string_view name) const;
        bool is_number(string_view arg) const;
        bool is_option(string_view arg) const;
        bool got_flag(string_view name) const;
        bool is_param(string_view name) const;

    private:
        args_container m_args; // TODO(Glyn): all the other containers can use string_view, args already creates the memory
        params_container m_params;
        args_container m_posArgs;
        flags_container m_flags;
        rsl::dynamic_set<rsl::dynamic_string> m_registeredParams;
    };

    inline void cli_parser::parse(const int argc, const char* const argv[])
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
            if (!is_option(m_args[i]))
            {
                m_posArgs.emplace_back(m_args[i]);
                continue;
            }

            const string_view name = trim_leading_dashes(m_args[i]);

            const size_type equalPos = linear_search(name, '=');
            if (equalPos != npos)
            {
                m_params.emplace(name.subview(0, equalPos), name.subview(equalPos + 1));
                continue;
            }

            // any potential option will get as its value the next arg, unless that arg is an option too
            // in that case it will be determined a flag.
            if (i == m_args.size() - 1 || is_option(m_args[i + 1]))
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

    inline bool cli_parser::is_number([[maybe_unused]] const string_view arg) const
    {
        // TODO(Glyn): from_chars implementation
        return false;
    }

    inline bool cli_parser::is_option(const string_view arg) const
    {
        rsl_assert_invalid_parameters(!arg.empty());
        if (is_number(arg)) { return false; }
        return '-' == arg[0];
    }

    inline string_view cli_parser::trim_leading_dashes(const string_view name) const
    {
        size_type pos = linear_search_not_eq(name, '-');
        return pos != npos ? name.subview(pos) : name;
    }

    inline bool cli_parser::got_flag(const string_view name) const
    {
        return m_flags.contains(trim_leading_dashes(name));
    }

    inline bool cli_parser::is_param(const string_view name) const
    {
        return m_registeredParams.contains(name);
    }

    inline bool cli_parser::operator[](const string_view name) const
    {
        return got_flag(name);
    }

    inline bool cli_parser::operator[](std::initializer_list<char const* const> init_list) const
    {
        return std::any_of(init_list.begin(), init_list.end(), [&](char const* const name) { return got_flag(string_view::from_string_length(name)); });
    }

    inline string_view cli_parser::operator[](size_type index) const
    {
        if (index < m_posArgs.size())
        {
            return m_posArgs[index];
        }

        return {};
    }

    inline string_view cli_parser::operator()(string_view name) const
    {
        return operator()(name, {});
    }

    inline string_view cli_parser::operator()(std::initializer_list<char const* const> init_list) const
    {
        for (auto& name : init_list)
        {
            const auto* values = m_params.find(trim_leading_dashes(string_view::from_string_length(name)));
            if (values && !values->empty())
            {
                return values->at(0ull);
            }
        }
        return {};
    }

    inline string_view cli_parser::operator()(string_view name, string_view defaultValue) const
    {
        const auto* values = m_params.find(trim_leading_dashes(name));
        if (values && !values->empty())
        {
            return values->at(0ull);
        }

        return defaultValue;
    }

    inline string_view cli_parser::operator()(std::initializer_list<char const* const> init_list, string_view defaultValue) const
    {
        for (auto& name : init_list)
        {
            const auto* values = m_params.find(trim_leading_dashes(string_view::from_string_length(name)));
            if (values && !values->empty())
            {
                return values->at(0ull);
            }
        }

        return defaultValue;
    }

    inline string_view cli_parser::operator()(size_type index) const
    {
        return operator()(index, {});
    }

    inline string_view cli_parser::operator()(size_type index, string_view defaultValue) const
    {
        if (index < m_posArgs.size())
        {
            return m_posArgs[index];
        }
        return defaultValue;
    }

    inline void cli_parser::add_param(string_view name)
    {
        m_registeredParams.insert(trim_leading_dashes(name));
    }

    template <string_like... Names>
    inline void cli_parser::add_params(Names&&... names)
    {
        (add_param(view_from_stringish(rsl::forward<Names>(names))), ...);
    }

    inline array_view<const dynamic_string> cli_parser::params(string_view name) const
    {
        auto trimmed_name = trim_leading_dashes(name);
        auto* values = m_params.find(trimmed_name);
        if (values && !values->empty())
        {
            return *values;
        }
        return {};
    }
} // namespace rsl
