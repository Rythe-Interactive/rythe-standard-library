#include "drive_filesystem_provider.hpp"

#include "filesystem_error.hpp"
#include "path_util.hpp"

namespace rsl::filesystem
{
    bool drive_file_solution::is_file() const
    {
        return m_provider && m_provider->is_valid() &&
                platform::is_regular_file(m_absolutePath);
    }

    bool drive_file_solution::is_directory() const
    {
        return m_provider && m_provider->is_valid() &&
                platform::is_directory(m_absolutePath);
    }

    bool drive_file_solution::is_valid_path() const
    {
        return platform::is_path_valid(m_absolutePath);
    }

    bool drive_file_solution::can_be_written() const
    {
        return (is_file() && platform::is_file_writable(m_absolutePath)) ||
                (is_directory() && platform::is_path_writable(m_absolutePath));
    }

    bool drive_file_solution::can_be_read() const
    {
        return (is_file() && platform::is_file_readable(m_absolutePath)) ||
                (is_directory() && platform::is_path_readable(m_absolutePath));
    }

    bool drive_file_solution::can_be_created() const
    {
        return m_provider && m_provider->is_valid() &&
                is_valid_path() &&
                !platform::does_path_entry_exist(m_absolutePath);
    }

    bool drive_file_solution::exists() const
    {
        return m_provider && m_provider->is_valid() &&
                platform::does_path_entry_exist(m_absolutePath);
    }

    result<dynamic_array<view>> drive_file_solution::ls() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        if constexpr (rythe_validate_high_impact)
        {
            if (!is_valid_path()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_solution, "Invalid drive file solution path.");
            }

            if (!exists()) [[unlikely]]
            {
                return make_error(filesystem_error::directory_not_found);
            }

            if (!is_directory()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_operation, "Solution is not a directory to enumerate.");
            }
        }

        dynamic_array<view> result;
        for (auto path : platform::enumerate_directory(m_absolutePath))
        {
            result.emplace_back(rsl::move(path));
        }

        return result;
    }

    result<byte_view> drive_file_solution::read() const
    {

        result<void> result = open_file_for_read(this);

        if (!exists()) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        if (!can_be_read()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_operation, "File can not be read.");
        }

    }

    result<void> drive_file_solution::write(byte_view data) {}

    result<void> drive_file_solution::append(byte_view data) {}

    result<void> drive_file_solution::flush() const {}

    result<dynamic_array<view>> drive_filesystem_provider::ls() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider root path.");
            }
        }

        dynamic_array<view> result;
        for (auto path : platform::enumerate_directory(m_rootPath))
        {
            result.emplace_back(rsl::move(path));
        }

        return result;
    }

    bool drive_filesystem_provider::is_readonly() const
    {
        return !platform::is_path_writable(m_rootPath);
    }

    bool drive_filesystem_provider::is_valid() const
    {
        return !m_rootPath.empty();
    }

    result<file_solution*> drive_filesystem_provider::create_solution(const string_view path)
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider root path.");
            }
        }

        dynamic_string absolutePath = replace_domain(path, m_rootPath);
        localize(in_place_signal, absolutePath);

        if constexpr (rythe_validate_medium_impact)
        {
            if (!platform::is_path_valid(absolutePath)) [[unlikely]]
            {
                return make_error(filesystem_error::no_solution_found, "Invalid path.");
            }
        }

        auto [index, newValue] = create_solution_reference(path);
        if (newValue)
        {
            if (index >= m_solutions.size())
            {
                m_solutions.resize(index + 1);
            }

            auto& solution = m_solutions[index];

            set_solution_provider(&solution, this);
            solution.m_virtualPath = dynamic_string::from_view(path);
            solution.m_absolutePath = rsl::move(absolutePath);
        }

        return &m_solutions[index];
    }

    void drive_filesystem_provider::release_solution(file_solution* solution)
    {
        drive_file_solution* driveSolution = dynamic_cast<drive_file_solution*>(solution);
        if (!driveSolution)
        {
            return;
        }

        const size_type solutionIndex = find_existing_solution(driveSolution->m_virtualPath);
        if (solutionIndex == npos)
        {
            return;
        }

        destroy_solution_reference(driveSolution->m_virtualPath);
        if (get_reference_count_status(solutionIndex).is_free())
        {
            driveSolution->m_virtualPath.clear();
            driveSolution->m_absolutePath.clear();
            set_solution_provider(driveSolution, nullptr);
        }
    }

    result<void> drive_filesystem_provider::open_file_for_read(const file_solution* solution) const
    {

    }

    result<void> drive_filesystem_provider::open_file_for_write(file_solution* solution) {}
    result<void> drive_filesystem_provider::close_file(const file_solution* solution) const {}
    result<void> drive_filesystem_provider::flush_file(const file_solution* solution) const {}
}
