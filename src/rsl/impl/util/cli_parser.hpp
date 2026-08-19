#pragma once
#include "../containers/string.hpp"
#include "../containers/map/dynamic_map.hpp"
#include "../containers/map/dynamic_set.hpp"
#include "../logging/logging.hpp"

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

        void set_command_usage(string_view usage);

        void add_param(string_view name, bool isFlag = false, string_view usagePattern = "{}");
        void add_param(array_view<const string_view> aliases, bool isFlag = false, string_view usagePattern = "{}");

        void parse(int argc, const char* const argv[]);

        [[nodiscard]] [[rythe_always_inline]] flags_container const& flags() const noexcept { return m_flags; }
        [[nodiscard]] [[rythe_always_inline]] params_container const& params() const noexcept { return m_params; }
        [[nodiscard]] [[rythe_always_inline]] args_view pos_args() const noexcept { return m_posArgs; }

        [[nodiscard]] [[rythe_always_inline]] args_iterator begin() const noexcept { return m_posArgs.cbegin(); }
        [[nodiscard]] [[rythe_always_inline]] args_iterator end() const noexcept { return m_posArgs.cend(); }
        [[nodiscard]] [[rythe_always_inline]] size_type size() const noexcept { return m_posArgs.size(); }
        [[nodiscard]] [[rythe_always_inline]] bool is_empty() const noexcept { return m_posArgs.is_empty(); }

        [[nodiscard]] bool has_flag(string_view name) const noexcept;
        [[nodiscard]] bool has_flag(array_view<const string_view> aliases) const noexcept;

        // Get first param in the list of values, or an invalid string view if not present
        [[nodiscard]] string_view get_param(string_view name) const noexcept;
        [[nodiscard]] string_view get_param(array_view<const string_view> aliases) const noexcept;

        [[nodiscard]] args_view get_params(string_view name) const noexcept;
        [[nodiscard]] args_view get_params(array_view<const string_view> aliases) const noexcept;

        // Will use the default undecorated logger if logger is nullptr
        void print_current_command(rsl::log::severity severity = rsl::log::severity::info, pointer<rsl::log::logger> logger = { nullptr }) const;

        // Will use the default undecorated logger if logger is nullptr
        void print_usage(rsl::log::severity severity = rsl::log::severity::info, pointer<rsl::log::logger> logger = { nullptr }) const;

    private:
        bool is_param(string_view name) const noexcept;

        struct param_info
        {
            bool isFlag;
            string_view usagePattern;
        };

        string_view m_commandUsage;
        dynamic_array<string_view> m_args;
        params_container m_params;
        args_container m_posArgs;
        flags_container m_flags;
        dynamic_map<string_view, param_info> m_registeredParams;
    };
} // namespace rsl
