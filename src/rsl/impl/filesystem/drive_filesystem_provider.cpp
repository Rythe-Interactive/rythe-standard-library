#include "drive_filesystem_provider.hpp"

#include "../platform/platform.hpp"

#include "filesystem_error.hpp"
#include "path_util.hpp"

namespace rsl::filesystem
{

    file_traits drive_file_solution::file_info() const
    {
        if (!m_provider) [[unlikely]]
        {
            return invalid_file_traits;
        }

        file_traits traits;
        traits.isDirectory = m_absolutePath.back() == separator();
        traits.isFile = !traits.isDirectory;
        traits.isValidPath = true;
        traits.canBeWritten = traits.isFile;
        traits.canBeRead = traits.isFile;
        traits.canBeCreated = true;
        traits.exists = true;

        inherit_traits(m_provider->filesystem_info(), traits);

        return traits;
    }

    result<dynamic_array<view>> drive_filesystem_provider::ls() const
    {
        if (!is_valid()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider root path.");
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
        return platform::is_path_readonly(m_rootPath);
    }

    bool drive_filesystem_provider::is_valid() const
    {
        return platform::is_path_valid(m_rootPath) && platform::is_directory(m_rootPath);
    }

    result<file_solution*> drive_filesystem_provider::create_solution(const string_view path)
    {
        if (!is_valid()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider root path.");
        }

        dynamic_string absolutePath = replace_domain(path, m_rootPath);
        localize(in_place_signal, absolutePath);

        if (!platform::is_path_valid(absolutePath)) [[unlikely]]
        {
            return make_error(filesystem_error::no_solution_found, "Invalid path.");
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

        size_type solutionIndex = find_existing_solution(driveSolution->m_virtualPath);
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
}
