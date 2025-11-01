#include "filesystem.hpp"

#include "file_solution.hpp"
#include "local_disk_archive.hpp"

namespace rsl::fs
{
    namespace internal
    {
        filesystem construct_filesystem() noexcept
        {
            static archive_registry registry;

            for (auto driveName : platform::enumerate_drives())
            {
                hybrid_string<64> domain = hybrid_string<64>::from_view(driveName);
                domain.append('/');
                standardize( in_place_signal, domain);
                registry.register_provider<local_disk_archive>(driveName + '\\', domain);
            }

            filesystem instance;
            instance.add_registry(&registry);
            return instance;
        }

        filesystem& get_default_filesystem() noexcept
        {
            static filesystem instance = construct_filesystem();
            return instance;
        }
    } // namespace internal

    result<file_solution*> filesystem::find_solution(const string_view path, const bool ignoreMultipleSolutions)
    {
        if (ignoreMultipleSolutions)
        {
            for (auto* registry : m_archiveRegistries)
            {
                result<file_solution*> solution = registry->find_solution(path, true);
                if (solution.carries_value())
                {
                    return solution;
                }
            }

            return make_error(filesystem_error::no_solution_found);
        }

        file_solution* solution = nullptr;
        for (auto* registry : m_archiveRegistries)
        {
            if (auto result = registry->find_solution(path); result.carries_value())
            {
                if (solution) [[unlikely]]
                {
                    solution->release();
                    result.value()->release();
                    return make_error(filesystem_error::multiple_solutions_found);
                }

                solution = result.value();
            }
            else
            {
                result.resolve();
            }
        }

        if (solution)
        {
            return solution;
        }

        return make_error(filesystem_error::no_solution_found);
    }

    get_filesystem_func get_filesystem = &internal::get_default_filesystem;
}
