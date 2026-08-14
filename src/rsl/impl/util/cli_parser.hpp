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
        using flags_container = dynamic_set<string_view>;
        using params_container = dynamic_multi_map<string_view, string_view>;
        using args_container = dynamic_array<string_view>;
        using args_iterator = args_container::const_iterator_type;
        using args_view = args_container::const_view_type;

        cli_parser() = default;

        void add_param(string_view name);
        template <string_like... Names>
        [[rythe_always_inline]] void add_params(Names&&... names);

        void parse(int argc, const char* const argv[]);

        flags_container const& flags() const { return m_flags; }
        params_container const& params() const { return m_params; }
        args_view params(string_view name) const;
        args_view pos_args() const { return m_posArgs; }

        args_iterator begin() const { return m_posArgs.cbegin(); }
        args_iterator end() const { return m_posArgs.cend(); }
        size_type size() const { return m_posArgs.size(); }

        bool has_flag(string_view name) const;
        bool has_flag(array_view<string_view> aliases) const;

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
        bool is_param(string_view name) const;

        dynamic_array<dynamic_string>
                m_args; // TODO(Glyn): all the other containers can use string_view, args already creates the memory
        params_container m_params;
        args_container m_posArgs;
        flags_container m_flags;
        rsl::dynamic_set<rsl::dynamic_string> m_registeredParams;
    };
} // namespace rsl

#include "cli_parser.inl"
