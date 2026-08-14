#pragma once
#include "../time_span.hpp"

// TODO(Glyn): Implement a propper time formatter
template <typename Char>
struct fmt::formatter<rsl::time_span, Char>
{
private:
    format_specs specs_;
    detail::arg_ref<Char> width_ref_;
    detail::arg_ref<Char> precision_ref_;
    basic_string_view<Char> fmt_;

public:
    constexpr const Char* parse(format_parse_context& ctx)
    {
        auto it = ctx.begin(), end = ctx.end();
        if (it == end || *it == '}') { return it; }

        it = detail::parse_align(it, end, specs_);
        if (it == end) { return it; }

        Char c = *it;
        if ((c >= '0' && c <= '9') || c == '{')
        {
            it = detail::parse_width(it, end, specs_, width_ref_, ctx);
            if (it == end) { return it; }
        }

        auto checker = detail::chrono_format_checker();
        if (*it == '.')
        {
            checker.has_precision_integral = false;
            it = detail::parse_precision(it, end, specs_, precision_ref_, ctx);
        }
        if (it != end && *it == 'L')
        {
            specs_.set_localized();
            ++it;
        }
        end = detail::parse_chrono_format(it, end, checker);
        fmt_ = { it, detail::to_unsigned(end - it) };
        return end;
    }

    template <typename FormatContext>
    auto format(rsl::time_span time, FormatContext& ctx) const
    {
        auto specs = specs_;
        auto precision = specs.precision;
        specs.precision = -1;
        auto begin = fmt_.begin(), end = fmt_.end();
        // As a possible future optimization, we could avoid extra copying if width
        // is not specified.
        auto buf = basic_memory_buffer<Char>();
        auto out = basic_appender<Char>(buf);
        detail::handle_dynamic_spec(specs.dynamic_width(), specs.width, width_ref_, ctx);
        detail::handle_dynamic_spec(specs.dynamic_precision(), precision, precision_ref_, ctx);
        if (begin == end || *begin == '}')
        {
            if (time.days<int>())
            {
                out = detail::format_duration_value<Char>(out, time.days<float>(), precision);
                out = detail::copy_unit("days", out, Char());
            }
            else if (time.hours<int>())
            {
                out = detail::format_duration_value<Char>(out, time.hours<float>(), precision);
                out = detail::copy_unit("hrs", out, Char());
            }
            else if (time.minutes<int>())
            {
                out = detail::format_duration_value<Char>(out, time.minutes<float>(), precision);
                out = detail::copy_unit("min", out, Char());
            }
            else if (time.seconds<int>())
            {
                out = detail::format_duration_value<Char>(out, time.seconds<float>(), precision);
                out = detail::copy_unit("s", out, Char());
            }
            else if (time.milliseconds<int>())
            {
                out = detail::format_duration_value<Char>(out, time.milliseconds<float>(), precision);
                out = detail::copy_unit("ms", out, Char());
            }
            else if (time.microseconds<int>())
            {
                out = detail::format_duration_value<Char>(out, time.microseconds<float>(), precision);
                out = detail::copy_unit("us", out, Char());
            }
            else
            {
                out = detail::format_duration_value<Char>(out, time.nanoseconds<float>(), precision);
                out = detail::copy_unit("ns", out, Char());
            }
        }
        else
        {
            auto f = detail::duration_formatter<Char, rsl::int64, std::nano>(out, std::chrono::nanoseconds(time.value), ctx.locale());
            f.precision = precision;
            f.localized = specs_.localized();
            detail::parse_chrono_format(begin, end, f);
        }
        return detail::write(ctx.out(), basic_string_view<Char>(buf.data(), buf.size()), specs);
    }
};
