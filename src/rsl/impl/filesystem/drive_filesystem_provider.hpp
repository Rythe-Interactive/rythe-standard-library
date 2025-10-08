#pragma once
#include "../platform/platform.hpp"

#include "file_solution.hpp"
#include "filesystem_provider.hpp"
#include "path_util.hpp"

namespace rsl::filesystem
{
    // TODO: drive doesn't say much, local_disk_archive would be a better name.
    class drive_file_solution : public file_solution
    {
    public:
        VIRTUAL_RULE_OF_5(drive_file_solution)

        [[nodiscard]] [[rythe_always_inline]] string_view get_absolute_path() const noexcept { return m_absolutePath; }

        [[nodiscard]] bool is_file() const override;
        [[nodiscard]] bool is_directory() const override;
        [[nodiscard]] bool is_valid_path() const override;
        [[nodiscard]] bool can_be_written() const override;
        [[nodiscard]] bool can_be_read() const override;
        [[nodiscard]] bool can_be_created() const override;
        [[nodiscard]] bool exists() const override;

        [[nodiscard]] result<dynamic_array<view>> ls() const override;
        [[nodiscard]] result<byte_view> read() const override;
        [[nodiscard]] result<void> write(byte_view data) override;
        [[nodiscard]] result<void> append(byte_view data) override;
        [[nodiscard]] result<void> flush() const override;

    private:
        friend class drive_filesystem_provider;
        dynamic_string m_virtualPath;
        dynamic_string m_absolutePath;
        mutable bool m_openForWrite;
        mutable file m_openFile;
    };

    class drive_filesystem_provider : public filesystem_provider
    {
    public:
        VIRTUAL_RULE_OF_5(drive_filesystem_provider);

        template <string_like Domain, string_like... Domains>
        drive_filesystem_provider(string_view rootPath, Domain&& domain, Domains&&... domains);

        [[nodiscard]] result<dynamic_array<view>> ls() const override;
        [[nodiscard]] bool is_readonly() const override;
        [[nodiscard]] bool is_valid() const override;
        [[nodiscard]] result<file_solution*> create_solution(string_view path) override;
        void release_solution(file_solution* solution) override;

    protected:
        friend class drive_file_solution;
        [[nodiscard]] result<void> open_file_for_read(const file_solution* solution) const override;
        [[nodiscard]] result<void> open_file_for_write(file_solution* solution) override;
        [[nodiscard]] result<void> close_file(const file_solution* solution) const override;
        [[nodiscard]] result<void> flush_file(const file_solution* solution) const override;

        dynamic_string m_rootPath;
        dynamic_array<drive_file_solution> m_solutions;
    };
}

#include "drive_filesystem_provider.inl"
