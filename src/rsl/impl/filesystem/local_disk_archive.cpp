#include "local_disk_archive.hpp"

#include "filesystem_error.hpp"
#include "path_util.hpp"
#include "view.hpp"

namespace rsl::fs
{
    bool local_disk_file_solution::is_file() const
    {
        return m_provider && m_provider->is_valid() && platform::is_regular_file(m_absolutePath);
    }

    bool local_disk_file_solution::is_directory() const
    {
        return m_provider && m_provider->is_valid() && platform::is_directory(m_absolutePath);
    }

    bool local_disk_file_solution::is_valid_path() const
    {
        return platform::is_path_valid(m_absolutePath);
    }

    bool local_disk_file_solution::can_be_written() const
    {
        return (is_file() && platform::is_file_writable(m_absolutePath)) ||
                (is_directory() && platform::is_path_writable(m_absolutePath));
    }

    bool local_disk_file_solution::can_be_read() const
    {
        return (is_file() && platform::is_file_readable(m_absolutePath)) ||
                (is_directory() && platform::is_path_readable(m_absolutePath));
    }

    bool local_disk_file_solution::can_be_created() const
    {
        return m_provider && m_provider->is_valid() && is_valid_path() && !platform::does_path_entry_exist(m_absolutePath);
    }

    bool local_disk_file_solution::exists() const
    {
        return m_provider && m_provider->is_valid() && platform::does_path_entry_exist(m_absolutePath);
    }

    result<dynamic_array<view>> local_disk_file_solution::ls() const
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

        result<iterator_view<directory_iterator>> iteratorViewResult = platform::iterate_directory(m_absolutePath);
        if (!iteratorViewResult.carries_value()) [[unlikely]]
        {
            return iteratorViewResult.propagate();
        }

        dynamic_array<view> result;
        for (auto path : iteratorViewResult.value())
        {
            result.emplace_back(path.get_path());
        }

