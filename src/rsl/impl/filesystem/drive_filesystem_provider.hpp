#pragma once

#include "file_solution.hpp"
#include "filesystem_provider.hpp"
#include "path_util.hpp"

namespace rsl::filesystem
{
    class drive_file_solution : public file_solution
    {
    public:
        VIRTUAL_RULE_OF_5(drive_file_solution)

        [[nodiscard]] [[rythe_always_inline]] string_view get_absolute_path() const noexcept { return m_absolutePath; }

        [[nodiscard]] file_traits file_info() const override;
        [[nodiscard]] result<dynamic_array<view>> ls() const override;
        [[nodiscard]] result<byte_view> read() const override;
        [[nodiscard]] result<void> write(byte_view data) override;
        [[nodiscard]] result<void> append(byte_view data) override;
        [[nodiscard]] result<void> flush() const override;

    private:
        friend class drive_filesystem_provider;
        dynamic_string m_virtualPath;
        dynamic_string m_absolutePath;
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
        dynamic_string m_rootPath;
        dynamic_array<drive_file_solution> m_solutions;
    };
}

#include "drive_filesystem_provider.inl"
