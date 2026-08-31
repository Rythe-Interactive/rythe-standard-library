#pragma once
#include "../rsl_core.hpp"

#include "../containers/string.hpp"
#include "../platform/file.hpp"
#include "../util/error_handling.hpp"

#include "file_solution.hpp"
#include "path_util.hpp"
#include "traits.hpp"

namespace rsl
{
    namespace fs
    {
        struct view_list;

        class view
        {
        public:
            RULE_OF_5_CONSTEXPR_NOEXCEPT(view)

            explicit constexpr view(string_view path, bool standardizePath = true) noexcept;
            constexpr view(dynamic_string&& path, bool standardizePath = true) noexcept;

            [[rythe_always_inline]] constexpr void standardize() noexcept;

            [[nodiscard]] [[rythe_always_inline]] operator bool() const noexcept;
            [[nodiscard]] [[rythe_always_inline]] bool is_valid(bool deepCheck = false) const;

            [[rythe_always_inline]] result<void> prefetch_solution(bool ignoreMultipleSolutions = !rythe_validate_high_impact) const;

            [[nodiscard]] [[rythe_always_inline]] bool exists() const;
            [[nodiscard]] [[rythe_always_inline]] bool is_file() const;
            [[nodiscard]] [[rythe_always_inline]] bool is_directory() const;
            [[nodiscard]] [[rythe_always_inline]] bool is_empty() const;
            [[nodiscard]] result<void> create() const;
            [[nodiscard]] result<void> delete_entry(file_delete_flags flags = file_delete_flags::none) const;

            [[nodiscard]] file_traits file_info() const;
            [[nodiscard]] filesystem_traits filesystem_info() const;

            [[nodiscard]] [[rythe_always_inline]] dynamic_string domain() const;
            [[nodiscard]] [[rythe_always_inline]] string_view path() const noexcept;
            [[nodiscard]] [[rythe_always_inline]] string_view extension(bool fullExtension = false) const noexcept;
            [[nodiscard]] [[rythe_always_inline]] string_view filename() const noexcept;
            [[nodiscard]] [[rythe_always_inline]] string_view stem() const noexcept;

            [[nodiscard]] [[rythe_always_inline]] view parent() const;
            [[nodiscard]] [[rythe_always_inline]] view subdir(string_view identifier) const;
            [[nodiscard]] [[rythe_always_inline]] view operator/(string_view identifier) const;

            [[nodiscard]] result<view_list> ls() const;

            template <invocable<void(view&)> Func>
            void iterate_recursive(Func&& func, bool reportErrors = false) const;

            view& replace_extension(string_view extension, bool fullExtension = false);

            [[nodiscard]] result<byte_view> read() const;
            [[nodiscard]] result<void> write(byte_view data);
            [[nodiscard]] result<void> append(byte_view data);
            [[nodiscard]] result<void> flush() const;

            [[nodiscard]] pointer<file_solution> get_solution();
            [[nodiscard]] pointer<const file_solution> get_solution() const;

        protected:
            void set_path(dynamic_string&& path);
            void release_solution() const;
            [[nodiscard]] result<const file_solution*> find_solution() const;
            [[nodiscard]] result<file_solution*> find_solution();

        private:
            dynamic_string m_path;
            mutable file_solution m_solution;
        };

        constexpr fs::view test()
        {
            return fs::view();
        }

        struct view_list : public dynamic_array<fs::view>
        {
            RULE_OF_5_CONSTEXPR_NOEXCEPT(view_list)
            using dynamic_array<fs::view>::dynamic_array;
            [[rythe_always_inline]] constexpr view_list(const dynamic_array<fs::view>&) noexcept;
            [[rythe_always_inline]] constexpr view_list(dynamic_array<fs::view>&&) noexcept;
        };
    } // namespace fs

    inline namespace literals
    {
        constexpr fs::view operator""_fsview(const cstring str, const size_type len) noexcept
        {
            return fs::view(string_view::from_buffer(str, len));
        }
    }
} // namespace rsl

#include "view.inl"