        return result;
    }

    namespace
    {
        result<void> create_directory_recursive(string_view path)
        {
            string_view parentPath = parent(path);
            if (!platform::does_path_entry_exist(parentPath))
            {
                result<void> parentCreationResult = create_directory_recursive(parentPath);
                if (parentCreationResult.has_errors())
                {
                    return parentCreationResult.propagate();
                }
            }

            return platform::create_directory(path);
        }
    } // namespace

    result<void> local_disk_file_solution::create() const
    {
        if (m_absolutePath[m_absolutePath.size() - 1ull] == '\\')
        {
            return create_directory_recursive(m_absolutePath);
        }

        string_view parentPath = parent(m_absolutePath);
        if (!platform::does_path_entry_exist(parentPath))
        {
            result<void> parentCreationResult = create_directory_recursive(parentPath);
            if (parentCreationResult.has_errors())
            {
                return parentCreationResult.propagate();
            }
        }

        return platform::create_file(m_absolutePath);
    }

    result<byte_view> local_disk_file_solution::read() const
    {
        if (!exists()) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        if (!can_be_read()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_operation, "File can not be read.");
        }

        result<void> result = open_file_for_read();
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        return m_fileMapping.view_read();
    }

    result<void> local_disk_file_solution::write(byte_view data)
    {
        if (!exists()) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        if (!can_be_written()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_operation, "File can not be written.");
        }

        result<void> result = open_file_for_write();
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        return platform::write_file(m_openFile, data);
    }

    result<void> local_disk_file_solution::append(byte_view data)
    {
        if (!exists()) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        if (!can_be_written()) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_operation, "File can not be written.");
        }

        result<void> result = open_file_for_append();
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        return platform::append_file(m_openFile, data);
    }

    result<void> local_disk_file_solution::flush() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!m_provider || !m_provider->is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }
        return okay;
    }

    result<dynamic_array<view>> local_disk_archive::ls() const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider root path.");
            }
        }

        result<iterator_view<directory_iterator>> iteratorViewResult = platform::iterate_directory(m_rootPath);
        if (!iteratorViewResult.carries_value()) [[unlikely]]
        {
            return iteratorViewResult.propagate();
        }

        dynamic_array<view> result;
        for (auto entry : iteratorViewResult.value())
        {
            result.emplace_back(entry.get_path());
        }

        return result;
    }

    bool local_disk_archive::is_readonly() const
    {
        return !platform::is_path_writable(m_rootPath);
    }

    bool local_disk_archive::is_valid() const
    {
        return !m_rootPath.is_empty();
    }

    result<file_solution*> local_disk_archive::create_solution(const string_view path)
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

    void local_disk_archive::release_solution(file_solution* solution)
    {
        local_disk_file_solution* driveSolution = dynamic_cast<local_disk_file_solution*>(solution);
        if (!driveSolution) [[unlikely]]
        {
            return;
        }

        const size_type solutionIndex = find_existing_solution(driveSolution->m_virtualPath);
        if (solutionIndex == npos) [[unlikely]]
        {
            return;
        }

        destroy_solution_reference(driveSolution->m_virtualPath);
        if (get_reference_count_status(solutionIndex).is_free())
        {
            if (driveSolution->m_fileMapping)
            {
                platform::release_file_mapping(driveSolution->m_fileMapping);
            }
            if (driveSolution->m_openFile)
            {
                platform::close_file(driveSolution->m_openFile);
            }

            driveSolution->m_virtualPath.clear();
            driveSolution->m_absolutePath.clear();
            set_solution_provider(driveSolution, nullptr);
        }
    }

    result<void> local_disk_archive::open_file_for_read(const file_solution* solution) const
    {
        const local_disk_file_solution* driveSolution = dynamic_cast<const local_disk_file_solution*>(solution);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        if (driveSolution->m_fileMapping)
        {
            return okay;
        }

        file& platformFile = driveSolution->m_openFile;
        file_access_mode accessMode = file_access_mode::read;

        if (platformFile && !mode_available_for_read(platformFile.get_mode()))
        {
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        if (!platformFile)
        {
            result<file> openResult = platform::open_file(driveSolution->m_absolutePath, accessMode, driveSolution->m_accessFlags);
            if (openResult.has_errors()) [[unlikely]]
            {
                return openResult.propagate();
            }

            platformFile = *openResult;
        }

        result<file_mapping> mappingResult = platform::create_file_mapping(platformFile);
        if (mappingResult.has_errors()) [[unlikely]]
        {
            return mappingResult.propagate();
        }

        driveSolution->m_fileMapping = *mappingResult;
        return okay;
    }

    result<void> local_disk_archive::open_file_for_write(file_solution* solution)
    {
        const local_disk_file_solution* driveSolution = dynamic_cast<const local_disk_file_solution*>(solution);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        if (driveSolution->m_fileMapping)
        {
            return okay;
        }

        file& platformFile = driveSolution->m_openFile;
        file_access_mode accessMode = file_access_mode::write;

        if (platformFile && !mode_available_for_write(platformFile.get_mode()))
        {
            if (driveSolution->m_fileMapping)
            {
                platform::release_file_mapping(driveSolution->m_fileMapping);
            }
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        if (!platformFile)
        {
            result<file> openResult = platform::open_file(driveSolution->m_absolutePath, accessMode, driveSolution->m_accessFlags);
            if (openResult.has_errors()) [[unlikely]]
            {
                return openResult.propagate();
            }

            platformFile = *openResult;
        }

        return okay;
    }

    result<void> local_disk_archive::open_file_for_append(file_solution* solution)
    {
        const local_disk_file_solution* driveSolution = dynamic_cast<const local_disk_file_solution*>(solution);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        if (driveSolution->m_fileMapping)
        {
            return okay;
        }

        file& platformFile = driveSolution->m_openFile;
        file_access_mode accessMode = file_access_mode::append;

        if (platformFile && !mode_available_for_append(platformFile.get_mode()))
        {
            if (driveSolution->m_fileMapping)
            {
                platform::release_file_mapping(driveSolution->m_fileMapping);
            }
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        if (!platformFile)
        {
            result<file> openResult = platform::open_file(driveSolution->m_absolutePath, accessMode, driveSolution->m_accessFlags);
            if (openResult.has_errors()) [[unlikely]]
            {
                return openResult.propagate();
            }

            platformFile = *openResult;
        }

        return okay;
    }
} // namespace rsl::fs
