#include "filesystem_registry.hpp"

#include "../platform/platform.hpp"

#include "drive_filesystem_provider.hpp"
#include "file_solution.hpp"
#include "filesystem_error.hpp"
#include "path_util.hpp"

namespace rsl
{
    namespace fs = filesystem;
}

namespace rsl::filesystem
{
    namespace internal
    {
        filesystem_registry construct_default_registry() noexcept
        {
            filesystem_registry registry;

            for (auto driveName : platform::enumerate_drives())
            {
                hybrid_string<64> domain = hybrid_string<64>::from_view(driveName);
                domain.append('/');
                standardize( in_place_signal, domain);
                registry.register_provider<drive_filesystem_provider>(driveName + '\\', domain);
            }

            return registry;
        }

        filesystem_registry& get_default_filesystem_registry() noexcept
        {
            static filesystem_registry registry = construct_default_registry();
            return registry;
        }
    } // namespace internal

    get_filesystem_registry_func get_filesystem_registry = &internal::get_default_filesystem_registry;

    void filesystem_registry::register_provider(temporary_object<filesystem_provider>&& provider)
    {
        filesystem_provider* entry = m_providers.emplace_back(rsl::move(provider)).get();
        for (const auto& domain : entry->get_domains())
        {
            m_domainMap.emplace(domain, entry);
        }
    }

    result<file_solution*> filesystem_registry::find_solution(const string_view path, const bool ignoreMultipleSolutions)
    {
        const string_view domain = fs::domain(path);

        dynamic_array<filesystem_provider*>* providers = m_domainMap.find(domain);
        if (!providers) [[unlikely]]
        {
            return make_error(filesystem_error::domain_not_found);
        }

        if (ignoreMultipleSolutions)
        {
            for (auto* provider : *providers)
            {
                auto result = provider->create_solution(path);
                if (result.is_okay())
                {
                    return *result;
                }
                result.resolve();
            }
        }
        else
        {
            file_solution* solution = nullptr;
            for (auto* provider : *providers)
            {
                if (auto result = provider->create_solution(path); result.is_okay())
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
        }

        return make_error(filesystem_error::no_solution_found);
    }

    bool filesystem_registry::has_domain(const string_view domain) const noexcept
    {
        return m_domainMap.contains(domain);
    }

    array_view<const unique_object<filesystem_provider>> filesystem_registry::providers() const noexcept
    {
        return m_providers;
    }

    iterator_view<domain_iterator> filesystem_registry::domains() const noexcept
    {
        return iterator_view(domain_iterator(m_providers.begin()), domain_iterator(m_providers.end()));
    }
}
