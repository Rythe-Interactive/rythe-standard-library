#pragma once
#include "../platform/platform.hpp"

#include "archive.hpp"
#include "view.hpp"

namespace rsl::fs
{
    struct local_disk_file_solution_data
    {
        dynamic_string virtualPath;
        dynamic_string absolutePath;
        file_access_flags accessFlags = file_access_flags::no_preference;
        mutable file openFile;
        mutable file_mapping fileMapping;
    };

    class local_disk_archive : public archive
    {
    public:
        VIRTUAL_RULE_OF_5(local_disk_archive);

        template <string_like Domain, string_like... Domains>
        local_disk_archive(string_view rootPath, Domain&& domain, Domains&&... domains);

        [[nodiscard]] result<dynamic_array<view>> ls() const override;
        [[nodiscard]] bool is_readonly() const override;
        [[nodiscard]] bool is_valid() const override;
        [[nodiscard]] result<file_solution_handle> create_solution(string_view path) override;
        void release_solution(file_solution_handle solution) override;

        [[nodiscard]] string_view get_absolute_path(file_solution solution) const noexcept;
        [[nodiscard]] string_view get_absolute_path(file_solution_handle handle) const noexcept;
        [[nodiscard]] bool is_file(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool is_directory(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool is_empty(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool is_valid_path(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool can_be_written(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool can_be_read(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool can_be_created(file_solution_handle solutionHandle) const override;
        [[nodiscard]] bool exists(file_solution_handle solutionHandle) const override;
        [[nodiscard]] result<dynamic_array<view>> ls(file_solution_handle solutionHandle) const override;
        void set_access_hint(file_solution_handle solutionHandle, file_access_flags flags) override;
        [[nodiscard]] result<void> create(file_solution_handle solutionHandle) const override;
        [[nodiscard]] result<void> delete_entry(file_solution_handle solutionHandle, file_delete_flags flags) const override;
        [[nodiscard]] result<byte_view> read(file_solution_handle solutionHandle) const override;
        [[nodiscard]] result<void> write(file_solution_handle solutionHandle, byte_view data) override;
        [[nodiscard]] result<void> append(file_solution_handle solutionHandle, byte_view data) override;
        [[nodiscard]] result<void> flush(file_solution_handle solutionHandle) const override;

    protected:
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer<local_disk_file_solution_data>
                get_solution_data(file_solution_handle handle) noexcept;
        [[nodiscard]] [[rythe_always_inline]] constexpr pointer<const local_disk_file_solution_data>
                get_solution_data(file_solution_handle handle) const noexcept;

        [[nodiscard]] result<void> open_file_for_read(file_solution_handle solutionHandle) const;
        [[nodiscard]] result<void> open_file_for_write(file_solution_handle solutionHandle);
        [[nodiscard]] result<void> open_file_for_append(file_solution_handle solutionHandle);

        dynamic_string m_rootPath;
        dynamic_array<local_disk_file_solution_data> m_solutions;
    };
}

#include "local_disk_archive.inl"
