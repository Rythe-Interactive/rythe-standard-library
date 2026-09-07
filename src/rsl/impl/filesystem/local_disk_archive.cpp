#include "local_disk_archive.hpp"

#include "filesystem_error.hpp"
#include "path_util.hpp"
#include "view.hpp"

namespace rsl::fs
{
    string_view local_disk_archive::get_absolute_path(file_solution solution) const noexcept
    {
        if (solution.get_provider().ptr != this)
        {
            return {};
        }

        return get_absolute_path(get_solution_handle(solution));
    }

    string_view local_disk_archive::get_absolute_path(file_solution_handle handle) const noexcept
    {
        pointer<const local_disk_file_solution_data> data = get_solution_data(handle);
        if (!data)
        {
            return {};
        }

        return data->absolutePath;
    }

    bool local_disk_archive::is_file(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        return !path.is_empty() && platform::is_regular_file(path);
    }

    bool local_disk_archive::is_directory(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        return !path.is_empty() && platform::is_directory(path);
    }

    bool local_disk_archive::is_empty(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return false;
        }
        return (platform::is_regular_file(path) && platform::is_file_empty(path)) ||
                (platform::is_directory(path) && platform::is_directory_empty(path));
    }

    bool local_disk_archive::is_valid_path(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        return !path.is_empty() && platform::is_path_valid(path);
    }

    bool local_disk_archive::can_be_written(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return false;
        }

        return (platform::is_regular_file(path) && platform::is_file_writable(path)) ||
                 (platform::is_directory(path) && platform::is_path_writable(path));
    }

    bool local_disk_archive::can_be_read(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return false;
        }

        return (platform::is_regular_file(path) && platform::is_file_readable(path)) ||
                (platform::is_directory(path) && platform::is_path_readable(path));
    }

    bool local_disk_archive::can_be_created(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return false;
        }

        return platform::is_path_valid(path) && !platform::does_path_entry_exist(path);
    }

    bool local_disk_archive::exists(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return false;
        }

        return platform::does_path_entry_exist(path);
    }

    result<dynamic_array<view>> local_disk_archive::ls(file_solution_handle handle) const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!is_valid()) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_filesystem, "Invalid drive filesystem provider.");
            }
        }

        if constexpr (rythe_validate_high_impact)
        {
            if (!is_valid_path(handle)) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_solution, "Invalid drive file solution path.");
            }

            if (!exists(handle)) [[unlikely]]
            {
                return make_error(filesystem_error::directory_not_found);
            }

            if (!is_directory(handle)) [[unlikely]]
            {
                return make_error(filesystem_error::invalid_operation, "Solution is not a directory to enumerate.");
            }
        }

        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return make_error(filesystem_error::invalid_solution, "Invalid drive file solution.");
        }

        result<iterator_view<directory_iterator>> iteratorViewResult = platform::iterate_directory(path);
        if (!iteratorViewResult.carries_value()) [[unlikely]]
        {
            return iteratorViewResult.propagate();
        }

        view rootPath( path, true );
        dynamic_array<view> result;
        for (const directory_entry& entry : iteratorViewResult.value())
        {
            result.emplace_back(rootPath/entry.get_path());
        }

        return result;
    }

    void local_disk_archive::set_access_hint(file_solution_handle solutionHandle, file_access_flags flags)
    {
        pointer<local_disk_file_solution_data> data = get_solution_data(solutionHandle);
        if (!data)
        {
            return;
        }

        data->accessFlags = flags;
    }

    namespace
    {
        result<void> create_directory_recursive(const string_view path)
        {
            const string_view parentPath = parent(path);
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

    result<void> local_disk_archive::create(file_solution_handle handle) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return make_error(filesystem_error::invalid_solution, "Invalid drive file solution.");
        }

        if (path[path.size() - 1ull] == separator_char{})
        {
            return create_directory_recursive(path);
        }

        string_view parentPath = parent(path);
        if (!platform::does_path_entry_exist(parentPath))
        {
            result<void> parentCreationResult = create_directory_recursive(parentPath);
            if (parentCreationResult.has_errors())
            {
                return parentCreationResult.propagate();
            }
        }

        return platform::create_file(path);
    }

    result<void> local_disk_archive::delete_entry(file_solution_handle handle, file_delete_flags flags) const
    {
        const string_view path = get_absolute_path(handle);
        if (path.is_empty())
        {
            return make_error(filesystem_error::invalid_solution, "Invalid drive file solution.");
        }

        if (is_directory(handle))
        {
            return platform::delete_directory(path, flags);
        }

        return platform::delete_file(path, flags);
    }

    result<byte_view> local_disk_archive::read(file_solution_handle handle) const
    {
        if (!exists(handle)) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        result<void> result = open_file_for_read(handle);
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        pointer<const local_disk_file_solution_data> solutionData = get_solution_data(handle);
        return solutionData->fileMapping.view_read();
    }

    result<void> local_disk_archive::write(file_solution_handle handle,byte_view data)
    {
        if (!exists(handle)) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        result<void> result = open_file_for_write(handle);
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        pointer<local_disk_file_solution_data> solutionData = get_solution_data(handle);
        return platform::write_file(solutionData->openFile, data);
    }

    result<void> local_disk_archive::append(file_solution_handle handle,byte_view data)
    {
        if (!exists(handle)) [[unlikely]]
        {
            return make_error(filesystem_error::file_not_found);
        }

        result<void> result = open_file_for_append(handle);
        if (result.has_errors()) [[unlikely]]
        {
            return result.propagate();
        }

        pointer<local_disk_file_solution_data> solutionData = get_solution_data(handle);
        return platform::append_file(solutionData->openFile, data);
    }

    result<void> local_disk_archive::flush(file_solution_handle) const
    {
        if constexpr (rythe_validate_low_impact)
        {
            if (!is_valid()) [[unlikely]]
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

    result<file_solution_handle> local_disk_archive::create_solution(const string_view path)
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

        dynamic_string standardizedPath = standardize(path);

        auto [index, newValue] = create_solution_reference(standardizedPath);

        file_solution_handle handle = get_solution_handle(index);

        if (newValue)
        {
            if (index >= m_solutions.size())
            {
                m_solutions.resize(index + 1);
            }

            auto& solution = m_solutions[index];
            solution.virtualPath = rsl::move(standardizedPath);
            solution.absolutePath = rsl::move(absolutePath);
        }

        return handle;
    }

    void local_disk_archive::release_solution(file_solution_handle solutionHandle)
    {
        const index_type solutionIndex = get_solution_index(solutionHandle);
        if (solutionIndex == npos) [[unlikely]]
        {
            return;
        }

        local_disk_file_solution_data& driveSolution = m_solutions[solutionIndex];

        destroy_solution_reference(driveSolution.virtualPath);
        if (get_reference_count_status(solutionIndex).is_free())
        {
            if (driveSolution.fileMapping)
            {
                platform::release_file_mapping(driveSolution.fileMapping);
            }
            if (driveSolution.openFile)
            {
                platform::close_file(driveSolution.openFile);
            }

            driveSolution.virtualPath.clear();
            driveSolution.absolutePath.clear();
        }
    }

    result<void> local_disk_archive::open_file_for_read(file_solution_handle solutionHandle) const
    {
        pointer<const local_disk_file_solution_data> driveSolution = get_solution_data(solutionHandle);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        if (driveSolution->fileMapping)
        {
            return okay;
        }

        file& platformFile = driveSolution->openFile;
        file_access_mode accessMode = file_access_mode::read;

        if (platformFile)
        {
            rsl_assert_medium_impact(!mode_available_for_read(platformFile.get_mode()));
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        rsl_assert_medium_impact(!platformFile);

        result<file> openResult = platform::open_file(driveSolution->absolutePath, accessMode, driveSolution->accessFlags);
        if (openResult.has_errors()) [[unlikely]]
        {
            return openResult.propagate();
        }

        platformFile = *openResult;

        result<file_mapping> mappingResult = platform::create_file_mapping(platformFile);
        if (mappingResult.has_errors()) [[unlikely]]
        {
            return mappingResult.propagate();
        }

        driveSolution->fileMapping = *mappingResult;

        return okay;
    }

    result<void> local_disk_archive::open_file_for_write(file_solution_handle solutionHandle)
    {
        pointer<local_disk_file_solution_data> driveSolution = get_solution_data(solutionHandle);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        file& platformFile = driveSolution->openFile;
        file_access_mode accessMode = file_access_mode::write;

        if (platformFile && !mode_available_for_write(platformFile.get_mode()))
        {
            if (driveSolution->fileMapping)
            {
                platform::release_file_mapping(driveSolution->fileMapping);
            }
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        if (!platformFile)
        {
            result<file> openResult = platform::open_file(driveSolution->absolutePath, accessMode, driveSolution->accessFlags);
            if (openResult.has_errors()) [[unlikely]]
            {
                return openResult.propagate();
            }

            platformFile = *openResult;

            if (mode_available_for_read(accessMode))
            {
                result<file_mapping> mappingResult = platform::create_file_mapping(platformFile);
                if (mappingResult.has_errors()) [[unlikely]]
                {
                    return mappingResult.propagate();
                }

                driveSolution->fileMapping = *mappingResult;
            }
        }

        return okay;
    }

    result<void> local_disk_archive::open_file_for_append(file_solution_handle solutionHandle)
    {
        pointer<local_disk_file_solution_data> driveSolution = get_solution_data(solutionHandle);
        if (!driveSolution) [[unlikely]]
        {
            return make_error(filesystem_error::invalid_filesystem);
        }

        file& platformFile = driveSolution->openFile;
        file_access_mode accessMode = file_access_mode::append;

        if (platformFile && !mode_available_for_append(platformFile.get_mode()))
        {
            if (driveSolution->fileMapping)
            {
                platform::release_file_mapping(driveSolution->fileMapping);
            }
            platform::close_file(platformFile);
            accessMode = file_access_mode::read_write_append;
        }

        if (!platformFile)
        {
            result<file> openResult = platform::open_file(driveSolution->absolutePath, accessMode, driveSolution->accessFlags);
            if (openResult.has_errors()) [[unlikely]]
            {
                return openResult.propagate();
            }

            platformFile = *openResult;

            if (mode_available_for_read(accessMode))
            {
                result<file_mapping> mappingResult = platform::create_file_mapping(platformFile);
                if (mappingResult.has_errors()) [[unlikely]]
                {
                    return mappingResult.propagate();
                }

                driveSolution->fileMapping = *mappingResult;
            }
        }

        return okay;
    }
} // namespace rsl::fs
