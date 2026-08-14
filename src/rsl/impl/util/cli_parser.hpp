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

        void add_param(string_view name, string_view usage = {});
        template <same_as<pair<string_view, string_view>>... Names>
        [[rythe_always_inline]] void add_params(Names... names);

        void parse(int argc, const char* const argv[]);

        [[nodiscard]] [[rythe_always_inline]] flags_container const& flags() const noexcept { return m_flags; }
        [[nodiscard]] [[rythe_always_inline]] params_container const& params() const noexcept { return m_params; }
        [[nodiscard]] [[rythe_always_inline]] args_view pos_args() const noexcept { return m_posArgs; }

        [[nodiscard]] [[rythe_always_inline]] args_iterator begin() const noexcept { return m_posArgs.cbegin(); }
        [[nodiscard]] [[rythe_always_inline]] args_iterator end() const noexcept { return m_posArgs.cend(); }
        [[nodiscard]] [[rythe_always_inline]] size_type size() const noexcept { return m_posArgs.size(); }

        [[nodiscard]] bool has_flag(string_view name) const noexcept;
        [[nodiscard]] bool has_flag(array_view<const string_view> aliases) const noexcept;

        // Get first param in the list of values, or an invalid string view if not present
        [[nodiscard]] string_view get_param(string_view name) const noexcept;
        [[nodiscard]] string_view get_param(array_view<const string_view> aliases) const noexcept;

        [[nodiscard]] args_view get_params(string_view name) const noexcept;
        [[nodiscard]] args_view get_params(array_view<const string_view> aliases) const noexcept;

    private:
        bool is_param(string_view name) const noexcept;

        dynamic_array<dynamic_string> m_args;
        params_container m_params;
        args_container m_posArgs;
        flags_container m_flags;
        dynamic_map<dynamic_string, dynamic_string> m_registeredParams;
    };
} // namespace rsl

#include "cli_parser.inl"
